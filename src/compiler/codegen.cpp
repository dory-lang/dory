/*
 * codegen.cpp - Minimal LLVM backend (literals, assignments, functions)
 *
 * Generates IR for selected expressions and statements.
 * Handles static declarations and initializers.
 * Supports writing IR and object files.
 */

#include "codegen.hpp"

#include <cctype>
#include <cstdio>
#include <cstring>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/Config/llvm-config.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Triple.h>

struct CodeGenerator::Impl
{
   explicit Impl( const std::string &oModuleName )
       : m_Context(), m_Module( std::make_unique<llvm::Module>( oModuleName, m_Context ) ), m_Builder( m_Context )
   {
      llvm::Type *pOpaquePtr = llvm::PointerType::get( m_Context, 0 );
      m_TypeDoItem           = llvm::StructType::create( m_Context, "DO_ITEM" );
      m_TypeDoItem->setBody( { type_i32(), type_i32(), pOpaquePtr, type_i64() }, false );

      llvm::Type *pItemPtr = pOpaquePtr;
      m_TypeDoVm           = llvm::StructType::create( m_Context, "DO_VM" );
      m_TypeDoVm->setBody( { type_i32(), type_i32(), pItemPtr, m_TypeDoItem }, false );
   }

   llvm::LLVMContext                             m_Context;
   std::unique_ptr<llvm::Module>                 m_Module;
   llvm::IRBuilder<>                             m_Builder;
   llvm::Function                               *m_CurrentFunction = nullptr;
   llvm::StructType                             *m_TypeDoItem      = nullptr;
   llvm::StructType                             *m_TypeDoVm        = nullptr;
   std::unordered_set<std::string>               m_InternalFuncs;
   std::map<std::string, llvm::AllocaInst *>     m_Locals;
   std::map<std::string, llvm::Type *>           m_LocalTypes;
   std::map<std::string, llvm::GlobalVariable *> m_Globals;
   std::map<std::string, llvm::Type *>           m_GlobalTypes;
   std::map<std::string, llvm::GlobalVariable *> m_StaticLocals;
   std::vector<llvm::BasicBlock *>               m_BreakTargets;
   std::vector<llvm::BasicBlock *>               m_ContinueTargets;
   llvm::AllocaInst                             *m_ArrayNilTmp = nullptr;
   std::map<std::string, llvm::AllocaInst *>     m_EnumLocals;

   llvm::Type *type_i64()
   {
      return llvm::Type::getInt64Ty( m_Context );
   }
   llvm::Type *type_i1()
   {
      return llvm::Type::getInt1Ty( m_Context );
   }
   llvm::Type *type_i32()
   {
      return llvm::Type::getInt32Ty( m_Context );
   }

   llvm::StructType *type_do_item()
   {
      return m_TypeDoItem;
   }
   llvm::StructType *type_do_vm()
   {
      return m_TypeDoVm;
   }

   static constexpr int32_t kDoItNil     = 0;  /* DO_K_NIL */
   static constexpr int32_t kDoItPointer = 1;  /* DO_K_POINTER */
   static constexpr int32_t kDoItInt32   = 2;  /* DO_K_I32 */
   static constexpr int32_t kDoItLong    = 3;  /* DO_K_I64 */
   static constexpr int32_t kDoItLogical = 5;  /* DO_K_LOGICAL */
   static constexpr int32_t kDoItString  = 9;  /* DO_K_STRING */
   static constexpr int32_t kDoItArray   = 11; /* DO_K_ARRAY */
   static constexpr int32_t kDoFlagByRef = 0x00000100;
   static constexpr int32_t kDoKindMask  = 0x000000FF;

   llvm::Constant *const_i32( int32_t v )
   {
      return llvm::ConstantInt::get( type_i32(), v );
   }

   llvm::Constant *const_i64( int64_t v )
   {
      return llvm::ConstantInt::get( type_i64(), v );
   }

   llvm::Constant *const_null_ptr()
   {
      return llvm::ConstantPointerNull::get( llvm::PointerType::get( m_Context, 0 ) );
   }

   llvm::Value *make_item( llvm::Value *pType, llvm::Value *pPayload )
   {
      if( !pType || !pPayload )
         return nullptr;
      llvm::Value *pItem = llvm::UndefValue::get( type_do_item() );
      pItem              = m_Builder.CreateInsertValue( pItem, pType, 0 );
      pItem              = m_Builder.CreateInsertValue( pItem, const_i32( 0 ), 1 );
      pItem              = m_Builder.CreateInsertValue( pItem, const_null_ptr(), 2 );
      pItem              = m_Builder.CreateInsertValue( pItem, pPayload, 3 );
      return pItem;
   }

   llvm::Constant *make_item_const( int32_t nType, llvm::Constant *pPayload )
   {
      if( !pPayload )
         return nullptr;
      return llvm::ConstantStruct::get( type_do_item(),
                                        { const_i32( nType ), const_i32( 0 ), const_null_ptr(), pPayload } );
   }

   llvm::Value *item_payload( llvm::Value *pItem )
   {
      if( !pItem )
         return nullptr;
      return m_Builder.CreateExtractValue( pItem, 3, "payload" );
   }

   llvm::Value *item_type( llvm::Value *pItem )
   {
      if( !pItem )
         return nullptr;
      return m_Builder.CreateExtractValue( pItem, 0, "type" );
   }

   llvm::Value *item_kind( llvm::Value *pItem )
   {
      llvm::Value *pType = item_type( pItem );
      if( !pType )
         return nullptr;
      return m_Builder.CreateAnd( pType, const_i32( kDoKindMask ), "kind" );
   }

   llvm::Value *item_to_bool( llvm::Value *pItem )
   {
      llvm::Value *pPayload = item_payload( pItem );
      if( !pPayload )
         return nullptr;
      return m_Builder.CreateICmpNE( pPayload, const_i64( 0 ), "tobool" );
   }

   llvm::Value *item_to_i64( llvm::Value *pItem )
   {
      return item_payload( pItem );
   }

   llvm::Value *make_bool_item( llvm::Value *pBool )
   {
      if( !pBool )
         return nullptr;
      llvm::Value *pValue = m_Builder.CreateZExt( pBool, type_i64(), "bool64" );
      return make_item( const_i32( kDoItLogical ), pValue );
   }

   llvm::Value *make_int_item( llvm::Value *pInt )
   {
      if( !pInt )
         return nullptr;
      llvm::Value *pValue = pInt;
      if( pValue->getType() != type_i64() )
         pValue = m_Builder.CreateIntCast( pValue, type_i64(), true, "i64" );
      return make_item( const_i32( kDoItLong ), pValue );
   }

   const char *get_napis_cstr( Napis pNapis ) const
   {
      if( !pNapis )
         return nullptr;
      if( pNapis->kind == Napis_::is_NapisStr )
         return pNapis->u.napisStr_.strlit_;
      return nullptr;
   }

   std::string decode_string_literal( const char *szRaw ) const
   {
      if( !szRaw )
         return {};

      size_t nLen = std::strlen( szRaw );
      size_t nBeg = 0;
      size_t nEnd = nLen;
      if( nLen >= 2 &&
          ( ( szRaw[ 0 ] == '"' && szRaw[ nLen - 1 ] == '"' ) || ( szRaw[ 0 ] == '\'' && szRaw[ nLen - 1 ] == '\'' ) ) )
      {
         nBeg = 1;
         nEnd = nLen - 1;
      }

      std::string oText;
      oText.reserve( nEnd > nBeg ? ( nEnd - nBeg ) : 0 );
      for( size_t i = nBeg; i < nEnd; ++i )
      {
         char ch = szRaw[ i ];
         if( ch == '\\' && i + 1 < nEnd )
         {
            char esc = szRaw[ ++i ];
            switch( esc )
            {
            case 'n':
               oText.push_back( '\n' );
               break;
            case 'r':
               oText.push_back( '\r' );
               break;
            case 't':
               oText.push_back( '\t' );
               break;
            case '\\':
               oText.push_back( '\\' );
               break;
            case '"':
               oText.push_back( '"' );
               break;
            case '\'':
               oText.push_back( '\'' );
               break;
            default:
               oText.push_back( esc );
               break;
            }
         }
         else
            oText.push_back( ch );
      }

      return oText;
   }

   std::optional<std::string> get_string_literal_text( Expr pExpr ) const
   {
      if( !pExpr || pExpr->kind != Expr_::is_ExprLit )
         return std::nullopt;
      Literal pLit = pExpr->u.exprLit_.literal_;
      if( !pLit || pLit->kind != Literal_::is_LitStr )
         return std::nullopt;
      const char *szRaw = get_napis_cstr( pLit->u.litStr_.napis_ );
      if( !szRaw )
         return std::nullopt;
      return decode_string_literal( szRaw );
   }

   llvm::Constant *const_string_bits( const char *szText )
   {
      if( !szText )
         return nullptr;
      llvm::Constant *pData = llvm::ConstantDataArray::getString( m_Context, szText, true );
      auto *pGlob = new llvm::GlobalVariable( *m_Module, pData->getType(), true, llvm::GlobalValue::PrivateLinkage,
                                              pData, "str" );
      pGlob->setUnnamedAddr( llvm::GlobalValue::UnnamedAddr::Global );
      pGlob->setAlignment( llvm::MaybeAlign( 1 ) );
      llvm::Constant                  *pZero   = const_i32( 0 );
      llvm::Constant                  *pIdxs[] = { pZero, pZero };
      llvm::ArrayRef<llvm::Constant *> oIdx( pIdxs );
      llvm::Constant *pPtr = llvm::ConstantExpr::getInBoundsGetElementPtr( pData->getType(), pGlob, oIdx );
      return llvm::ConstantExpr::getPtrToInt( pPtr, type_i64() );
   }

   void collect_internal_funcs( Defs pDefs )
   {
      if( !pDefs )
         return;

      auto add_def = [ this ]( Def pDef )
      {
         if( !pDef )
            return;
         const char *szName = nullptr;
         switch( pDef->kind )
         {
         case Def_::is_DefFun:
            szName = pDef->u.defFun_.functiondef_->u.functionDefMain_.ident_;
            break;
         case Def_::is_DefProc:
            szName = pDef->u.defProc_.proceduredef_->u.procedureDefMain_.ident_;
            break;
         case Def_::is_DefMeth:
            szName = pDef->u.defMeth_.methoddef_->u.methodDefMain_.ident_;
            break;
         }
         if( szName )
            m_InternalFuncs.insert( szName );
      };

      switch( pDefs->kind )
      {
      case Defs_::is_ListDefSingle:
         add_def( pDefs->u.listDefSingle_.def_ );
         break;
      case Defs_::is_ListDefCons:
         add_def( pDefs->u.listDefCons_.def_ );
         collect_internal_funcs( pDefs->u.listDefCons_.defs_ );
         break;
      }
   }

   bool is_internal_func( const char *szName ) const
   {
      if( !szName )
         return false;
      return m_InternalFuncs.find( szName ) != m_InternalFuncs.end();
   }

   llvm::FunctionCallee get_do_func( const char *szName )
   {
      if( !szName )
         return nullptr;
      std::string oNorm;
      for( const unsigned char *p = ( const unsigned char * ) szName; *p; ++p )
         oNorm.push_back( ( char ) std::toupper( *p ) );
      std::string         oName  = std::string( "DO_" ) + oNorm;
      llvm::Type         *pVmPtr = llvm::PointerType::get( m_Context, 0 );
      llvm::FunctionType *pFT =
          llvm::FunctionType::get( llvm::Type::getVoidTy( m_Context ), std::vector<llvm::Type *>{ pVmPtr }, false );
      return m_Module->getOrInsertFunction( oName, pFT );
   }

   llvm::FunctionCallee get_rt_func( const char *szName, llvm::Type *pRet, std::vector<llvm::Type *> aArgs )
   {
      if( !szName || !pRet )
         return nullptr;
      llvm::FunctionType *pFT = llvm::FunctionType::get( pRet, aArgs, false );
      return m_Module->getOrInsertFunction( szName, pFT );
   }

   struct ConstValue
   {
      bool    valid   = false;
      bool    is_bool = false;
      int64_t i       = 0;
      bool    b       = false;
   };

   static ConstValue cv_invalid()
   {
      return ConstValue{};
   }

   static ConstValue cv_int( int64_t v )
   {
      ConstValue o;
      o.valid   = true;
      o.is_bool = false;
      o.i       = v;
      return o;
   }

   static ConstValue cv_bool( bool v )
   {
      ConstValue o;
      o.valid   = true;
      o.is_bool = true;
      o.b       = v;
      return o;
   }

   static bool cv_truthy( const ConstValue &o )
   {
      return o.is_bool ? o.b : ( o.i != 0 );
   }

   static int64_t cv_as_int( const ConstValue &o )
   {
      return o.is_bool ? ( o.b ? 1 : 0 ) : o.i;
   }

   static bool is_main_name( const char *szName )
   {
      if( !szName )
         return false;
      return ( ( szName[ 0 ] == 'm' || szName[ 0 ] == 'M' ) && ( szName[ 1 ] == 'a' || szName[ 1 ] == 'A' ) &&
               ( szName[ 2 ] == 'i' || szName[ 2 ] == 'I' ) && ( szName[ 3 ] == 'n' || szName[ 3 ] == 'N' ) &&
               szName[ 4 ] == '\0' );
   }

   enum CmpKind
   {
      CMP_EQ,
      CMP_NE,
      CMP_LT,
      CMP_LE,
      CMP_GT,
      CMP_GE
   };

   llvm::Value *codegen_compare( Expr pLeftExpr, Expr pRightExpr, CmpKind eKind )
   {
      llvm::Value *pLeft  = codegen_expr( pLeftExpr );
      llvm::Value *pRight = codegen_expr( pRightExpr );
      if( !pLeft || !pRight )
         return nullptr;

      llvm::Value *pLeftType  = item_type( pLeft );
      llvm::Value *pLeftKind  = item_kind( pLeft );
      llvm::Value *pRightKind = item_kind( pRight );
      if( !pLeftType || !pLeftKind || !pRightKind )
         return nullptr;

      llvm::Value *pIsStrLeft  = m_Builder.CreateICmpEQ( pLeftKind, const_i32( kDoItString ), "cmp.isstr.l" );
      llvm::Value *pIsStrRight = m_Builder.CreateICmpEQ( pRightKind, const_i32( kDoItString ), "cmp.isstr.r" );
      llvm::Value *pIsStrBoth  = m_Builder.CreateAnd( pIsStrLeft, pIsStrRight, "cmp.isstr.both" );
      llvm::Value *pIsArrLeft  = m_Builder.CreateICmpEQ( pLeftKind, const_i32( kDoItArray ), "cmp.isarr.l" );
      llvm::Value *pIsArrRight = m_Builder.CreateICmpEQ( pRightKind, const_i32( kDoItArray ), "cmp.isarr.r" );
      llvm::Value *pIsArrBoth  = m_Builder.CreateAnd( pIsArrLeft, pIsArrRight, "cmp.isarr.both" );

      llvm::Value *pIsNumLeft =
          m_Builder.CreateOr( m_Builder.CreateICmpEQ( pLeftKind, const_i32( kDoItInt32 ) ),
                              m_Builder.CreateICmpEQ( pLeftKind, const_i32( kDoItLong ) ), "cmp.isnum.l" );
      llvm::Value *pIsNumRight =
          m_Builder.CreateOr( m_Builder.CreateICmpEQ( pRightKind, const_i32( kDoItInt32 ) ),
                              m_Builder.CreateICmpEQ( pRightKind, const_i32( kDoItLong ) ), "cmp.isnum.r" );
      llvm::Value *pIsNumBoth = m_Builder.CreateAnd( pIsNumLeft, pIsNumRight, "cmp.isnum.both" );

      llvm::Type          *pI8PtrTy = llvm::PointerType::get( m_Context, 0 );
      llvm::FunctionCallee pStrcmp  = get_rt_func( "strcmp", type_i32(), { pI8PtrTy, pI8PtrTy } );
      llvm::FunctionCallee pErr =
          get_rt_func( "do_err_type", llvm::Type::getVoidTy( m_Context ), { pI8PtrTy, pI8PtrTy, type_i32() } );
      if( !pStrcmp || !pErr )
         return nullptr;

      llvm::Constant *pEmptyBits = const_string_bits( "" );
      if( !pEmptyBits )
         return nullptr;
      llvm::Constant *pEmptyPtr = llvm::ConstantExpr::getIntToPtr( pEmptyBits, pI8PtrTy );

      llvm::Function   *pFun    = m_CurrentFunction;
      llvm::BasicBlock *pStrBB  = llvm::BasicBlock::Create( m_Context, "cmp.str", pFun );
      llvm::BasicBlock *pChkBB  = llvm::BasicBlock::Create( m_Context, "cmp.chk", pFun );
      llvm::BasicBlock *pNumBB  = llvm::BasicBlock::Create( m_Context, "cmp.num", pFun );
      llvm::BasicBlock *pErrBB  = llvm::BasicBlock::Create( m_Context, "cmp.err", pFun );
      llvm::BasicBlock *pContBB = llvm::BasicBlock::Create( m_Context, "cmp.cont", pFun );
      llvm::BasicBlock *pArrBB  = nullptr;

      if( eKind == CMP_EQ || eKind == CMP_NE )
      {
         pArrBB                      = llvm::BasicBlock::Create( m_Context, "cmp.arr", pFun );
         llvm::BasicBlock *pStrChkBB = llvm::BasicBlock::Create( m_Context, "cmp.strchk", pFun );
         m_Builder.CreateCondBr( pIsArrBoth, pArrBB, pStrChkBB );
         m_Builder.SetInsertPoint( pStrChkBB );
         m_Builder.CreateCondBr( pIsStrBoth, pStrBB, pChkBB );
      }
      else
      {
         m_Builder.CreateCondBr( pIsStrBoth, pStrBB, pChkBB );
      }

      m_Builder.SetInsertPoint( pChkBB );
      m_Builder.CreateCondBr( pIsNumBoth, pNumBB, pErrBB );

      llvm::Value *pStrItem = nullptr;
      llvm::Value *pNumItem = nullptr;
      llvm::Value *pErrItem = nullptr;
      llvm::Value *pArrItem = nullptr;

      if( pArrBB )
      {
         m_Builder.SetInsertPoint( pArrBB );
         llvm::Value *pLeftBits  = item_to_i64( pLeft );
         llvm::Value *pRightBits = item_to_i64( pRight );
         llvm::Value *pArrBool   = nullptr;
         if( eKind == CMP_EQ )
            pArrBool = m_Builder.CreateICmpEQ( pLeftBits, pRightBits, "cmp.eq" );
         else
            pArrBool = m_Builder.CreateICmpNE( pLeftBits, pRightBits, "cmp.ne" );
         pArrItem = make_bool_item( pArrBool );
         m_Builder.CreateBr( pContBB );
      }

      m_Builder.SetInsertPoint( pStrBB );
      llvm::Value *pLeftBits     = item_to_i64( pLeft );
      llvm::Value *pRightBits    = item_to_i64( pRight );
      llvm::Value *pLeftNull     = m_Builder.CreateICmpEQ( pLeftBits, const_i64( 0 ), "cmp.l.null" );
      llvm::Value *pRightNull    = m_Builder.CreateICmpEQ( pRightBits, const_i64( 0 ), "cmp.r.null" );
      llvm::Value *pLeftPtr      = m_Builder.CreateIntToPtr( pLeftBits, pI8PtrTy, "cmp.l.ptr" );
      llvm::Value *pRightPtr     = m_Builder.CreateIntToPtr( pRightBits, pI8PtrTy, "cmp.r.ptr" );
      llvm::Value *pLeftPtrSafe  = m_Builder.CreateSelect( pLeftNull, pEmptyPtr, pLeftPtr, "cmp.l.safe" );
      llvm::Value *pRightPtrSafe = m_Builder.CreateSelect( pRightNull, pEmptyPtr, pRightPtr, "cmp.r.safe" );
      llvm::Value *pCmp          = m_Builder.CreateCall( pStrcmp, { pLeftPtrSafe, pRightPtrSafe }, "strcmp" );
      llvm::Value *pStrBool      = nullptr;
      switch( eKind )
      {
      case CMP_EQ:
         pStrBool = m_Builder.CreateICmpEQ( pCmp, const_i32( 0 ), "cmp.eq" );
         break;
      case CMP_NE:
         pStrBool = m_Builder.CreateICmpNE( pCmp, const_i32( 0 ), "cmp.ne" );
         break;
      case CMP_LT:
         pStrBool = m_Builder.CreateICmpSLT( pCmp, const_i32( 0 ), "cmp.lt" );
         break;
      case CMP_LE:
         pStrBool = m_Builder.CreateICmpSLE( pCmp, const_i32( 0 ), "cmp.le" );
         break;
      case CMP_GT:
         pStrBool = m_Builder.CreateICmpSGT( pCmp, const_i32( 0 ), "cmp.gt" );
         break;
      case CMP_GE:
         pStrBool = m_Builder.CreateICmpSGE( pCmp, const_i32( 0 ), "cmp.ge" );
         break;
      }
      pStrItem = make_bool_item( pStrBool );
      m_Builder.CreateBr( pContBB );

      m_Builder.SetInsertPoint( pNumBB );
      llvm::Value *pLeftInt  = item_to_i64( pLeft );
      llvm::Value *pRightInt = item_to_i64( pRight );
      llvm::Value *pNumBool  = nullptr;
      switch( eKind )
      {
      case CMP_EQ:
         pNumBool = m_Builder.CreateICmpEQ( pLeftInt, pRightInt, "cmp.eq" );
         break;
      case CMP_NE:
         pNumBool = m_Builder.CreateICmpNE( pLeftInt, pRightInt, "cmp.ne" );
         break;
      case CMP_LT:
         pNumBool = m_Builder.CreateICmpSLT( pLeftInt, pRightInt, "cmp.lt" );
         break;
      case CMP_LE:
         pNumBool = m_Builder.CreateICmpSLE( pLeftInt, pRightInt, "cmp.le" );
         break;
      case CMP_GT:
         pNumBool = m_Builder.CreateICmpSGT( pLeftInt, pRightInt, "cmp.gt" );
         break;
      case CMP_GE:
         pNumBool = m_Builder.CreateICmpSGE( pLeftInt, pRightInt, "cmp.ge" );
         break;
      }
      pNumItem = make_bool_item( pNumBool );
      m_Builder.CreateBr( pContBB );

      m_Builder.SetInsertPoint( pErrBB );
      llvm::Constant *pFuncNameBits = const_string_bits( "cmp" );
      llvm::Constant *pExpectedBits = const_string_bits( "string or numeric" );
      llvm::Value    *pFuncName     = llvm::ConstantExpr::getIntToPtr( pFuncNameBits, pI8PtrTy );
      llvm::Value    *pExpected     = llvm::ConstantExpr::getIntToPtr( pExpectedBits, pI8PtrTy );
      m_Builder.CreateCall( pErr, { pFuncName, pExpected, pLeftType } );
      pErrItem = make_item( const_i32( kDoItNil ), const_i64( 0 ) );
      m_Builder.CreateBr( pContBB );

      m_Builder.SetInsertPoint( pContBB );
      unsigned nPhi = pArrBB ? 4 : 3;
      auto    *pPhi = m_Builder.CreatePHI( type_do_item(), nPhi, "cmptmp" );
      pPhi->addIncoming( pStrItem, pStrBB );
      pPhi->addIncoming( pNumItem, pNumBB );
      pPhi->addIncoming( pErrItem, pErrBB );
      if( pArrBB )
         pPhi->addIncoming( pArrItem, pArrBB );
      return pPhi;
   }

   llvm::AllocaInst *get_or_create_alloca( const char *szName, llvm::Type *pType )
   {
      if( !szName || !pType )
         return nullptr;

      auto it = m_Locals.find( szName );
      if( it != m_Locals.end() )
         return it->second;

      llvm::IRBuilder<> oTmp( &m_CurrentFunction->getEntryBlock(), m_CurrentFunction->getEntryBlock().begin() );
      llvm::AllocaInst *pAlloca = oTmp.CreateAlloca( pType, nullptr, szName );
      m_Locals[ szName ]        = pAlloca;
      m_LocalTypes[ szName ]    = pType;
      return pAlloca;
   }

   llvm::AllocaInst *array_nil_ptr()
   {
      if( m_ArrayNilTmp )
         return m_ArrayNilTmp;
      if( !m_CurrentFunction )
         return nullptr;
      llvm::IRBuilder<> oTmp( &m_CurrentFunction->getEntryBlock(), m_CurrentFunction->getEntryBlock().begin() );
      m_ArrayNilTmp = oTmp.CreateAlloca( type_do_item(), nullptr, "arr.nil" );
      oTmp.CreateStore( make_item_const( kDoItNil, const_i64( 0 ) ), m_ArrayNilTmp );
      return m_ArrayNilTmp;
   }

   bool get_single_subscript( SubscriptList pList, Expr &oExpr )
   {
      if( !pList || pList->kind != SubscriptList_::is_SubscriptListSingle )
         return false;
      oExpr                   = pList->u.subscriptListSingle_.expr_;
      SubscriptListTail pTail = pList->u.subscriptListSingle_.subscriptlisttail_;
      return ( !pTail || pTail->kind == SubscriptListTail_::is_ListSubscriptTailEmpty );
   }

   static bool equals_ignore_case( const char *szLeft, const char *szRight )
   {
      if( !szLeft || !szRight )
         return false;
      while( *szLeft && *szRight )
      {
         char a = ( char ) std::toupper( ( unsigned char ) *szLeft );
         char b = ( char ) std::toupper( ( unsigned char ) *szRight );
         if( a != b )
            return false;
         ++szLeft;
         ++szRight;
      }
      return *szLeft == *szRight;
   }

   llvm::AllocaInst *get_enum_alloca( const char *szName ) const
   {
      auto it = m_EnumLocals.find( szName ? szName : "" );
      return it != m_EnumLocals.end() ? it->second : nullptr;
   }

   llvm::Value *codegen_enum_accessor( Expr pObjExpr, const char *szMeth )
   {
      if( !pObjExpr || !szMeth )
         return nullptr;
      if( pObjExpr->kind != Expr_::is_ExprQId )
         return nullptr;

      QualifiedId pQ = pObjExpr->u.exprQId_.qualifiedid_;
      if( !pQ || pQ->kind != QualifiedId_::is_QualifiedIdSingle )
         return nullptr;

      const char *szName = pQ->u.qualifiedIdSingle_.ident_;
      llvm::AllocaInst *pEnumAlloca = get_enum_alloca( szName );
      if( !pEnumAlloca )
         return nullptr;

      llvm::Value *pEnumPtr = m_Builder.CreateLoad( llvm::PointerType::get( m_Context, 0 ), pEnumAlloca, "enum" );
      llvm::Type  *pEnumPtrTy = llvm::PointerType::get( m_Context, 0 );
      llvm::Type  *pItemPtrTy = llvm::PointerType::get( m_Context, 0 );

      if( equals_ignore_case( szMeth, "enumIndex" ) || equals_ignore_case( szMeth, "__enumIndex" ) )
      {
         llvm::FunctionCallee pFn = get_rt_func( "do_enum_index", type_i64(), { pEnumPtrTy } );
         if( !pFn )
            return nullptr;
         llvm::Value *pIdx = m_Builder.CreateCall( pFn, { pEnumPtr }, "enum.idx" );
         return make_int_item( pIdx );
      }
      if( equals_ignore_case( szMeth, "enumIsFirst" ) || equals_ignore_case( szMeth, "__enumIsFirst" ) )
      {
         llvm::FunctionCallee pFn = get_rt_func( "do_enum_isfirst", type_i32(), { pEnumPtrTy } );
         if( !pFn )
            return nullptr;
         llvm::Value *pVal  = m_Builder.CreateCall( pFn, { pEnumPtr }, "enum.first" );
         llvm::Value *pBool = m_Builder.CreateICmpNE( pVal, const_i32( 0 ), "enum.firstb" );
         return make_bool_item( pBool );
      }
      if( equals_ignore_case( szMeth, "enumIsLast" ) || equals_ignore_case( szMeth, "__enumIsLast" ) )
      {
         llvm::FunctionCallee pFn = get_rt_func( "do_enum_islast", type_i32(), { pEnumPtrTy } );
         if( !pFn )
            return nullptr;
         llvm::Value *pVal  = m_Builder.CreateCall( pFn, { pEnumPtr }, "enum.last" );
         llvm::Value *pBool = m_Builder.CreateICmpNE( pVal, const_i32( 0 ), "enum.lastb" );
         return make_bool_item( pBool );
      }
      if( equals_ignore_case( szMeth, "enumValue" ) || equals_ignore_case( szMeth, "__enumValue" ) )
      {
         llvm::FunctionCallee pFn = get_rt_func( "do_enum_value_ptr", pItemPtrTy, { pEnumPtrTy } );
         if( !pFn )
            return nullptr;
         llvm::Value *pPtr = m_Builder.CreateCall( pFn, { pEnumPtr }, "enum.valptr" );
         return m_Builder.CreateLoad( type_do_item(), pPtr, "enum.val" );
      }
      if( equals_ignore_case( szMeth, "enumBase" ) || equals_ignore_case( szMeth, "__enumBase" ) )
      {
         llvm::FunctionCallee pFn = get_rt_func( "do_enum_base", pEnumPtrTy, { pEnumPtrTy } );
         if( !pFn )
            return nullptr;
         llvm::Value *pArrPtr = m_Builder.CreateCall( pFn, { pEnumPtr }, "enum.base" );
         llvm::Value *pBits   = m_Builder.CreatePtrToInt( pArrPtr, type_i64(), "enum.basebits" );
         return make_item( const_i32( kDoItArray ), pBits );
      }
      if( equals_ignore_case( szMeth, "enumKey" ) || equals_ignore_case( szMeth, "__enumKey" ) ||
          equals_ignore_case( szMeth, "enumStart" ) || equals_ignore_case( szMeth, "__enumStart" ) ||
          equals_ignore_case( szMeth, "enumSkip" ) || equals_ignore_case( szMeth, "__enumSkip" ) ||
          equals_ignore_case( szMeth, "enumStop" ) || equals_ignore_case( szMeth, "__enumStop" ) )
      {
         return make_item_const( kDoItNil, const_i64( 0 ) );
      }

      return nullptr;
   }

   std::vector<Expr> collect_subscripts( SubscriptList pList )
   {
      std::vector<Expr> aIdxs;
      if( !pList || pList->kind != SubscriptList_::is_SubscriptListSingle )
         return aIdxs;

      aIdxs.push_back( pList->u.subscriptListSingle_.expr_ );
      SubscriptListTail pTail = pList->u.subscriptListSingle_.subscriptlisttail_;
      while( pTail && pTail->kind == SubscriptListTail_::is_ListSubscriptTailCons )
      {
         aIdxs.push_back( pTail->u.listSubscriptTailCons_.expr_ );
         pTail = pTail->u.listSubscriptTailCons_.subscriptlisttail_;
      }
      return aIdxs;
   }

   llvm::Value *emit_array_at( llvm::Value *pArrPtr, llvm::Value *pIdxVal )
   {
      if( !pArrPtr || !pIdxVal || !m_CurrentFunction )
         return nullptr;

      llvm::Type          *pArrPtrTy  = llvm::PointerType::get( m_Context, 0 );
      llvm::Type          *pItemPtrTy = llvm::PointerType::get( m_Context, 0 );
      llvm::FunctionCallee pAt        = get_rt_func( "do_array_at", pItemPtrTy, { pArrPtrTy, type_i64() } );
      llvm::FunctionCallee pLen       = get_rt_func( "do_array_len", type_i64(), { pArrPtrTy } );
      llvm::FunctionCallee pErr       = get_rt_func( "do_err_bounds", llvm::Type::getVoidTy( m_Context ),
                                                     { llvm::PointerType::get( m_Context, 0 ), type_i64(), type_i64() } );
      if( !pAt || !pLen || !pErr )
         return nullptr;

      llvm::Value *pElemPtr = m_Builder.CreateCall( pAt, { pArrPtr, pIdxVal }, "arr.at" );
      llvm::Value *pIsNull  = m_Builder.CreateICmpEQ(
          pElemPtr, llvm::ConstantPointerNull::get( llvm::PointerType::get( m_Context, 0 ) ), "arr.null" );

      llvm::Function   *pFun    = m_CurrentFunction;
      llvm::BasicBlock *pOkBB   = llvm::BasicBlock::Create( m_Context, "arr.ok", pFun );
      llvm::BasicBlock *pErrBB  = llvm::BasicBlock::Create( m_Context, "arr.err", pFun );
      llvm::BasicBlock *pContBB = llvm::BasicBlock::Create( m_Context, "arr.cont", pFun );
      m_Builder.CreateCondBr( pIsNull, pErrBB, pOkBB );

      m_Builder.SetInsertPoint( pErrBB );
      llvm::Value    *pLenVal       = m_Builder.CreateCall( pLen, { pArrPtr }, "arr.len" );
      llvm::Constant *pFuncNameBits = const_string_bits( "index" );
      if( !pFuncNameBits )
         return nullptr;
      llvm::Value *pFuncName = llvm::ConstantExpr::getIntToPtr( pFuncNameBits, llvm::PointerType::get( m_Context, 0 ) );
      m_Builder.CreateCall( pErr, { pFuncName, pIdxVal, pLenVal } );
      llvm::Value *pNilPtr = array_nil_ptr();
      if( !pNilPtr )
         return nullptr;
      m_Builder.CreateBr( pContBB );

      m_Builder.SetInsertPoint( pOkBB );
      m_Builder.CreateBr( pContBB );

      m_Builder.SetInsertPoint( pContBB );
      auto *pPhi = m_Builder.CreatePHI( llvm::PointerType::get( m_Context, 0 ), 2, "arr.ptr" );
      pPhi->addIncoming( pElemPtr, pOkBB );
      pPhi->addIncoming( pNilPtr, pErrBB );
      return pPhi;
   }

   std::vector<Expr> collect_expr_list( ExprListOpt pList )
   {
      std::vector<Expr> aItems;
      if( pList && pList->kind == ExprListOpt_::is_OptExprListSome )
      {
         aItems.push_back( pList->u.optExprListSome_.expr_ );
         ExprListTail pTail = pList->u.optExprListSome_.exprlisttail_;
         while( pTail && pTail->kind == ExprListTail_::is_ListExprTailCons )
         {
            aItems.push_back( pTail->u.listExprTailCons_.expr_ );
            pTail = pTail->u.listExprTailCons_.exprlisttail_;
         }
      }
      return aItems;
   }

   std::vector<Ident> collect_foreach_vars( ForEachVars pVars )
   {
      std::vector<Ident> aVars;
      if( pVars && pVars->kind == ForEachVars_::is_ForEachVarsMain )
      {
         aVars.push_back( pVars->u.forEachVarsMain_.ident_ );
         ForEachVarsTail pTail = pVars->u.forEachVarsMain_.foreachvarstail_;
         while( pTail && pTail->kind == ForEachVarsTail_::is_ListForEachVarsTailCons )
         {
            aVars.push_back( pTail->u.listForEachVarsTailCons_.ident_ );
            pTail = pTail->u.listForEachVarsTailCons_.foreachvarstail_;
         }
      }
      return aVars;
   }

   std::vector<Expr> collect_foreach_exprs( ForEachExprs pExprs )
   {
      std::vector<Expr> aExprs;
      if( pExprs && pExprs->kind == ForEachExprs_::is_ForEachExprsMain )
      {
         aExprs.push_back( pExprs->u.forEachExprsMain_.expr_ );
         ForEachExprsTail pTail = pExprs->u.forEachExprsMain_.foreachexprstail_;
         while( pTail && pTail->kind == ForEachExprsTail_::is_ListForEachExprsTailCons )
         {
            aExprs.push_back( pTail->u.listForEachExprsTailCons_.expr_ );
            pTail = pTail->u.listForEachExprsTailCons_.foreachexprstail_;
         }
      }
      return aExprs;
   }

   llvm::Value *codegen_lvalue_ptr( LValue pLVal )
   {
      if( !pLVal )
         return nullptr;

      switch( pLVal->kind )
      {
      case LValue_::is_LValQId:
      {
         QualifiedId pQ = pLVal->u.lValQId_.qualifiedid_;
         if( !pQ || pQ->kind != QualifiedId_::is_QualifiedIdSingle )
            return nullptr;
         const char *szName = pQ->u.qualifiedIdSingle_.ident_;
         std::string oName  = szName ? szName : "";

         auto itStatic = m_StaticLocals.find( oName );
         if( itStatic != m_StaticLocals.end() )
            return itStatic->second;

         auto itGlob = m_Globals.find( oName );
         if( itGlob != m_Globals.end() )
            return itGlob->second;

         auto itLocal = m_Locals.find( oName );
         if( itLocal != m_Locals.end() )
            return itLocal->second;

         if( !szName )
            return nullptr;
         return get_or_create_alloca( szName, type_do_item() );
      }
      case LValue_::is_LValIndex:
      {
         std::vector<Expr> aIdxs = collect_subscripts( pLVal->u.lValIndex_.subscriptlist_ );
         if( aIdxs.empty() )
            return nullptr;

         llvm::Value *pBasePtr = codegen_lvalue_ptr( pLVal->u.lValIndex_.lvalue_ );
         if( !pBasePtr )
            return nullptr;
         llvm::Value *pBaseItem = m_Builder.CreateLoad( type_do_item(), pBasePtr, "arr" );

         for( size_t i = 0; i < aIdxs.size(); ++i )
         {
            llvm::Value *pBaseKind = item_kind( pBaseItem );
            if( !pBaseKind )
               return nullptr;
            llvm::Value *pIsArray = m_Builder.CreateICmpEQ( pBaseKind, const_i32( kDoItArray ), "isarray" );

            llvm::Function   *pFun    = m_CurrentFunction;
            llvm::BasicBlock *pOkBB   = llvm::BasicBlock::Create( m_Context, "arr.ok", pFun );
            llvm::BasicBlock *pErrBB  = llvm::BasicBlock::Create( m_Context, "arr.err", pFun );
            llvm::BasicBlock *pContBB = llvm::BasicBlock::Create( m_Context, "arr.cont", pFun );
            m_Builder.CreateCondBr( pIsArray, pOkBB, pErrBB );

            llvm::Value *pArrPtr = nullptr;

            m_Builder.SetInsertPoint( pOkBB );
            llvm::Value *pArrBits = item_to_i64( pBaseItem );
            if( !pArrBits )
               return nullptr;
            pArrPtr = m_Builder.CreateIntToPtr( pArrBits, llvm::PointerType::get( m_Context, 0 ), "arrptr" );
            m_Builder.CreateBr( pContBB );

            m_Builder.SetInsertPoint( pErrBB );
            llvm::Type          *pI8PtrTy = llvm::PointerType::get( m_Context, 0 );
            llvm::FunctionCallee pErr =
                get_rt_func( "do_err_type", llvm::Type::getVoidTy( m_Context ), { pI8PtrTy, pI8PtrTy, type_i32() } );
            llvm::Constant *pFuncNameBits = const_string_bits( "index" );
            llvm::Constant *pExpectedBits = const_string_bits( "array" );
            if( !pErr || !pFuncNameBits || !pExpectedBits )
               return nullptr;
            llvm::Value *pFuncName = llvm::ConstantExpr::getIntToPtr( pFuncNameBits, pI8PtrTy );
            llvm::Value *pExpected = llvm::ConstantExpr::getIntToPtr( pExpectedBits, pI8PtrTy );
            llvm::Value *pBaseType = item_type( pBaseItem );
            if( !pBaseType )
               return nullptr;
            m_Builder.CreateCall( pErr, { pFuncName, pExpected, pBaseType } );
            llvm::Value *pNilPtr = array_nil_ptr();
            if( !pNilPtr )
               return nullptr;
            m_Builder.CreateBr( pContBB );

            m_Builder.SetInsertPoint( pContBB );
            auto *pArrPhi = m_Builder.CreatePHI( llvm::PointerType::get( m_Context, 0 ), 2, "arrptrphi" );
            pArrPhi->addIncoming( pArrPtr, pOkBB );
            pArrPhi->addIncoming( pNilPtr, pErrBB );

            llvm::Value *pIdxItem = codegen_expr( aIdxs[ i ] );
            if( !pIdxItem )
               return nullptr;
            llvm::Value *pIdxVal = item_to_i64( pIdxItem );
            if( !pIdxVal )
               return nullptr;

            llvm::Value *pElemPtr = emit_array_at( pArrPhi, pIdxVal );
            if( !pElemPtr )
               return nullptr;

            if( i + 1 == aIdxs.size() )
               return pElemPtr;

            pBaseItem = m_Builder.CreateLoad( type_do_item(), pElemPtr, "arr.next" );
         }
         return nullptr;
      }
      case LValue_::is_LValField:
         return nullptr;
      }

      return nullptr;
   }

   llvm::Value *codegen_lhs_ptr( LHS pLhs )
   {
      if( !pLhs )
         return nullptr;

      switch( pLhs->kind )
      {
      case LHS_::is_LHSVal:
         return codegen_lvalue_ptr( pLhs->u.lHSVal_.lvalue_ );
      case LHS_::is_LHSDeref:
      {
         llvm::Value *pItem = codegen_expr( pLhs->u.lHSDeref_.expr_ );
         if( !pItem )
            return nullptr;
         llvm::Value *pBits = item_to_i64( pItem );
         if( !pBits )
            return nullptr;
         return m_Builder.CreateIntToPtr( pBits, llvm::PointerType::get( m_Context, 0 ), "derefptr" );
      }
      }

      return nullptr;
   }

   llvm::Value *codegen_literal( Literal pLit )
   {
      if( !pLit )
         return nullptr;

      switch( pLit->kind )
      {
      case Literal_::is_LitInt:
         return make_int_item( const_i64( std::stoll( pLit->u.litInt_.intlit_ ) ) );
      case Literal_::is_LitTrueDot:
         return make_bool_item( llvm::ConstantInt::get( type_i1(), 1 ) );
      case Literal_::is_LitFalseDot:
         return make_bool_item( llvm::ConstantInt::get( type_i1(), 0 ) );
      case Literal_::is_LitNil:
         return make_item( const_i32( kDoItNil ), const_i64( 0 ) );
      case Literal_::is_LitStr:
      {
         const char *szRaw = get_napis_cstr( pLit->u.litStr_.napis_ );
         if( !szRaw )
            return nullptr;
         std::string           oText = decode_string_literal( szRaw );
         llvm::GlobalVariable *pGlob = m_Builder.CreateGlobalString( oText, "str" );
         llvm::Value          *pPtr  = m_Builder.CreateConstGEP2_32( pGlob->getValueType(), pGlob, 0, 0 );
         llvm::Value          *pBits = m_Builder.CreatePtrToInt( pPtr, type_i64(), "strbits" );
         return make_item( const_i32( kDoItString ), pBits );
      }
      default:
         return nullptr;
      }
   }

   std::vector<Expr> collect_args( ArgListOpt pArgs )
   {
      std::vector<Expr> aArgs;
      if( pArgs && pArgs->kind == ArgListOpt_::is_OptArgListSome )
      {
         aArgs.push_back( pArgs->u.optArgListSome_.expr_ );
         ArgListTail pTail = pArgs->u.optArgListSome_.arglisttail_;
         while( pTail && pTail->kind == ArgListTail_::is_ListArgTailCons )
         {
            aArgs.push_back( pTail->u.listArgTailCons_.expr_ );
            pTail = pTail->u.listArgTailCons_.arglisttail_;
         }
      }
      return aArgs;
   }

   llvm::Value *emit_do_call( const char *szName, const std::vector<Expr> &aArgs )
   {
      llvm::AllocaInst *pArgv = nullptr;
      if( !aArgs.empty() )
      {
         pArgv = m_Builder.CreateAlloca( type_do_item(), const_i32( ( int ) aArgs.size() ), "argv" );
         for( size_t i = 0; i < aArgs.size(); ++i )
         {
            llvm::Value *pArg = codegen_expr( aArgs[ i ] );
            if( !pArg )
               return nullptr;
            llvm::Value *pItemPtr = m_Builder.CreateGEP( type_do_item(), pArgv, const_i32( ( int ) i ) );
            m_Builder.CreateStore( pArg, pItemPtr );
         }
      }

      llvm::AllocaInst *pVm = m_Builder.CreateAlloca( type_do_vm(), nullptr, "vm" );

      llvm::Value *pArgcPtr   = m_Builder.CreateStructGEP( type_do_vm(), pVm, 0 );
      llvm::Value *pVmPadPtr  = m_Builder.CreateStructGEP( type_do_vm(), pVm, 1 );
      llvm::Value *pVmArgvPtr = m_Builder.CreateStructGEP( type_do_vm(), pVm, 2 );

      m_Builder.CreateStore( const_i32( ( int ) aArgs.size() ), pArgcPtr );
      m_Builder.CreateStore( const_i32( 0 ), pVmPadPtr );
      llvm::Value *pArgvCast =
          pArgv ? m_Builder.CreateBitCast( pArgv, llvm::PointerType::get( m_Context, 0 ) ) : const_null_ptr();
      m_Builder.CreateStore( pArgvCast, pVmArgvPtr );

      llvm::Value *pRetPtr = m_Builder.CreateStructGEP( type_do_vm(), pVm, 3 );
      m_Builder.CreateStore( make_item_const( kDoItNil, const_i64( 0 ) ), pRetPtr );

      m_Builder.CreateCall( get_do_func( szName ), std::vector<llvm::Value *>{ pVm } );
      return m_Builder.CreateLoad( type_do_item(), pRetPtr, "ret" );
   }

   llvm::Value *codegen_call_expr( CallExpr pCall )
   {
      if( !pCall || pCall->kind != CallExpr_::is_CallExprMain )
         return nullptr;

      QualifiedId pQ = pCall->u.callExprMain_.qualifiedid_;
      if( !pQ || pQ->kind != QualifiedId_::is_QualifiedIdSingle )
         return nullptr;

      const char *szName = pQ->u.qualifiedIdSingle_.ident_;
      if( !szName )
         return nullptr;

      std::vector<Expr> aArgs = collect_args( pCall->u.callExprMain_.arglistopt_ );
      if( is_internal_func( szName ) )
      {
         if( !aArgs.empty() )
            return nullptr;
         llvm::Function *pFun = m_Module->getFunction( szName );
         if( !pFun )
         {
            llvm::FunctionType *pFT = llvm::FunctionType::get( type_do_item(), std::vector<llvm::Type *>{}, false );
            pFun = llvm::Function::Create( pFT, llvm::Function::ExternalLinkage, szName, m_Module.get() );
         }
         return m_Builder.CreateCall( pFun, {} );
      }

      return emit_do_call( szName, aArgs );
   }

   ConstValue const_eval_expr_or( ExprOr pExpr )
   {
      if( !pExpr )
         return cv_invalid();

      switch( pExpr->kind )
      {
      case ExprOr_::is_ExprOrBase:
         return const_eval_expr( pExpr->u.exprOrBase_.expr_ );
      case ExprOr_::is_ExprOrDot:
      {
         ConstValue oLeft  = const_eval_expr( pExpr->u.exprOrDot_.expr_ );
         ConstValue oRight = const_eval_expr_or( pExpr->u.exprOrDot_.expror_ );
         if( !oLeft.valid || !oRight.valid )
            return cv_invalid();
         return cv_bool( cv_truthy( oLeft ) || cv_truthy( oRight ) );
      }
      }

      return cv_invalid();
   }

   ConstValue const_eval_expr( Expr pExpr )
   {
      if( !pExpr )
         return cv_invalid();

      switch( pExpr->kind )
      {
      case Expr_::is_ExprAssignBase:
         return const_eval_expr_or( pExpr->u.exprAssignBase_.expror_ );
      case Expr_::is_ExprAndDot:
      {
         ConstValue oLeft  = const_eval_expr( pExpr->u.exprAndDot_.expr_1 );
         ConstValue oRight = const_eval_expr( pExpr->u.exprAndDot_.expr_2 );
         if( !oLeft.valid || !oRight.valid )
            return cv_invalid();
         return cv_bool( cv_truthy( oLeft ) && cv_truthy( oRight ) );
      }
      case Expr_::is_ExprAndBase:
         return const_eval_expr( pExpr->u.exprAndBase_.expr_ );
      case Expr_::is_ExprNotDot:
      {
         ConstValue oVal = const_eval_expr( pExpr->u.exprNotDot_.expr_ );
         if( !oVal.valid )
            return cv_invalid();
         return cv_bool( !cv_truthy( oVal ) );
      }
      case Expr_::is_ExprNotBase:
         return const_eval_expr( pExpr->u.exprNotBase_.expr_ );
      case Expr_::is_ExprAdd:
      {
         ConstValue a = const_eval_expr( pExpr->u.exprAdd_.expr_1 );
         ConstValue b = const_eval_expr( pExpr->u.exprAdd_.expr_2 );
         if( !a.valid || !b.valid )
            return cv_invalid();
         return cv_int( cv_as_int( a ) + cv_as_int( b ) );
      }
      case Expr_::is_ExprSub:
      {
         ConstValue a = const_eval_expr( pExpr->u.exprSub_.expr_1 );
         ConstValue b = const_eval_expr( pExpr->u.exprSub_.expr_2 );
         if( !a.valid || !b.valid )
            return cv_invalid();
         return cv_int( cv_as_int( a ) - cv_as_int( b ) );
      }
      case Expr_::is_ExprMul:
      {
         ConstValue a = const_eval_expr( pExpr->u.exprMul_.expr_1 );
         ConstValue b = const_eval_expr( pExpr->u.exprMul_.expr_2 );
         if( !a.valid || !b.valid )
            return cv_invalid();
         return cv_int( cv_as_int( a ) * cv_as_int( b ) );
      }
      case Expr_::is_ExprDiv:
      {
         ConstValue a = const_eval_expr( pExpr->u.exprDiv_.expr_1 );
         ConstValue b = const_eval_expr( pExpr->u.exprDiv_.expr_2 );
         if( !a.valid || !b.valid )
            return cv_invalid();
         int64_t denom = cv_as_int( b );
         if( denom == 0 )
            return cv_invalid();
         return cv_int( cv_as_int( a ) / denom );
      }
      case Expr_::is_ExprMod:
      {
         ConstValue a = const_eval_expr( pExpr->u.exprMod_.expr_1 );
         ConstValue b = const_eval_expr( pExpr->u.exprMod_.expr_2 );
         if( !a.valid || !b.valid )
            return cv_invalid();
         int64_t denom = cv_as_int( b );
         if( denom == 0 )
            return cv_invalid();
         return cv_int( cv_as_int( a ) % denom );
      }
      case Expr_::is_ExprPlus:
         return const_eval_expr( pExpr->u.exprPlus_.expr_ );
      case Expr_::is_ExprMinus:
      {
         ConstValue a = const_eval_expr( pExpr->u.exprMinus_.expr_ );
         if( !a.valid )
            return cv_invalid();
         return cv_int( -cv_as_int( a ) );
      }
      case Expr_::is_ExprLit:
      {
         Literal pLit = pExpr->u.exprLit_.literal_;
         if( !pLit )
            return cv_invalid();
         switch( pLit->kind )
         {
         case Literal_::is_LitTrueDot:
            return cv_bool( true );
         case Literal_::is_LitFalseDot:
            return cv_bool( false );
         case Literal_::is_LitInt:
            return cv_int( std::stoll( pLit->u.litInt_.intlit_ ) );
         case Literal_::is_LitNil:
            return cv_int( 0 );
         default:
            return cv_invalid();
         }
      }
      case Expr_::is_ExprParen:
         return const_eval_expr( pExpr->u.exprParen_.expr_ );
      default:
         return cv_invalid();
      }
   }

   llvm::Constant *codegen_const_expr( Expr pExpr )
   {
      if( !pExpr )
         return nullptr;

      if( pExpr->kind == Expr_::is_ExprLit )
      {
         Literal pLit = pExpr->u.exprLit_.literal_;
         if( !pLit )
            return nullptr;
         switch( pLit->kind )
         {
         case Literal_::is_LitNil:
            return make_item_const( kDoItNil, const_i64( 0 ) );
         case Literal_::is_LitTrueDot:
            return make_item_const( kDoItLogical, const_i64( 1 ) );
         case Literal_::is_LitFalseDot:
            return make_item_const( kDoItLogical, const_i64( 0 ) );
         case Literal_::is_LitInt:
            return make_item_const( kDoItLong, const_i64( std::stoll( pLit->u.litInt_.intlit_ ) ) );
         case Literal_::is_LitStr:
         {
            const char *szRaw = get_napis_cstr( pLit->u.litStr_.napis_ );
            if( !szRaw )
               return nullptr;
            std::string     oText = decode_string_literal( szRaw );
            llvm::Constant *pBits = const_string_bits( oText.c_str() );
            if( !pBits )
               return nullptr;
            return make_item_const( kDoItString, pBits );
         }
         default:
            break;
         }
      }

      ConstValue oVal = const_eval_expr( pExpr );
      if( !oVal.valid )
         return nullptr;
      if( oVal.is_bool )
         return make_item_const( kDoItLogical, const_i64( oVal.b ? 1 : 0 ) );
      return make_item_const( kDoItLong, const_i64( oVal.i ) );
   }

   llvm::Value *codegen_expr_or( ExprOr pExpr )
   {
      if( !pExpr )
         return nullptr;

      switch( pExpr->kind )
      {
      case ExprOr_::is_ExprOrBase:
         return codegen_expr( pExpr->u.exprOrBase_.expr_ );
      case ExprOr_::is_ExprOrDot:
      {
         llvm::Value *pLeft      = codegen_expr( pExpr->u.exprOrDot_.expr_ );
         llvm::Value *pRight     = codegen_expr_or( pExpr->u.exprOrDot_.expror_ );
         llvm::Value *pLeftBool  = item_to_bool( pLeft );
         llvm::Value *pRightBool = item_to_bool( pRight );
         if( !pLeftBool || !pRightBool )
            return nullptr;
         llvm::Value *pOr = m_Builder.CreateOr( pLeftBool, pRightBool, "or" );
         return make_bool_item( pOr );
      }
      }

      return nullptr;
   }

   llvm::Value *codegen_expr( Expr pExpr )
   {
      if( !pExpr )
         return nullptr;

      switch( pExpr->kind )
      {
      case Expr_::is_ExprAssignBase:
         return codegen_expr_or( pExpr->u.exprAssignBase_.expror_ );
      case Expr_::is_ExprEq:
         return codegen_compare( pExpr->u.exprEq_.expr_1, pExpr->u.exprEq_.expr_2, CMP_EQ );
      case Expr_::is_ExprEq2:
         return codegen_compare( pExpr->u.exprEq2_.expr_1, pExpr->u.exprEq2_.expr_2, CMP_EQ );
      case Expr_::is_ExprNeq:
         return codegen_compare( pExpr->u.exprNeq_.expr_1, pExpr->u.exprNeq_.expr_2, CMP_NE );
      case Expr_::is_ExprNeq2:
         return codegen_compare( pExpr->u.exprNeq2_.expr_1, pExpr->u.exprNeq2_.expr_2, CMP_NE );
      case Expr_::is_ExprNeq3:
         return codegen_compare( pExpr->u.exprNeq3_.expr_1, pExpr->u.exprNeq3_.expr_2, CMP_NE );
      case Expr_::is_ExprLt:
         return codegen_compare( pExpr->u.exprLt_.expr_1, pExpr->u.exprLt_.expr_2, CMP_LT );
      case Expr_::is_ExprLe:
         return codegen_compare( pExpr->u.exprLe_.expr_1, pExpr->u.exprLe_.expr_2, CMP_LE );
      case Expr_::is_ExprGt:
         return codegen_compare( pExpr->u.exprGt_.expr_1, pExpr->u.exprGt_.expr_2, CMP_GT );
      case Expr_::is_ExprGe:
         return codegen_compare( pExpr->u.exprGe_.expr_1, pExpr->u.exprGe_.expr_2, CMP_GE );
      case Expr_::is_ExprAndDot:
      {
         llvm::Value *pLeft      = codegen_expr( pExpr->u.exprAndDot_.expr_1 );
         llvm::Value *pRight     = codegen_expr( pExpr->u.exprAndDot_.expr_2 );
         llvm::Value *pLeftBool  = item_to_bool( pLeft );
         llvm::Value *pRightBool = item_to_bool( pRight );
         if( !pLeftBool || !pRightBool )
            return nullptr;
         llvm::Value *pAnd = m_Builder.CreateAnd( pLeftBool, pRightBool, "and" );
         return make_bool_item( pAnd );
      }
      case Expr_::is_ExprAndBase:
         return codegen_expr( pExpr->u.exprAndBase_.expr_ );
      case Expr_::is_ExprNotDot:
      {
         llvm::Value *pVal  = codegen_expr( pExpr->u.exprNotDot_.expr_ );
         llvm::Value *pBool = item_to_bool( pVal );
         if( !pBool )
            return nullptr;
         llvm::Value *pNot = m_Builder.CreateNot( pBool, "not" );
         return make_bool_item( pNot );
      }
      case Expr_::is_ExprNotBase:
         return codegen_expr( pExpr->u.exprNotBase_.expr_ );
      case Expr_::is_ExprRelBase:
         return codegen_expr( pExpr->u.exprRelBase_.expr_ );
      case Expr_::is_ExprAdd:
      {
         std::optional<std::string> oLeftLit  = get_string_literal_text( pExpr->u.exprAdd_.expr_1 );
         std::optional<std::string> oRightLit = get_string_literal_text( pExpr->u.exprAdd_.expr_2 );
         if( oLeftLit && oRightLit )
         {
            std::string     oConcat = *oLeftLit + *oRightLit;
            llvm::Constant *pBits   = const_string_bits( oConcat.c_str() );
            if( !pBits )
               return nullptr;
            return make_item_const( kDoItString, pBits );
         }

         ConstValue oConst = const_eval_expr( pExpr );
         if( oConst.valid )
         {
            if( oConst.is_bool )
               return make_item_const( kDoItLogical, const_i64( oConst.b ? 1 : 0 ) );
            return make_item_const( kDoItLong, const_i64( oConst.i ) );
         }

         llvm::Value *pLeft  = codegen_expr( pExpr->u.exprAdd_.expr_1 );
         llvm::Value *pRight = codegen_expr( pExpr->u.exprAdd_.expr_2 );
         if( !pLeft || !pRight )
            return nullptr;

         llvm::Value *pLeftType  = item_type( pLeft );
         llvm::Value *pLeftKind  = item_kind( pLeft );
         llvm::Value *pRightKind = item_kind( pRight );
         if( !pLeftType || !pLeftKind || !pRightKind )
            return nullptr;

         llvm::Value *pIsStrLeft  = m_Builder.CreateICmpEQ( pLeftKind, const_i32( kDoItString ), "isstr.l" );
         llvm::Value *pIsStrRight = m_Builder.CreateICmpEQ( pRightKind, const_i32( kDoItString ), "isstr.r" );
         llvm::Value *pIsStrBoth  = m_Builder.CreateAnd( pIsStrLeft, pIsStrRight, "isstr.both" );

         llvm::Value *pIsNumLeft =
             m_Builder.CreateOr( m_Builder.CreateICmpEQ( pLeftKind, const_i32( kDoItInt32 ) ),
                                 m_Builder.CreateICmpEQ( pLeftKind, const_i32( kDoItLong ) ), "isnum.l" );
         llvm::Value *pIsNumRight =
             m_Builder.CreateOr( m_Builder.CreateICmpEQ( pRightKind, const_i32( kDoItInt32 ) ),
                                 m_Builder.CreateICmpEQ( pRightKind, const_i32( kDoItLong ) ), "isnum.r" );
         llvm::Value *pIsNumBoth = m_Builder.CreateAnd( pIsNumLeft, pIsNumRight, "isnum.both" );

         llvm::Type *pI8Ty    = llvm::Type::getInt8Ty( m_Context );
         llvm::Type *pI8PtrTy = llvm::PointerType::get( m_Context, 0 );

         llvm::FunctionCallee pStrlen = get_rt_func( "strlen", type_i64(), { pI8PtrTy } );
         llvm::FunctionCallee pMemcpy = get_rt_func( "memcpy", pI8PtrTy, { pI8PtrTy, pI8PtrTy, type_i64() } );
         llvm::FunctionCallee pXgrab  = get_rt_func( "do_xgrab", pI8PtrTy, { type_i64() } );
         llvm::FunctionCallee pErr =
             get_rt_func( "do_err_type", llvm::Type::getVoidTy( m_Context ), { pI8PtrTy, pI8PtrTy, type_i32() } );
         if( !pStrlen || !pMemcpy || !pXgrab || !pErr )
            return nullptr;

         llvm::Constant *pEmptyBits = const_string_bits( "" );
         if( !pEmptyBits )
            return nullptr;
         llvm::Constant *pEmptyPtr = llvm::ConstantExpr::getIntToPtr( pEmptyBits, pI8PtrTy );

         llvm::Function   *pFun    = m_CurrentFunction;
         llvm::BasicBlock *pStrBB  = llvm::BasicBlock::Create( m_Context, "add.str", pFun );
         llvm::BasicBlock *pChkBB  = llvm::BasicBlock::Create( m_Context, "add.chk", pFun );
         llvm::BasicBlock *pNumBB  = llvm::BasicBlock::Create( m_Context, "add.num", pFun );
         llvm::BasicBlock *pErrBB  = llvm::BasicBlock::Create( m_Context, "add.err", pFun );
         llvm::BasicBlock *pContBB = llvm::BasicBlock::Create( m_Context, "add.cont", pFun );

         m_Builder.CreateCondBr( pIsStrBoth, pStrBB, pChkBB );

         m_Builder.SetInsertPoint( pChkBB );
         m_Builder.CreateCondBr( pIsNumBoth, pNumBB, pErrBB );

         llvm::Value *pStrItem = nullptr;
         llvm::Value *pNumItem = nullptr;
         llvm::Value *pErrItem = nullptr;

         m_Builder.SetInsertPoint( pStrBB );
         llvm::Value *pLeftBits     = item_to_i64( pLeft );
         llvm::Value *pRightBits    = item_to_i64( pRight );
         llvm::Value *pLeftNull     = m_Builder.CreateICmpEQ( pLeftBits, const_i64( 0 ), "l.null" );
         llvm::Value *pRightNull    = m_Builder.CreateICmpEQ( pRightBits, const_i64( 0 ), "r.null" );
         llvm::Value *pLeftPtr      = m_Builder.CreateIntToPtr( pLeftBits, pI8PtrTy, "l.ptr" );
         llvm::Value *pRightPtr     = m_Builder.CreateIntToPtr( pRightBits, pI8PtrTy, "r.ptr" );
         llvm::Value *pLeftPtrSafe  = m_Builder.CreateSelect( pLeftNull, pEmptyPtr, pLeftPtr, "l.safe" );
         llvm::Value *pRightPtrSafe = m_Builder.CreateSelect( pRightNull, pEmptyPtr, pRightPtr, "r.safe" );
         llvm::Value *pLenLeft      = m_Builder.CreateCall( pStrlen, { pLeftPtrSafe }, "llen" );
         llvm::Value *pLenRight     = m_Builder.CreateCall( pStrlen, { pRightPtrSafe }, "rlen" );
         llvm::Value *pTotalLen     = m_Builder.CreateAdd( pLenLeft, pLenRight, "totlen" );
         llvm::Value *pAllocLen     = m_Builder.CreateAdd( pTotalLen, const_i64( 1 ), "alloclen" );
         llvm::Value *pDst          = m_Builder.CreateCall( pXgrab, { pAllocLen }, "dst" );
         m_Builder.CreateCall( pMemcpy, { pDst, pLeftPtrSafe, pLenLeft } );
         llvm::Value *pDst2         = m_Builder.CreateInBoundsGEP( pI8Ty, pDst, pLenLeft, "dst2" );
         llvm::Value *pRightCopyLen = m_Builder.CreateAdd( pLenRight, const_i64( 1 ), "rlen1" );
         m_Builder.CreateCall( pMemcpy, { pDst2, pRightPtrSafe, pRightCopyLen } );
         llvm::Value *pDstBits = m_Builder.CreatePtrToInt( pDst, type_i64(), "dstbits" );
         pStrItem              = make_item( const_i32( kDoItString ), pDstBits );
         m_Builder.CreateBr( pContBB );

         m_Builder.SetInsertPoint( pNumBB );
         llvm::Value *pLeftInt  = item_to_i64( pLeft );
         llvm::Value *pRightInt = item_to_i64( pRight );
         llvm::Value *pSum      = m_Builder.CreateAdd( pLeftInt, pRightInt, "add" );
         pNumItem               = make_int_item( pSum );
         m_Builder.CreateBr( pContBB );

         m_Builder.SetInsertPoint( pErrBB );
         llvm::Constant *pFuncNameBits = const_string_bits( "add" );
         llvm::Constant *pExpectedBits = const_string_bits( "string or numeric" );
         llvm::Value    *pFuncName     = llvm::ConstantExpr::getIntToPtr( pFuncNameBits, pI8PtrTy );
         llvm::Value    *pExpected     = llvm::ConstantExpr::getIntToPtr( pExpectedBits, pI8PtrTy );
         m_Builder.CreateCall( pErr, { pFuncName, pExpected, pLeftType } );
         pErrItem = make_item( const_i32( kDoItNil ), const_i64( 0 ) );
         m_Builder.CreateBr( pContBB );

         m_Builder.SetInsertPoint( pContBB );
         auto *pPhi = m_Builder.CreatePHI( type_do_item(), 3, "addtmp" );
         pPhi->addIncoming( pStrItem, pStrBB );
         pPhi->addIncoming( pNumItem, pNumBB );
         pPhi->addIncoming( pErrItem, pErrBB );
         return pPhi;
      }
      case Expr_::is_ExprAddBase:
         return codegen_expr( pExpr->u.exprAddBase_.expr_ );
      case Expr_::is_ExprMulBase:
         return codegen_expr( pExpr->u.exprMulBase_.expr_ );
      case Expr_::is_ExprPowBase:
         return codegen_expr( pExpr->u.exprPowBase_.expr_ );
      case Expr_::is_ExprAddrOf:
      {
         llvm::Value *pPtr = codegen_lhs_ptr( pExpr->u.exprAddrOf_.lhs_ );
         if( !pPtr )
            return nullptr;
         llvm::Value *pBits = m_Builder.CreatePtrToInt( pPtr, type_i64(), "pbits" );
         return make_item( const_i32( kDoItPointer | kDoFlagByRef ), pBits );
      }
      case Expr_::is_ExprDeref:
      {
         std::vector<Expr> aArgs;
         aArgs.push_back( pExpr->u.exprDeref_.expr_ );
         return emit_do_call( "peek", aArgs );
      }
      case Expr_::is_ExprSignBase:
         return codegen_expr( pExpr->u.exprSignBase_.expr_ );
      case Expr_::is_ExprPlus:
         return codegen_expr( pExpr->u.exprPlus_.expr_ );
      case Expr_::is_ExprMinus:
      {
         llvm::Value *pVal = codegen_expr( pExpr->u.exprMinus_.expr_ );
         if( !pVal )
            return nullptr;
         llvm::Value *pInt = item_to_i64( pVal );
         if( !pInt )
            return nullptr;
         llvm::Value *pNeg = m_Builder.CreateNeg( pInt, "neg" );
         return make_int_item( pNeg );
      }
      case Expr_::is_ExprLit:
         return codegen_literal( pExpr->u.exprLit_.literal_ );
      case Expr_::is_ExprCall:
         return codegen_call_expr( pExpr->u.exprCall_.callexpr_ );
      case Expr_::is_ExprParen:
         return codegen_expr( pExpr->u.exprParen_.expr_ );
      case Expr_::is_ExprMethodCall:
      {
         Expr  pObjExpr = pExpr->u.exprMethodCall_.expr_;
         Ident szMeth   = pExpr->u.exprMethodCall_.ident_;
         return codegen_enum_accessor( pObjExpr, szMeth );
      }
      case Expr_::is_ExprFieldAccess:
      {
         Expr  pObjExpr = pExpr->u.exprFieldAccess_.expr_;
         Ident szMeth   = pExpr->u.exprFieldAccess_.ident_;
         return codegen_enum_accessor( pObjExpr, szMeth );
      }
      case Expr_::is_ExprIndex:
      {
         std::vector<Expr> aIdxs = collect_subscripts( pExpr->u.exprIndex_.subscriptlist_ );
         if( aIdxs.empty() )
            return nullptr;

         llvm::Value *pBaseItem = codegen_expr( pExpr->u.exprIndex_.expr_ );
         if( !pBaseItem )
            return nullptr;

         for( size_t i = 0; i < aIdxs.size(); ++i )
         {
            llvm::Value *pBaseKind = item_kind( pBaseItem );
            if( !pBaseKind )
               return nullptr;
            llvm::Value *pIsArray = m_Builder.CreateICmpEQ( pBaseKind, const_i32( kDoItArray ), "isarray" );

            llvm::Function   *pFun    = m_CurrentFunction;
            llvm::BasicBlock *pOkBB   = llvm::BasicBlock::Create( m_Context, "idx.ok", pFun );
            llvm::BasicBlock *pErrBB  = llvm::BasicBlock::Create( m_Context, "idx.err", pFun );
            llvm::BasicBlock *pContBB = llvm::BasicBlock::Create( m_Context, "idx.cont", pFun );
            m_Builder.CreateCondBr( pIsArray, pOkBB, pErrBB );

            llvm::Value *pArrPtr = nullptr;

            m_Builder.SetInsertPoint( pOkBB );
            llvm::Value *pArrBits = item_to_i64( pBaseItem );
            if( !pArrBits )
               return nullptr;
            pArrPtr = m_Builder.CreateIntToPtr( pArrBits, llvm::PointerType::get( m_Context, 0 ), "arrptr" );
            m_Builder.CreateBr( pContBB );

            m_Builder.SetInsertPoint( pErrBB );
            llvm::Type          *pI8PtrTy = llvm::PointerType::get( m_Context, 0 );
            llvm::FunctionCallee pErr =
                get_rt_func( "do_err_type", llvm::Type::getVoidTy( m_Context ), { pI8PtrTy, pI8PtrTy, type_i32() } );
            llvm::Constant *pFuncNameBits = const_string_bits( "index" );
            llvm::Constant *pExpectedBits = const_string_bits( "array" );
            if( !pErr || !pFuncNameBits || !pExpectedBits )
               return nullptr;
            llvm::Value *pFuncName = llvm::ConstantExpr::getIntToPtr( pFuncNameBits, pI8PtrTy );
            llvm::Value *pExpected = llvm::ConstantExpr::getIntToPtr( pExpectedBits, pI8PtrTy );
            llvm::Value *pBaseType = item_type( pBaseItem );
            if( !pBaseType )
               return nullptr;
            m_Builder.CreateCall( pErr, { pFuncName, pExpected, pBaseType } );
            llvm::Value *pNilPtr = array_nil_ptr();
            if( !pNilPtr )
               return nullptr;
            m_Builder.CreateBr( pContBB );

            m_Builder.SetInsertPoint( pContBB );
            auto *pArrPhi = m_Builder.CreatePHI( llvm::PointerType::get( m_Context, 0 ), 2, "arrptrphi" );
            pArrPhi->addIncoming( pArrPtr, pOkBB );
            pArrPhi->addIncoming( pNilPtr, pErrBB );

            llvm::Value *pIdxItem = codegen_expr( aIdxs[ i ] );
            if( !pIdxItem )
               return nullptr;
            llvm::Value *pIdxVal = item_to_i64( pIdxItem );
            if( !pIdxVal )
               return nullptr;

            llvm::Value *pElemPtr = emit_array_at( pArrPhi, pIdxVal );
            if( !pElemPtr )
               return nullptr;

            if( i + 1 == aIdxs.size() )
               return m_Builder.CreateLoad( type_do_item(), pElemPtr, "arrval" );

            pBaseItem = m_Builder.CreateLoad( type_do_item(), pElemPtr, "arr.next" );
         }
         return nullptr;
      }
      case Expr_::is_ExprArray:
      {
         std::vector<Expr>    aItems     = collect_expr_list( pExpr->u.exprArray_.exprlistopt_ );
         llvm::Type          *pArrPtrTy  = llvm::PointerType::get( m_Context, 0 );
         llvm::Type          *pItemPtrTy = llvm::PointerType::get( m_Context, 0 );
         llvm::FunctionCallee pNew       = get_rt_func( "do_array_new", pArrPtrTy, { type_i64() } );
         llvm::FunctionCallee pAt        = get_rt_func( "do_array_at", pItemPtrTy, { pArrPtrTy, type_i64() } );
         if( !pNew || !pAt )
            return nullptr;

         llvm::Value *pArr = m_Builder.CreateCall( pNew, { const_i64( ( int64_t ) aItems.size() ) }, "arr" );
         for( size_t i = 0; i < aItems.size(); ++i )
         {
            llvm::Value *pItem = codegen_expr( aItems[ i ] );
            if( !pItem )
               return nullptr;
            llvm::Value *pPtr = m_Builder.CreateCall( pAt, { pArr, const_i64( ( int64_t ) i + 1 ) }, "arr.at" );
            m_Builder.CreateStore( pItem, pPtr );
         }

         llvm::Value *pBits = m_Builder.CreatePtrToInt( pArr, type_i64(), "arrbits" );
         return make_item( const_i32( kDoItArray ), pBits );
      }
      case Expr_::is_ExprQId:
      {
         QualifiedId pQ = pExpr->u.exprQId_.qualifiedid_;
         if( !pQ || pQ->kind != QualifiedId_::is_QualifiedIdSingle )
            return nullptr;
         const char *szName = pQ->u.qualifiedIdSingle_.ident_;
         auto        it     = m_Locals.find( szName ? szName : "" );
         if( it != m_Locals.end() )
         {
            llvm::AllocaInst *pAlloca = it->second;
            return m_Builder.CreateLoad( type_do_item(), pAlloca, szName );
         }
         auto itStatic = m_StaticLocals.find( szName ? szName : "" );
         if( itStatic != m_StaticLocals.end() )
         {
            llvm::GlobalVariable *pGlob = itStatic->second;
            return m_Builder.CreateLoad( type_do_item(), pGlob, szName );
         }
         auto itGlob = m_Globals.find( szName ? szName : "" );
         if( itGlob != m_Globals.end() )
         {
            llvm::GlobalVariable *pGlob = itGlob->second;
            return m_Builder.CreateLoad( type_do_item(), pGlob, szName );
         }
         return nullptr;
      }
      default:
         return nullptr;
      }
   }

   llvm::Value *codegen_if_cond( IfCond pCond )
   {
      if( !pCond || pCond->kind != IfCond_::is_IfCondExpr )
         return nullptr;
      llvm::Value *pVal = codegen_expr( pCond->u.ifCondExpr_.expr_ );
      if( !pVal )
         return nullptr;
      return item_to_bool( pVal );
   }

   llvm::Value *codegen_while_cond( WhileCond pCond )
   {
      if( !pCond || pCond->kind != WhileCond_::is_WhileCondExpr )
         return nullptr;
      llvm::Value *pVal = codegen_expr( pCond->u.whileCondExpr_.expr_ );
      if( !pVal )
         return nullptr;
      return item_to_bool( pVal );
   }

   llvm::Value *codegen_case_cond( Expr pExpr )
   {
      if( !pExpr )
         return nullptr;
      llvm::Value *pVal = codegen_expr( pExpr );
      if( !pVal )
         return nullptr;
      return item_to_bool( pVal );
   }

   bool codegen_if_stmt( Stmt pStmt )
   {
      if( !pStmt || pStmt->kind != Stmt_::is_StmtIf )
         return false;
      if( !m_CurrentFunction )
         return false;

      IfCond     pCond    = pStmt->u.stmtIf_.ifcond_;
      StmtList   pThen    = pStmt->u.stmtIf_.stmtlist_;
      ElseIfList pElseIfs = pStmt->u.stmtIf_.elseiflist_;
      ElseOpt    pElseOpt = pStmt->u.stmtIf_.elseopt_;

      llvm::Function   *pFun    = m_CurrentFunction;
      llvm::BasicBlock *pContBB = llvm::BasicBlock::Create( m_Context, "if.cont", pFun );

      llvm::Value *pCondVal = codegen_if_cond( pCond );
      if( !pCondVal )
         return false;

      llvm::BasicBlock *pThenBB = llvm::BasicBlock::Create( m_Context, "if.then", pFun );
      llvm::BasicBlock *pNextBB = llvm::BasicBlock::Create( m_Context, "if.next", pFun );
      m_Builder.CreateCondBr( pCondVal, pThenBB, pNextBB );

      m_Builder.SetInsertPoint( pThenBB );
      if( !codegen_stmtlist( pThen ) )
         return false;
      if( !pThenBB->getTerminator() )
         m_Builder.CreateBr( pContBB );

      m_Builder.SetInsertPoint( pNextBB );

      while( pElseIfs && pElseIfs->kind == ElseIfList_::is_ListElseIfCons )
      {
         ElseIf   pElseIf  = pElseIfs->u.listElseIfCons_.elseif_;
         IfCond   pElCond  = nullptr;
         StmtList pElStmts = nullptr;
         if( pElseIf && pElseIf->kind == ElseIf_::is_ElseIfClause )
         {
            pElCond  = pElseIf->u.elseIfClause_.ifcond_;
            pElStmts = pElseIf->u.elseIfClause_.stmtlist_;
         }
         llvm::Value *pElCondVal = codegen_if_cond( pElCond );
         if( !pElCondVal )
            return false;

         llvm::BasicBlock *pElThenBB = llvm::BasicBlock::Create( m_Context, "elseif.then", pFun );
         llvm::BasicBlock *pElNextBB = llvm::BasicBlock::Create( m_Context, "elseif.next", pFun );
         m_Builder.CreateCondBr( pElCondVal, pElThenBB, pElNextBB );

         m_Builder.SetInsertPoint( pElThenBB );
         if( !codegen_stmtlist( pElStmts ) )
            return false;
         if( !pElThenBB->getTerminator() )
            m_Builder.CreateBr( pContBB );

         m_Builder.SetInsertPoint( pElNextBB );
         pElseIfs = pElseIfs->u.listElseIfCons_.elseiflist_;
      }

      if( pElseOpt && pElseOpt->kind == ElseOpt_::is_OptElseSome )
      {
         if( !codegen_stmtlist( pElseOpt->u.optElseSome_.stmtlist_ ) )
            return false;
         if( !m_Builder.GetInsertBlock()->getTerminator() )
            m_Builder.CreateBr( pContBB );
      }
      else
      {
         if( !m_Builder.GetInsertBlock()->getTerminator() )
            m_Builder.CreateBr( pContBB );
      }

      m_Builder.SetInsertPoint( pContBB );
      return true;
   }

   bool codegen_do_while_stmt( Stmt pStmt )
   {
      if( !pStmt || pStmt->kind != Stmt_::is_StmtDoWhile )
         return false;
      if( !m_CurrentFunction )
         return false;

      WhileCond pCond = pStmt->u.stmtDoWhile_.whilecond_;
      StmtList  pBody = pStmt->u.stmtDoWhile_.stmtlist_;

      llvm::Function   *pFun    = m_CurrentFunction;
      llvm::BasicBlock *pCondBB = llvm::BasicBlock::Create( m_Context, "while.cond", pFun );
      llvm::BasicBlock *pBodyBB = llvm::BasicBlock::Create( m_Context, "while.body", pFun );
      llvm::BasicBlock *pExitBB = llvm::BasicBlock::Create( m_Context, "while.exit", pFun );

      m_Builder.CreateBr( pCondBB );

      m_Builder.SetInsertPoint( pCondBB );
      llvm::Value *pCondVal = codegen_while_cond( pCond );
      if( !pCondVal )
         return false;
      m_Builder.CreateCondBr( pCondVal, pBodyBB, pExitBB );

      m_Builder.SetInsertPoint( pBodyBB );
      m_BreakTargets.push_back( pExitBB );
      m_ContinueTargets.push_back( pCondBB );
      if( !codegen_stmtlist( pBody ) )
         return false;
      m_BreakTargets.pop_back();
      m_ContinueTargets.pop_back();

      llvm::BasicBlock *pCurBB = m_Builder.GetInsertBlock();
      if( pCurBB && !pCurBB->getTerminator() )
         m_Builder.CreateBr( pCondBB );

      m_Builder.SetInsertPoint( pExitBB );
      return true;
   }

   bool codegen_for_stmt( Stmt pStmt )
   {
      if( !pStmt || pStmt->kind != Stmt_::is_StmtFor )
         return false;
      if( !m_CurrentFunction )
         return false;

      const char *szName = pStmt->u.stmtFor_.ident_;
      Expr        pStart = pStmt->u.stmtFor_.expr_1;
      Expr        pEnd   = pStmt->u.stmtFor_.expr_2;
      StepOpt     pStep  = pStmt->u.stmtFor_.stepopt_;
      StmtList    pBody  = pStmt->u.stmtFor_.stmtlist_;

      llvm::Value *pStartItem = codegen_expr( pStart );
      if( !pStartItem )
         return false;

      llvm::AllocaInst *pCounterPtr = get_or_create_alloca( szName, type_do_item() );
      if( !pCounterPtr )
         return false;

      m_Builder.CreateStore( pStartItem, pCounterPtr );

      llvm::IRBuilder<> oTmp( &m_CurrentFunction->getEntryBlock(), m_CurrentFunction->getEntryBlock().begin() );
      llvm::AllocaInst *pStepAlloca = oTmp.CreateAlloca( type_i64(), nullptr, "for.step" );

      llvm::Function   *pFun    = m_CurrentFunction;
      llvm::BasicBlock *pCondBB = llvm::BasicBlock::Create( m_Context, "for.cond", pFun );
      llvm::BasicBlock *pEndBB  = llvm::BasicBlock::Create( m_Context, "for.endchk", pFun );
      llvm::BasicBlock *pStepBB = llvm::BasicBlock::Create( m_Context, "for.stepchk", pFun );
      llvm::BasicBlock *pTestBB = llvm::BasicBlock::Create( m_Context, "for.test", pFun );
      llvm::BasicBlock *pBodyBB = llvm::BasicBlock::Create( m_Context, "for.body", pFun );
      llvm::BasicBlock *pIncBB  = llvm::BasicBlock::Create( m_Context, "for.inc", pFun );
      llvm::BasicBlock *pExitBB = llvm::BasicBlock::Create( m_Context, "for.exit", pFun );

      llvm::Type          *pI8PtrTy = llvm::PointerType::get( m_Context, 0 );
      llvm::FunctionCallee pErr =
          get_rt_func( "do_err_type", llvm::Type::getVoidTy( m_Context ), { pI8PtrTy, pI8PtrTy, type_i32() } );
      llvm::Constant *pFuncNameBits = const_string_bits( "for" );
      llvm::Constant *pExpectedBits = const_string_bits( "numeric" );
      llvm::Value    *pFuncName     = llvm::ConstantExpr::getIntToPtr( pFuncNameBits, pI8PtrTy );
      llvm::Value    *pExpected     = llvm::ConstantExpr::getIntToPtr( pExpectedBits, pI8PtrTy );

      m_Builder.CreateBr( pCondBB );

      m_Builder.SetInsertPoint( pCondBB );
      llvm::Value *pCounterItem = m_Builder.CreateLoad( type_do_item(), pCounterPtr, szName );
      llvm::Value *pCounterKind = item_kind( pCounterItem );
      if( !pCounterKind || !pErr || !pFuncNameBits || !pExpectedBits )
         return false;
      llvm::Value *pIsCounterNum =
          m_Builder.CreateOr( m_Builder.CreateICmpEQ( pCounterKind, const_i32( kDoItInt32 ) ),
                              m_Builder.CreateICmpEQ( pCounterKind, const_i32( kDoItLong ) ), "for.isnum" );

      llvm::BasicBlock *pErrCounterBB = llvm::BasicBlock::Create( m_Context, "for.err.counter", pFun );
      m_Builder.CreateCondBr( pIsCounterNum, pEndBB, pErrCounterBB );

      m_Builder.SetInsertPoint( pErrCounterBB );
      llvm::Value *pCounterType = item_type( pCounterItem );
      if( !pCounterType )
         return false;
      m_Builder.CreateCall( pErr, { pFuncName, pExpected, pCounterType } );
      m_Builder.CreateBr( pExitBB );

      m_Builder.SetInsertPoint( pEndBB );
      llvm::Value *pEndItem = codegen_expr( pEnd );
      if( !pEndItem )
         return false;
      llvm::Value *pEndKind = item_kind( pEndItem );
      if( !pEndKind )
         return false;
      llvm::Value *pIsEndNum =
          m_Builder.CreateOr( m_Builder.CreateICmpEQ( pEndKind, const_i32( kDoItInt32 ) ),
                              m_Builder.CreateICmpEQ( pEndKind, const_i32( kDoItLong ) ), "for.isendnum" );

      llvm::BasicBlock *pErrEndBB = llvm::BasicBlock::Create( m_Context, "for.err.end", pFun );
      m_Builder.CreateCondBr( pIsEndNum, pStepBB, pErrEndBB );

      m_Builder.SetInsertPoint( pErrEndBB );
      llvm::Value *pEndType = item_type( pEndItem );
      if( !pEndType )
         return false;
      m_Builder.CreateCall( pErr, { pFuncName, pExpected, pEndType } );
      m_Builder.CreateBr( pExitBB );

      m_Builder.SetInsertPoint( pStepBB );
      if( pStep && pStep->kind == StepOpt_::is_OptStepSome )
      {
         llvm::Value *pStepItem = codegen_expr( pStep->u.optStepSome_.expr_ );
         if( !pStepItem )
            return false;
         llvm::Value *pStepKind = item_kind( pStepItem );
         if( !pStepKind )
            return false;
         llvm::Value *pIsStepNum =
             m_Builder.CreateOr( m_Builder.CreateICmpEQ( pStepKind, const_i32( kDoItInt32 ) ),
                                 m_Builder.CreateICmpEQ( pStepKind, const_i32( kDoItLong ) ), "for.isstepnum" );

         llvm::BasicBlock *pErrStepBB   = llvm::BasicBlock::Create( m_Context, "for.err.step", pFun );
         llvm::BasicBlock *pStoreStepBB = llvm::BasicBlock::Create( m_Context, "for.step", pFun );
         m_Builder.CreateCondBr( pIsStepNum, pStoreStepBB, pErrStepBB );

         m_Builder.SetInsertPoint( pErrStepBB );
         llvm::Value *pStepType = item_type( pStepItem );
         if( !pStepType )
            return false;
         m_Builder.CreateCall( pErr, { pFuncName, pExpected, pStepType } );
         m_Builder.CreateBr( pExitBB );

         m_Builder.SetInsertPoint( pStoreStepBB );
         llvm::Value *pStepVal = item_to_i64( pStepItem );
         if( !pStepVal )
            return false;
         m_Builder.CreateStore( pStepVal, pStepAlloca );
         m_Builder.CreateBr( pTestBB );
      }
      else
      {
         m_Builder.CreateStore( const_i64( 1 ), pStepAlloca );
         m_Builder.CreateBr( pTestBB );
      }

      m_Builder.SetInsertPoint( pTestBB );
      llvm::Value *pCounterNow = m_Builder.CreateLoad( type_do_item(), pCounterPtr, szName );
      llvm::Value *pCounterVal = item_to_i64( pCounterNow );
      llvm::Value *pEndVal     = item_to_i64( pEndItem );
      llvm::Value *pStepVal    = m_Builder.CreateLoad( type_i64(), pStepAlloca, "step" );
      if( !pCounterVal || !pEndVal || !pStepVal )
         return false;

      llvm::Value *pIsNeg  = m_Builder.CreateICmpSLT( pStepVal, const_i64( 0 ), "for.neg" );
      llvm::Value *pCondLe = m_Builder.CreateICmpSLE( pCounterVal, pEndVal, "for.le" );
      llvm::Value *pCondGe = m_Builder.CreateICmpSGE( pCounterVal, pEndVal, "for.ge" );
      llvm::Value *pCond   = m_Builder.CreateSelect( pIsNeg, pCondGe, pCondLe, "for.cond" );
      m_Builder.CreateCondBr( pCond, pBodyBB, pExitBB );

      m_Builder.SetInsertPoint( pBodyBB );
      m_BreakTargets.push_back( pExitBB );
      m_ContinueTargets.push_back( pIncBB );
      if( !codegen_stmtlist( pBody ) )
         return false;
      m_BreakTargets.pop_back();
      m_ContinueTargets.pop_back();

      llvm::BasicBlock *pCurBB = m_Builder.GetInsertBlock();
      if( pCurBB && !pCurBB->getTerminator() )
         m_Builder.CreateBr( pIncBB );

      m_Builder.SetInsertPoint( pIncBB );
      llvm::Value *pCounterIncItem = m_Builder.CreateLoad( type_do_item(), pCounterPtr, szName );
      llvm::Value *pCounterIncVal  = item_to_i64( pCounterIncItem );
      llvm::Value *pStepIncVal     = m_Builder.CreateLoad( type_i64(), pStepAlloca, "step" );
      if( !pCounterIncVal || !pStepIncVal )
         return false;
      llvm::Value *pNextVal = m_Builder.CreateAdd( pCounterIncVal, pStepIncVal, "for.next" );
      m_Builder.CreateStore( make_int_item( pNextVal ), pCounterPtr );
      m_Builder.CreateBr( pCondBB );

      m_Builder.SetInsertPoint( pExitBB );
      return true;
   }

   bool codegen_for_each_stmt( Stmt pStmt )
   {
      if( !pStmt || pStmt->kind != Stmt_::is_StmtForEach )
         return false;
      if( !m_CurrentFunction )
         return false;

      std::vector<Ident> aVars  = collect_foreach_vars( pStmt->u.stmtForEach_.foreachvars_ );
      std::vector<Expr>  aExprs = collect_foreach_exprs( pStmt->u.stmtForEach_.foreachexprs_ );
      if( aVars.empty() || aExprs.empty() )
         return false;
      if( aVars.size() != aExprs.size() )
         return false;

      const bool lDescend = pStmt->u.stmtForEach_.foreachdescopt_ &&
                            pStmt->u.stmtForEach_.foreachdescopt_->kind == ForEachDescOpt_::is_ForEachDescOptSome;

      const size_t nCount = aVars.size();

      std::vector<llvm::Value *> aArrPtrs;
      std::vector<llvm::Value *> aLens;
      std::vector<llvm::AllocaInst *> aEnumPtrs;
      std::vector<std::pair<std::string, llvm::AllocaInst *>> aPrevEnums;
      aArrPtrs.reserve( nCount );
      aLens.reserve( nCount );
      aEnumPtrs.reserve( nCount );
      aPrevEnums.reserve( nCount );

      llvm::PointerType *pArrPtrTy = llvm::PointerType::get( m_Context, 0 );
      llvm::FunctionCallee pLen = get_rt_func( "do_array_len", type_i64(), { pArrPtrTy } );
      llvm::FunctionCallee pEnumNew = get_rt_func( "do_enum_new_array", pArrPtrTy, { pArrPtrTy, type_i32() } );
      llvm::FunctionCallee pEnumFree = get_rt_func( "do_enum_free", llvm::Type::getVoidTy( m_Context ), { pArrPtrTy } );
      llvm::FunctionCallee pEnumSet = get_rt_func( "do_enum_set_index", llvm::Type::getVoidTy( m_Context ),
                                                   { pArrPtrTy, type_i64() } );
      if( !pLen )
         return false;
      if( !pEnumNew || !pEnumFree || !pEnumSet )
         return false;

      llvm::IRBuilder<> oTmp( &m_CurrentFunction->getEntryBlock(), m_CurrentFunction->getEntryBlock().begin() );

      for( size_t i = 0; i < nCount; ++i )
      {
         llvm::Value *pItem = codegen_expr( aExprs[ i ] );
         if( !pItem )
            return false;
         llvm::Value *pKind = item_kind( pItem );
         if( !pKind )
            return false;

         llvm::Value *pIsArray = m_Builder.CreateICmpEQ( pKind, const_i32( kDoItArray ), "isarray" );
         llvm::Function   *pFun    = m_CurrentFunction;
         llvm::BasicBlock *pOkBB   = llvm::BasicBlock::Create( m_Context, "fe.ok", pFun );
         llvm::BasicBlock *pErrBB  = llvm::BasicBlock::Create( m_Context, "fe.err", pFun );
         llvm::BasicBlock *pContBB = llvm::BasicBlock::Create( m_Context, "fe.cont", pFun );
         m_Builder.CreateCondBr( pIsArray, pOkBB, pErrBB );

         llvm::Value *pArrPtr = nullptr;

         m_Builder.SetInsertPoint( pOkBB );
         llvm::Value *pBits = item_to_i64( pItem );
         if( !pBits )
            return false;
         pArrPtr = m_Builder.CreateIntToPtr( pBits, pArrPtrTy, "arrptr" );
         m_Builder.CreateBr( pContBB );

         m_Builder.SetInsertPoint( pErrBB );
         llvm::Type          *pI8PtrTy = llvm::PointerType::get( m_Context, 0 );
         llvm::FunctionCallee pErr =
             get_rt_func( "do_err_type", llvm::Type::getVoidTy( m_Context ), { pI8PtrTy, pI8PtrTy, type_i32() } );
         llvm::Constant *pFuncNameBits = const_string_bits( "for each" );
         llvm::Constant *pExpectedBits = const_string_bits( "array" );
         if( !pErr || !pFuncNameBits || !pExpectedBits )
            return false;
         llvm::Value *pFuncName = llvm::ConstantExpr::getIntToPtr( pFuncNameBits, pI8PtrTy );
         llvm::Value *pExpected = llvm::ConstantExpr::getIntToPtr( pExpectedBits, pI8PtrTy );
         llvm::Value *pTypeVal  = item_type( pItem );
         if( !pTypeVal )
            return false;
         m_Builder.CreateCall( pErr, { pFuncName, pExpected, pTypeVal } );
         llvm::Value *pNilPtr = llvm::ConstantPointerNull::get( pArrPtrTy );
         m_Builder.CreateBr( pContBB );

         m_Builder.SetInsertPoint( pContBB );
         auto *pArrPhi = m_Builder.CreatePHI( pArrPtrTy, 2, "arrptrphi" );
         pArrPhi->addIncoming( pArrPtr, pOkBB );
         pArrPhi->addIncoming( pNilPtr, pErrBB );

         llvm::Value *pLenVal = m_Builder.CreateCall( pLen, { pArrPhi }, "arr.len" );
         aArrPtrs.push_back( pArrPhi );
         aLens.push_back( pLenVal );

         llvm::AllocaInst *pEnumAlloca = oTmp.CreateAlloca( pArrPtrTy, nullptr, "foreach.enum" );
         llvm::Value *pEnumPtr = m_Builder.CreateCall( pEnumNew, { pArrPhi, const_i32( lDescend ? 1 : 0 ) }, "enum" );
         m_Builder.CreateStore( pEnumPtr, pEnumAlloca );
         aEnumPtrs.push_back( pEnumAlloca );

         std::string oName = aVars[ i ] ? aVars[ i ] : "";
         auto it = m_EnumLocals.find( oName );
         aPrevEnums.push_back( { oName, it != m_EnumLocals.end() ? it->second : nullptr } );
         m_EnumLocals[ oName ] = pEnumAlloca;
      }

      llvm::Value *pMinLen = aLens[ 0 ];
      for( size_t i = 1; i < nCount; ++i )
      {
         llvm::Value *pIsLt = m_Builder.CreateICmpSLT( aLens[ i ], pMinLen, "len.lt" );
         pMinLen            = m_Builder.CreateSelect( pIsLt, aLens[ i ], pMinLen, "len.min" );
      }

      llvm::AllocaInst *pIdxAlloca = oTmp.CreateAlloca( type_i64(), nullptr, "foreach.idx" );
      llvm::Value      *pStartIdx  = lDescend ? pMinLen : const_i64( 1 );
      llvm::Value      *pEndIdx    = lDescend ? const_i64( 1 ) : pMinLen;
      llvm::Value      *pStepVal   = lDescend ? const_i64( -1 ) : const_i64( 1 );
      m_Builder.CreateStore( pStartIdx, pIdxAlloca );

      std::vector<llvm::AllocaInst *> aSaved;
      aSaved.reserve( nCount );
      for( size_t i = 0; i < nCount; ++i )
      {
         llvm::AllocaInst *pVar = get_or_create_alloca( aVars[ i ], type_do_item() );
         if( !pVar )
            return false;
         llvm::AllocaInst *pSave = oTmp.CreateAlloca( type_do_item(), nullptr, "foreach.save" );
         llvm::Value      *pOld  = m_Builder.CreateLoad( type_do_item(), pVar );
         m_Builder.CreateStore( pOld, pSave );
         aSaved.push_back( pSave );
      }

      llvm::Function   *pFun    = m_CurrentFunction;
      llvm::BasicBlock *pCondBB = llvm::BasicBlock::Create( m_Context, "foreach.cond", pFun );
      llvm::BasicBlock *pBodyBB = llvm::BasicBlock::Create( m_Context, "foreach.body", pFun );
      llvm::BasicBlock *pIncBB  = llvm::BasicBlock::Create( m_Context, "foreach.inc", pFun );
      llvm::BasicBlock *pExitBB = llvm::BasicBlock::Create( m_Context, "foreach.exit", pFun );

      m_Builder.CreateBr( pCondBB );

      m_Builder.SetInsertPoint( pCondBB );
      llvm::Value *pIdxCur = m_Builder.CreateLoad( type_i64(), pIdxAlloca, "idx" );
      llvm::Value *pCond   = lDescend ? m_Builder.CreateICmpSGE( pIdxCur, pEndIdx, "idx.ge" )
                                      : m_Builder.CreateICmpSLE( pIdxCur, pEndIdx, "idx.le" );
      m_Builder.CreateCondBr( pCond, pBodyBB, pExitBB );

      m_Builder.SetInsertPoint( pBodyBB );
      for( size_t i = 0; i < nCount; ++i )
      {
         llvm::Value *pEnumPtr = m_Builder.CreateLoad( pArrPtrTy, aEnumPtrs[ i ], "enum" );
         m_Builder.CreateCall( pEnumSet, { pEnumPtr, pIdxCur } );
      }
      for( size_t i = 0; i < nCount; ++i )
      {
         llvm::Value *pElemPtr = emit_array_at( aArrPtrs[ i ], pIdxCur );
         if( !pElemPtr )
            return false;
         llvm::Value *pVal = m_Builder.CreateLoad( type_do_item(), pElemPtr, "elem" );
         llvm::AllocaInst *pVar = get_or_create_alloca( aVars[ i ], type_do_item() );
         if( !pVar )
            return false;
         m_Builder.CreateStore( pVal, pVar );
      }

      m_BreakTargets.push_back( pExitBB );
      m_ContinueTargets.push_back( pIncBB );
      if( !codegen_stmtlist( pStmt->u.stmtForEach_.stmtlist_ ) )
         return false;
      m_BreakTargets.pop_back();
      m_ContinueTargets.pop_back();

      llvm::BasicBlock *pCurBB = m_Builder.GetInsertBlock();
      if( pCurBB && !pCurBB->getTerminator() )
         m_Builder.CreateBr( pIncBB );

      m_Builder.SetInsertPoint( pIncBB );
      for( size_t i = 0; i < nCount; ++i )
      {
         llvm::Value *pElemPtr = emit_array_at( aArrPtrs[ i ], pIdxCur );
         if( !pElemPtr )
            return false;
         llvm::AllocaInst *pVar = get_or_create_alloca( aVars[ i ], type_do_item() );
         if( !pVar )
            return false;
         llvm::Value *pVal = m_Builder.CreateLoad( type_do_item(), pVar );
         m_Builder.CreateStore( pVal, pElemPtr );
      }

      llvm::Value *pNextIdx = m_Builder.CreateAdd( pIdxCur, pStepVal, "idx.next" );
      m_Builder.CreateStore( pNextIdx, pIdxAlloca );
      m_Builder.CreateBr( pCondBB );

      m_Builder.SetInsertPoint( pExitBB );
      for( size_t i = 0; i < nCount; ++i )
      {
         llvm::AllocaInst *pVar = get_or_create_alloca( aVars[ i ], type_do_item() );
         llvm::Value      *pOld = m_Builder.CreateLoad( type_do_item(), aSaved[ i ] );
         m_Builder.CreateStore( pOld, pVar );
      }
      for( size_t i = 0; i < nCount; ++i )
      {
         llvm::Value *pEnumPtr = m_Builder.CreateLoad( pArrPtrTy, aEnumPtrs[ i ], "enum" );
         m_Builder.CreateCall( pEnumFree, { pEnumPtr } );
      }
      for( const auto &oPrev : aPrevEnums )
      {
         if( oPrev.second )
            m_EnumLocals[ oPrev.first ] = oPrev.second;
         else
            m_EnumLocals.erase( oPrev.first );
      }
      return true;
   }

   bool codegen_do_case_stmt( Stmt pStmt )
   {
      if( !pStmt || pStmt->kind != Stmt_::is_StmtDoCase )
         return false;
      if( !m_CurrentFunction )
         return false;

      CaseCondList pCases = pStmt->u.stmtDoCase_.casecondlist_;
      OtherwiseOpt pOther = pStmt->u.stmtDoCase_.otherwiseopt_;

      llvm::Function   *pFun    = m_CurrentFunction;
      llvm::BasicBlock *pContBB = llvm::BasicBlock::Create( m_Context, "case.cont", pFun );

      m_BreakTargets.push_back( pContBB );

      llvm::BasicBlock *pCheckBB = m_Builder.GetInsertBlock();
      if( !pCheckBB )
         pCheckBB = llvm::BasicBlock::Create( m_Context, "case.check", pFun );

      while( pCases && pCases->kind == CaseCondList_::is_ListCaseCondCons )
      {
         CaseCondClause pClause = pCases->u.listCaseCondCons_.casecondclause_;
         Expr           pExpr   = nullptr;
         StmtList       pStmts  = nullptr;
         if( pClause && pClause->kind == CaseCondClause_::is_CaseCondClauseItem )
         {
            pExpr  = pClause->u.caseCondClauseItem_.expr_;
            pStmts = pClause->u.caseCondClauseItem_.stmtlist_;
         }

         m_Builder.SetInsertPoint( pCheckBB );
         llvm::Value *pCondVal = codegen_case_cond( pExpr );
         if( !pCondVal )
         {
            m_BreakTargets.pop_back();
            return false;
         }

         llvm::BasicBlock *pThenBB = llvm::BasicBlock::Create( m_Context, "case.then", pFun );
         llvm::BasicBlock *pNextBB = llvm::BasicBlock::Create( m_Context, "case.next", pFun );
         m_Builder.CreateCondBr( pCondVal, pThenBB, pNextBB );

         m_Builder.SetInsertPoint( pThenBB );
         if( !codegen_stmtlist( pStmts ) )
         {
            m_BreakTargets.pop_back();
            return false;
         }
         if( !pThenBB->getTerminator() )
            m_Builder.CreateBr( pContBB );

         pCheckBB = pNextBB;
         pCases   = pCases->u.listCaseCondCons_.casecondlist_;
      }

      m_Builder.SetInsertPoint( pCheckBB );
      if( pOther && pOther->kind == OtherwiseOpt_::is_OptOtherwiseSome )
      {
         if( !codegen_stmtlist( pOther->u.optOtherwiseSome_.stmtlist_ ) )
         {
            m_BreakTargets.pop_back();
            return false;
         }
         if( !m_Builder.GetInsertBlock()->getTerminator() )
            m_Builder.CreateBr( pContBB );
      }
      else
      {
         if( !m_Builder.GetInsertBlock()->getTerminator() )
            m_Builder.CreateBr( pContBB );
      }

      m_BreakTargets.pop_back();
      m_Builder.SetInsertPoint( pContBB );
      return true;
   }

   bool codegen_switch_stmt( Stmt pStmt )
   {
      if( !pStmt || pStmt->kind != Stmt_::is_StmtSwitch )
         return false;
      if( !m_CurrentFunction )
         return false;

      SwitchCond   pCond  = pStmt->u.stmtSwitch_.switchcond_;
      CaseList     pCases = pStmt->u.stmtSwitch_.caselist_;
      OtherwiseOpt pOther = pStmt->u.stmtSwitch_.otherwiseopt_;

      if( !pCond || pCond->kind != SwitchCond_::is_SwitchCondExpr )
         return false;

      llvm::Value *pSwitchItem = codegen_expr( pCond->u.switchCondExpr_.expr_ );
      if( !pSwitchItem )
         return false;

      llvm::IRBuilder<> oTmp( &m_CurrentFunction->getEntryBlock(), m_CurrentFunction->getEntryBlock().begin() );
      llvm::AllocaInst *pSwitchAlloca = oTmp.CreateAlloca( type_do_item(), nullptr, "switch.val" );
      m_Builder.CreateStore( pSwitchItem, pSwitchAlloca );

      llvm::Function   *pFun     = m_CurrentFunction;
      llvm::BasicBlock *pTypeBB  = llvm::BasicBlock::Create( m_Context, "switch.type", pFun );
      llvm::BasicBlock *pExitBB  = llvm::BasicBlock::Create( m_Context, "switch.exit", pFun );
      llvm::BasicBlock *pOtherBB = pOther && pOther->kind == OtherwiseOpt_::is_OptOtherwiseSome
                                       ? llvm::BasicBlock::Create( m_Context, "switch.other", pFun )
                                       : nullptr;

      std::vector<CaseClause>         aCases;
      std::vector<llvm::BasicBlock *> aCaseBBs;
      std::vector<llvm::BasicBlock *> aCmpBBs;

      while( pCases && pCases->kind == CaseList_::is_ListCaseCons )
      {
         CaseClause pClause = pCases->u.listCaseCons_.caseclause_;
         aCases.push_back( pClause );
         aCaseBBs.push_back( llvm::BasicBlock::Create( m_Context, "switch.case", pFun ) );
         aCmpBBs.push_back( llvm::BasicBlock::Create( m_Context, "switch.cmp", pFun ) );
         pCases = pCases->u.listCaseCons_.caselist_;
      }

      m_Builder.CreateBr( pTypeBB );

      m_Builder.SetInsertPoint( pTypeBB );
      llvm::Value *pSwitchNow  = m_Builder.CreateLoad( type_do_item(), pSwitchAlloca, "switch" );
      llvm::Value *pSwitchKind = item_kind( pSwitchNow );
      if( !pSwitchKind )
         return false;

      llvm::Value *pIsNum =
          m_Builder.CreateOr( m_Builder.CreateICmpEQ( pSwitchKind, const_i32( kDoItInt32 ) ),
                              m_Builder.CreateICmpEQ( pSwitchKind, const_i32( kDoItLong ) ), "switch.isnum" );
      llvm::Value *pIsStr = m_Builder.CreateICmpEQ( pSwitchKind, const_i32( kDoItString ), "switch.isstr" );
      llvm::Value *pIsOk  = m_Builder.CreateOr( pIsNum, pIsStr, "switch.isok" );

      llvm::Type          *pI8PtrTy = llvm::PointerType::get( m_Context, 0 );
      llvm::FunctionCallee pErr =
          get_rt_func( "do_err_type", llvm::Type::getVoidTy( m_Context ), { pI8PtrTy, pI8PtrTy, type_i32() } );
      llvm::Constant *pFuncNameBits = const_string_bits( "switch" );
      llvm::Constant *pExpectedBits = const_string_bits( "string or numeric" );
      if( !pErr || !pFuncNameBits || !pExpectedBits )
         return false;
      llvm::Value *pFuncName = llvm::ConstantExpr::getIntToPtr( pFuncNameBits, pI8PtrTy );
      llvm::Value *pExpected = llvm::ConstantExpr::getIntToPtr( pExpectedBits, pI8PtrTy );

      llvm::BasicBlock *pErrBB      = llvm::BasicBlock::Create( m_Context, "switch.err", pFun );
      llvm::BasicBlock *pCmpStartBB = aCmpBBs.empty() ? ( pOtherBB ? pOtherBB : pExitBB ) : aCmpBBs.front();
      m_Builder.CreateCondBr( pIsOk, pCmpStartBB, pErrBB );

      m_Builder.SetInsertPoint( pErrBB );
      llvm::Value *pSwitchType = item_type( pSwitchNow );
      if( !pSwitchType )
         return false;
      m_Builder.CreateCall( pErr, { pFuncName, pExpected, pSwitchType } );
      m_Builder.CreateBr( pExitBB );

      llvm::FunctionCallee pStrcmp = get_rt_func( "strcmp", type_i32(), { pI8PtrTy, pI8PtrTy } );
      if( !pStrcmp )
         return false;
      llvm::Constant *pEmptyBits = const_string_bits( "" );
      if( !pEmptyBits )
         return false;
      llvm::Constant *pEmptyPtr = llvm::ConstantExpr::getIntToPtr( pEmptyBits, pI8PtrTy );

      for( size_t i = 0; i < aCases.size(); ++i )
      {
         CaseClause pClause = aCases[ i ];
         if( !pClause || pClause->kind != CaseClause_::is_CaseClauseItem )
            return false;

         CaseValue pVal = pClause->u.caseClauseItem_.casevalue_;
         if( !pVal )
            return false;

         m_Builder.SetInsertPoint( aCmpBBs[ i ] );

         llvm::Value *pSwitchCur   = m_Builder.CreateLoad( type_do_item(), pSwitchAlloca, "switch" );
         llvm::Value *pSwitchKind2 = item_kind( pSwitchCur );
         if( !pSwitchKind2 )
            return false;

         llvm::Value *pMatch = nullptr;
         if( pVal->kind == CaseValue_::is_CaseValueInt )
         {
            int64_t      nCase = std::stoll( pVal->u.caseValueInt_.intlit_ );
            llvm::Value *pIsNum2 =
                m_Builder.CreateOr( m_Builder.CreateICmpEQ( pSwitchKind2, const_i32( kDoItInt32 ) ),
                                    m_Builder.CreateICmpEQ( pSwitchKind2, const_i32( kDoItLong ) ), "switch.num" );
            llvm::Value *pSwitchInt = item_to_i64( pSwitchCur );
            if( !pSwitchInt )
               return false;
            llvm::Value *pEq = m_Builder.CreateICmpEQ( pSwitchInt, const_i64( nCase ), "switch.eq" );
            pMatch           = m_Builder.CreateAnd( pIsNum2, pEq, "switch.match" );
         }
         else if( pVal->kind == CaseValue_::is_CaseValueStr )
         {
            const char *szRaw = get_napis_cstr( pVal->u.caseValueStr_.napis_ );
            if( !szRaw )
               return false;
            std::string     oText = decode_string_literal( szRaw );
            llvm::Constant *pBits = const_string_bits( oText.c_str() );
            if( !pBits )
               return false;
            llvm::Value *pCasePtr = llvm::ConstantExpr::getIntToPtr( pBits, pI8PtrTy );

            llvm::Value *pIsStr2     = m_Builder.CreateICmpEQ( pSwitchKind2, const_i32( kDoItString ), "switch.str" );
            llvm::Value *pSwitchBits = item_to_i64( pSwitchCur );
            if( !pSwitchBits )
               return false;
            llvm::Value *pSwitchNull = m_Builder.CreateICmpEQ( pSwitchBits, const_i64( 0 ), "switch.null" );
            llvm::Value *pSwitchPtr  = m_Builder.CreateIntToPtr( pSwitchBits, pI8PtrTy, "switch.ptr" );
            llvm::Value *pSwitchSafe = m_Builder.CreateSelect( pSwitchNull, pEmptyPtr, pSwitchPtr, "switch.safe" );
            llvm::Value *pCmp        = m_Builder.CreateCall( pStrcmp, { pSwitchSafe, pCasePtr }, "strcmp" );
            llvm::Value *pEq         = m_Builder.CreateICmpEQ( pCmp, const_i32( 0 ), "switch.eq" );
            pMatch                   = m_Builder.CreateAnd( pIsStr2, pEq, "switch.match" );
         }
         else
         {
            return false;
         }

         llvm::BasicBlock *pNextCmp = ( i + 1 < aCmpBBs.size() ) ? aCmpBBs[ i + 1 ] : ( pOtherBB ? pOtherBB : pExitBB );
         m_Builder.CreateCondBr( pMatch, aCaseBBs[ i ], pNextCmp );
      }

      m_BreakTargets.push_back( pExitBB );
      for( size_t i = 0; i < aCases.size(); ++i )
      {
         CaseClause pClause = aCases[ i ];
         StmtList   pStmts  = pClause->u.caseClauseItem_.stmtlist_;

         m_Builder.SetInsertPoint( aCaseBBs[ i ] );
         if( !codegen_stmtlist( pStmts ) )
         {
            m_BreakTargets.pop_back();
            return false;
         }

         if( !m_Builder.GetInsertBlock()->getTerminator() )
         {
            if( i + 1 < aCaseBBs.size() )
               m_Builder.CreateBr( aCaseBBs[ i + 1 ] );
            else
               m_Builder.CreateBr( pExitBB );
         }
      }
      m_BreakTargets.pop_back();

      if( pOtherBB )
      {
         m_Builder.SetInsertPoint( pOtherBB );
         if( !codegen_stmtlist( pOther->u.optOtherwiseSome_.stmtlist_ ) )
            return false;
         if( !m_Builder.GetInsertBlock()->getTerminator() )
            m_Builder.CreateBr( pExitBB );
      }

      m_Builder.SetInsertPoint( pExitBB );
      return true;
   }

   bool codegen_stmt( Stmt pStmt )
   {
      if( !pStmt )
         return true;

      switch( pStmt->kind )
      {
      case Stmt_::is_StmtAssignInl:
      {
         LHS pLhs = pStmt->u.stmtAssignInl_.lhs_;
         if( pLhs && pLhs->kind == LHS_::is_LHSDeref )
         {
            std::vector<Expr> aArgs;
            aArgs.push_back( pLhs->u.lHSDeref_.expr_ );
            aArgs.push_back( pStmt->u.stmtAssignInl_.expr_ );
            return emit_do_call( "poke", aArgs ) != nullptr;
         }

         llvm::Value *pVal = codegen_expr( pStmt->u.stmtAssignInl_.expr_ );
         if( !pVal )
            return false;
         llvm::Value *pPtr = codegen_lhs_ptr( pLhs );
         if( !pPtr )
            return false;
         m_Builder.CreateStore( pVal, pPtr );
         return true;
      }
      case Stmt_::is_StmtCall:
      {
         CallExpr pCall = pStmt->u.stmtCall_.callexpr_;
         return codegen_call_expr( pCall ) != nullptr;
      }
      case Stmt_::is_StmtIf:
         return codegen_if_stmt( pStmt );
      case Stmt_::is_StmtDoWhile:
         return codegen_do_while_stmt( pStmt );
      case Stmt_::is_StmtFor:
         return codegen_for_stmt( pStmt );
      case Stmt_::is_StmtForEach:
         return codegen_for_each_stmt( pStmt );
      case Stmt_::is_StmtSwitch:
         return codegen_switch_stmt( pStmt );
      case Stmt_::is_StmtDoCase:
         return codegen_do_case_stmt( pStmt );
      case Stmt_::is_StmtExit:
      {
         if( m_BreakTargets.empty() )
            return false;
         m_Builder.CreateBr( m_BreakTargets.back() );
         return true;
      }
      case Stmt_::is_StmtLoop:
      {
         if( m_ContinueTargets.empty() )
            return false;
         m_Builder.CreateBr( m_ContinueTargets.back() );
         return true;
      }
      case Stmt_::is_StmtReturn:
      {
         ReturnExprOpt pRetOpt = pStmt->u.stmtReturn_.returnexpropt_;
         llvm::Value  *pVal    = nullptr;
         if( !pRetOpt || pRetOpt->kind == ReturnExprOpt_::is_ReturnExprOptEmpty )
         {
            pVal = make_item_const( kDoItNil, const_i64( 0 ) );
         }
         else
         {
            pVal = codegen_expr( pRetOpt->u.returnExprOptSome_.expr_ );
            if( !pVal )
               return false;
         }
         llvm::Type *pRet = m_CurrentFunction ? m_CurrentFunction->getReturnType() : type_do_item();
         if( pRet == type_do_item() )
         {
            m_Builder.CreateRet( pVal );
            return true;
         }
         if( pRet->isIntegerTy( 32 ) )
         {
            llvm::Value *pInt = item_to_i64( pVal );
            if( !pInt )
               return false;
            llvm::Value *pI32 = m_Builder.CreateIntCast( pInt, type_i32(), true, "ret32" );
            m_Builder.CreateRet( pI32 );
            return true;
         }
         return false;
      }
      default:
         return false;
      }
   }

   bool codegen_stmtlist( StmtList pList )
   {
      if( !pList )
         return true;
      if( m_Builder.GetInsertBlock() && m_Builder.GetInsertBlock()->getTerminator() )
         return true;
      switch( pList->kind )
      {
      case StmtList_::is_ListStmtEmpty:
         return true;
      case StmtList_::is_ListStmtCons:
         if( !codegen_stmt( pList->u.listStmtCons_.stmt_ ) )
            return false;
         if( m_Builder.GetInsertBlock() && m_Builder.GetInsertBlock()->getTerminator() )
            return true;
         return codegen_stmtlist( pList->u.listStmtCons_.stmtlist_ );
      case StmtList_::is_ListStmtLine:
         return codegen_stmtlist( pList->u.listStmtLine_.stmtlist_ );
      }
      return false;
   }

   bool codegen_stmtlist_nonempty( StmtListNonEmpty pList )
   {
      if( !pList )
         return true;
      if( pList->kind == StmtListNonEmpty_::is_ListStmtNonEmpty )
      {
         if( !codegen_stmt( pList->u.listStmtNonEmpty_.stmt_ ) )
            return false;
         if( m_Builder.GetInsertBlock() && m_Builder.GetInsertBlock()->getTerminator() )
            return true;
         return codegen_stmtlist( pList->u.listStmtNonEmpty_.stmtlist_ );
      }
      return false;
   }

   bool codegen_identinit_list_alloc( IdentInitList pList )
   {
      if( !pList || pList->kind != IdentInitList_::is_ListIdentInitSingle )
         return true;

      auto alloc_one = [ this ]( IdentInit pItem )
      {
         if( !pItem || pItem->kind != IdentInit_::is_IdentInitItem )
            return true;
         const char       *szName  = pItem->u.identInitItem_.ident_;
         llvm::AllocaInst *pAlloca = get_or_create_alloca( szName, type_do_item() );
         if( !pAlloca )
            return false;
         m_Builder.CreateStore( make_item_const( kDoItNil, const_i64( 0 ) ), pAlloca );
         return true;
      };

      if( !alloc_one( pList->u.listIdentInitSingle_.identinit_ ) )
         return false;

      IdentInitTail pTail = pList->u.listIdentInitSingle_.identinittail_;
      while( pTail && pTail->kind == IdentInitTail_::is_ListIdentInitTailCons )
      {
         if( !alloc_one( pTail->u.listIdentInitTailCons_.identinit_ ) )
            return false;
         pTail = pTail->u.listIdentInitTailCons_.identinittail_;
      }

      return true;
   }

   bool codegen_identinit_list_init( IdentInitList pList )
   {
      if( !pList || pList->kind != IdentInitList_::is_ListIdentInitSingle )
         return true;

      auto init_one = [ this ]( IdentInit pItem )
      {
         if( !pItem || pItem->kind != IdentInit_::is_IdentInitItem )
            return true;

         InitOpt pInit = pItem->u.identInitItem_.initopt_;
         if( !pInit || pInit->kind != InitOpt_::is_OptInitSome )
            return true;

         const char       *szName  = pItem->u.identInitItem_.ident_;
         llvm::AllocaInst *pAlloca = get_or_create_alloca( szName, type_do_item() );
         if( !pAlloca )
            return false;

         llvm::Value *pVal = codegen_expr( pInit->u.optInitSome_.expr_ );
         if( !pVal )
            return false;

         m_Builder.CreateStore( pVal, pAlloca );
         return true;
      };

      if( !init_one( pList->u.listIdentInitSingle_.identinit_ ) )
         return false;

      IdentInitTail pTail = pList->u.listIdentInitSingle_.identinittail_;
      while( pTail && pTail->kind == IdentInitTail_::is_ListIdentInitTailCons )
      {
         if( !init_one( pTail->u.listIdentInitTailCons_.identinit_ ) )
            return false;
         pTail = pTail->u.listIdentInitTailCons_.identinittail_;
      }

      return true;
   }

   bool codegen_local_decls_alloc( LocalDeclsOpt pDecls )
   {
      while( pDecls && pDecls->kind != LocalDeclsOpt_::is_ListLocalDeclEmpty )
      {
         if( pDecls->kind == LocalDeclsOpt_::is_ListLocalDeclLine )
         {
            pDecls = pDecls->u.listLocalDeclLine_.localdeclsopt_;
            continue;
         }
         if( pDecls->kind == LocalDeclsOpt_::is_ListLocalDeclCons )
         {
            LocalDecl pDecl = pDecls->u.listLocalDeclCons_.localdecl_;
            if( pDecl && pDecl->kind == LocalDecl_::is_LocalDeclItem )
            {
               if( !codegen_identinit_list_alloc( pDecl->u.localDeclItem_.identinitlist_ ) )
                  return false;
            }
            pDecls = pDecls->u.listLocalDeclCons_.localdeclsopt_;
            continue;
         }
         break;
      }
      return true;
   }

   bool codegen_local_decls_init( LocalDeclsOpt pDecls )
   {
      while( pDecls && pDecls->kind != LocalDeclsOpt_::is_ListLocalDeclEmpty )
      {
         if( pDecls->kind == LocalDeclsOpt_::is_ListLocalDeclLine )
         {
            pDecls = pDecls->u.listLocalDeclLine_.localdeclsopt_;
            continue;
         }
         if( pDecls->kind == LocalDeclsOpt_::is_ListLocalDeclCons )
         {
            LocalDecl pDecl = pDecls->u.listLocalDeclCons_.localdecl_;
            if( pDecl && pDecl->kind == LocalDecl_::is_LocalDeclItem )
            {
               if( !codegen_identinit_list_init( pDecl->u.localDeclItem_.identinitlist_ ) )
                  return false;
            }
            pDecls = pDecls->u.listLocalDeclCons_.localdeclsopt_;
            continue;
         }
         break;
      }
      return true;
   }

   bool codegen_local_decls( LocalDeclsOpt pDecls )
   {
      return codegen_local_decls_alloc( pDecls ) && codegen_local_decls_init( pDecls );
   }

   bool codegen_function_def( FunctionDef pFun )
   {
      if( !pFun || pFun->kind != FunctionDef_::is_FunctionDefMain )
         return false;

      const char *szName     = pFun->u.functionDefMain_.ident_;
      const char *szLlvmName = szName;
      llvm::Type *pRetType   = type_do_item();
      if( is_main_name( szName ) )
      {
         szLlvmName = "main";
         pRetType   = type_i32();
      }
      llvm::FunctionType *pFT      = llvm::FunctionType::get( pRetType, std::vector<llvm::Type *>{}, false );
      llvm::Function     *pLlvmFun = m_Module->getFunction( szLlvmName );
      if( pLlvmFun )
      {
         if( pLlvmFun->getFunctionType() != pFT || !pLlvmFun->empty() )
            return false;
         pLlvmFun->setLinkage( llvm::Function::ExternalLinkage );
      }
      else
      {
         pLlvmFun = llvm::Function::Create( pFT, llvm::Function::ExternalLinkage, szLlvmName, m_Module.get() );
      }
      m_CurrentFunction = pLlvmFun;

      llvm::BasicBlock *pEntry = llvm::BasicBlock::Create( m_Context, "entry", m_CurrentFunction );
      m_Builder.SetInsertPoint( pEntry );
      m_Locals.clear();
      m_LocalTypes.clear();
      m_StaticLocals.clear();

      if( !codegen_static_decls( pFun->u.functionDefMain_.staticdeclsopt_, szName ) )
         return false;
      if( !codegen_local_decls( pFun->u.functionDefMain_.localdeclsopt_ ) )
         return false;

      if( !codegen_stmtlist_nonempty( pFun->u.functionDefMain_.stmtlistnonempty_ ) )
         return false;

      if( !pEntry->getTerminator() )
      {
         if( pRetType == type_do_item() )
            m_Builder.CreateRet( make_item_const( kDoItNil, const_i64( 0 ) ) );
         else
            m_Builder.CreateRet( llvm::ConstantInt::get( pRetType, 0 ) );
      }

      return !llvm::verifyFunction( *m_CurrentFunction, &llvm::errs() );
   }

   bool codegen_procedure_def( ProcedureDef pProc )
   {
      if( !pProc || pProc->kind != ProcedureDef_::is_ProcedureDefMain )
         return false;

      const char *szName     = pProc->u.procedureDefMain_.ident_;
      const char *szLlvmName = szName;
      llvm::Type *pRetType   = type_do_item();
      if( is_main_name( szName ) )
      {
         szLlvmName = "main";
         pRetType   = type_i32();
      }
      llvm::FunctionType *pFT      = llvm::FunctionType::get( pRetType, std::vector<llvm::Type *>{}, false );
      llvm::Function     *pLlvmFun = m_Module->getFunction( szLlvmName );
      if( pLlvmFun )
      {
         if( pLlvmFun->getFunctionType() != pFT || !pLlvmFun->empty() )
            return false;
         pLlvmFun->setLinkage( llvm::Function::ExternalLinkage );
      }
      else
      {
         pLlvmFun = llvm::Function::Create( pFT, llvm::Function::ExternalLinkage, szLlvmName, m_Module.get() );
      }
      m_CurrentFunction = pLlvmFun;

      llvm::BasicBlock *pEntry = llvm::BasicBlock::Create( m_Context, "entry", m_CurrentFunction );
      m_Builder.SetInsertPoint( pEntry );
      m_Locals.clear();
      m_LocalTypes.clear();
      m_StaticLocals.clear();

      if( !codegen_static_decls( pProc->u.procedureDefMain_.staticdeclsopt_, szName ) )
         return false;
      if( !codegen_local_decls( pProc->u.procedureDefMain_.localdeclsopt_ ) )
         return false;

      if( !codegen_stmtlist_nonempty( pProc->u.procedureDefMain_.stmtlistnonempty_ ) )
         return false;

      if( !pEntry->getTerminator() )
      {
         if( pRetType == type_do_item() )
            m_Builder.CreateRet( make_item_const( kDoItNil, const_i64( 0 ) ) );
         else
            m_Builder.CreateRet( llvm::ConstantInt::get( pRetType, 0 ) );
      }

      return !llvm::verifyFunction( *m_CurrentFunction, &llvm::errs() );
   }

   bool codegen_emit_static_init_once( llvm::GlobalVariable *pValue, llvm::Constant *pInit, const std::string &oName )
   {
      if( !pValue || !pInit || !m_CurrentFunction )
         return false;

      llvm::GlobalVariable *pFlag =
          new llvm::GlobalVariable( *m_Module, type_i1(), false, llvm::GlobalValue::InternalLinkage,
                                    llvm::ConstantInt::get( type_i1(), 0 ), oName + "_init" );

      llvm::BasicBlock *pDo   = llvm::BasicBlock::Create( m_Context, "static.init", m_CurrentFunction );
      llvm::BasicBlock *pCont = llvm::BasicBlock::Create( m_Context, "static.cont", m_CurrentFunction );

      llvm::Value *pLoaded = m_Builder.CreateLoad( type_i1(), pFlag );
      llvm::Value *pCond   = m_Builder.CreateICmpEQ( pLoaded, llvm::ConstantInt::get( type_i1(), 0 ) );
      m_Builder.CreateCondBr( pCond, pDo, pCont );

      m_Builder.SetInsertPoint( pDo );
      m_Builder.CreateStore( pInit, pValue );
      m_Builder.CreateStore( llvm::ConstantInt::get( type_i1(), 1 ), pFlag );
      m_Builder.CreateBr( pCont );

      m_Builder.SetInsertPoint( pCont );
      return true;
   }

   bool codegen_static_item( StaticItem pItem, const char *szFuncName )
   {
      if( !pItem || pItem->kind != StaticItem_::is_StaticItemMain )
         return true;

      const char        *szName = pItem->u.staticItemMain_.ident_;
      StaticArrayDimsOpt pDims  = pItem->u.staticItemMain_.staticarraydimsopt_;
      StaticInitOpt      pInit  = pItem->u.staticItemMain_.staticinitopt_;

      if( pDims && pDims->kind != StaticArrayDimsOpt_::is_StaticArrayDimsEmpty )
      {
         fprintf( stderr, "error: static arrays are not supported in codegen yet\n" );
         return false;
      }

      llvm::Constant *pInitConst = nullptr;
      if( pInit && pInit->kind == StaticInitOpt_::is_StaticInitSome )
         pInitConst = codegen_const_expr( pInit->u.staticInitSome_.expr_ );

      llvm::Type     *pType    = type_do_item();
      llvm::Constant *pInitVal = pInitConst ? pInitConst : make_item_const( kDoItNil, const_i64( 0 ) );

      std::string oBase       = szName ? szName : "";
      std::string oGlobalName = oBase;
      if( szFuncName && *szFuncName )
         oGlobalName = std::string( "__static_" ) + szFuncName + "_" + oBase;
      else
         oGlobalName = std::string( "__static_" ) + oBase;

      llvm::GlobalVariable *pGlob = new llvm::GlobalVariable(
          *m_Module, pType, false, llvm::GlobalValue::InternalLinkage, pInitVal, oGlobalName );

      if( szFuncName && *szFuncName )
      {
         m_StaticLocals[ oBase ] = pGlob;
         if( pInitConst )
            return codegen_emit_static_init_once( pGlob, pInitConst, oGlobalName );
         return true;
      }

      m_Globals[ oBase ] = pGlob;
      return true;
   }

   bool codegen_static_item_list( StaticItemList pList, const char *szFuncName )
   {
      if( !pList )
         return true;
      if( pList->kind == StaticItemList_::is_StaticItemListSingle )
      {
         if( !codegen_static_item( pList->u.staticItemListSingle_.staticitem_, szFuncName ) )
            return false;
         StaticItemTail pTail = pList->u.staticItemListSingle_.staticitemtail_;
         while( pTail && pTail->kind == StaticItemTail_::is_StaticItemTailCons )
         {
            if( !codegen_static_item( pTail->u.staticItemTailCons_.staticitem_, szFuncName ) )
               return false;
            pTail = pTail->u.staticItemTailCons_.staticitemtail_;
         }
      }
      return true;
   }

   bool codegen_static_decl( StaticDecl pDecl, const char *szFuncName )
   {
      if( !pDecl || pDecl->kind != StaticDecl_::is_StaticDeclItem )
         return true;
      return codegen_static_item_list( pDecl->u.staticDeclItem_.staticitemlist_, szFuncName );
   }

   bool codegen_static_decls( StaticDeclsOpt pDecls, const char *szFuncName )
   {
      if( !pDecls || pDecls->kind == StaticDeclsOpt_::is_ListStaticDeclEmpty )
         return true;
      if( pDecls->kind == StaticDeclsOpt_::is_ListStaticDeclCons )
      {
         if( !codegen_static_decl( pDecls->u.listStaticDeclCons_.staticdecl_, szFuncName ) )
            return false;
         return codegen_static_decls( pDecls->u.listStaticDeclCons_.staticdeclsopt_, szFuncName );
      }
      return true;
   }
};

CodeGenerator::CodeGenerator( const std::string &oModuleName ) : m_Impl( new Impl( oModuleName ) )
{
}

CodeGenerator::~CodeGenerator()
{
   delete m_Impl;
}

bool CodeGenerator::generate( Program pRoot )
{
   if( !pRoot || pRoot->kind != Program_::is_ProgramRoot )
      return false;

   m_Impl->collect_internal_funcs( pRoot->u.programRoot_.defs_ );

   TopDecls pTopDecls = pRoot->u.programRoot_.topdecls_;
   while( pTopDecls )
   {
      if( pTopDecls->kind == TopDecls_::is_ListTopDeclCons )
      {
         TopDecl pTop = pTopDecls->u.listTopDeclCons_.topdecl_;
         if( pTop && pTop->kind == TopDecl_::is_TopDeclStatic )
         {
            if( !m_Impl->codegen_static_decl( pTop->u.topDeclStatic_.staticdecl_, nullptr ) )
               return false;
         }
         pTopDecls = pTopDecls->u.listTopDeclCons_.topdecls_;
         continue;
      }
      if( pTopDecls->kind == TopDecls_::is_ListTopDeclLine )
      {
         pTopDecls = pTopDecls->u.listTopDeclLine_.topdecls_;
         continue;
      }
      break;
   }

   Defs pDefs = pRoot->u.programRoot_.defs_;
   bool lOk   = true;
   while( pDefs && lOk )
   {
      if( pDefs->kind == Defs_::is_ListDefSingle )
      {
         Def pDef = pDefs->u.listDefSingle_.def_;
         if( pDef )
         {
            if( pDef->kind == Def_::is_DefFun )
               lOk = m_Impl->codegen_function_def( pDef->u.defFun_.functiondef_ );
            else if( pDef->kind == Def_::is_DefProc )
               lOk = m_Impl->codegen_procedure_def( pDef->u.defProc_.proceduredef_ );
         }
         break;
      }
      if( pDefs->kind == Defs_::is_ListDefCons )
      {
         Def pDef = pDefs->u.listDefCons_.def_;
         if( pDef )
         {
            if( pDef->kind == Def_::is_DefFun )
               lOk = m_Impl->codegen_function_def( pDef->u.defFun_.functiondef_ );
            else if( pDef->kind == Def_::is_DefProc )
               lOk = m_Impl->codegen_procedure_def( pDef->u.defProc_.proceduredef_ );
         }
         pDefs = pDefs->u.listDefCons_.defs_;
         continue;
      }
      break;
   }

   return lOk;
}

bool CodeGenerator::writeIRToFile( const std::string &oPath ) const
{
   if( !m_Impl || !m_Impl->m_Module )
      return false;

   std::error_code      oErr;
   llvm::raw_fd_ostream oStream( oPath, oErr, llvm::sys::fs::OF_None );
   if( oErr )
      return false;

   m_Impl->m_Module->print( oStream, nullptr );
   return true;
}

bool CodeGenerator::writeObjectToFile( const std::string &oPath ) const
{
   if( !m_Impl || !m_Impl->m_Module )
      return false;

   llvm::InitializeNativeTarget();
   llvm::InitializeNativeTargetAsmPrinter();
   llvm::InitializeNativeTargetAsmParser();

   std::string         oErr;
   std::string         oTripleStr = LLVM_DEFAULT_TARGET_TRIPLE;
   const llvm::Target *pTarget    = llvm::TargetRegistry::lookupTarget( oTripleStr, oErr );
   if( !pTarget )
      return false;

   llvm::TargetOptions                  oOptions;
   llvm::Triple                         oTriple( oTripleStr );
   std::unique_ptr<llvm::TargetMachine> pMachine( pTarget->createTargetMachine(
       oTriple, "generic", "", oOptions, std::nullopt, std::nullopt, llvm::CodeGenOptLevel::Default, false ) );
   if( !pMachine )
      return false;

   m_Impl->m_Module->setTargetTriple( oTriple );
   m_Impl->m_Module->setDataLayout( pMachine->createDataLayout() );

   std::error_code      oEc;
   llvm::raw_fd_ostream oDest( oPath, oEc, llvm::sys::fs::OF_None );
   if( oEc )
      return false;

   llvm::legacy::PassManager oPass;
   if( pMachine->addPassesToEmitFile( oPass, oDest, nullptr, llvm::CodeGenFileType::ObjectFile ) )
      return false;

   oPass.run( *m_Impl->m_Module );
   oDest.flush();
   return true;
}

std::string CodeGenerator::getIRString() const
{
   if( !m_Impl || !m_Impl->m_Module )
      return std::string();

   std::string              oOut;
   llvm::raw_string_ostream oStream( oOut );
   m_Impl->m_Module->print( oStream, nullptr );
   return oStream.str();
}

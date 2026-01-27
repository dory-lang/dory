/*
 * sema.cpp - Minimal semantic stubs
 *
 * Checks parameter duplicates, static initializers, and main presence.
 * Reports semantic errors (line number is a stub for now).
 */

#include "sema.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unordered_set>
#include <vector>

struct _SymbolTable
{
   int dummy;
};

SymbolTable *sema_CreateSymbolTable()
{
   return ( SymbolTable * ) calloc( 1, sizeof( SymbolTable ) );
}

void sema_DestroySymbolTable( SymbolTable *pTable )
{
   free( pTable );
}

static bool sema_is_main_ident( const char *szName )
{
   if( !szName )
      return false;

   return ( ( szName[ 0 ] == 'm' || szName[ 0 ] == 'M' ) && ( szName[ 1 ] == 'a' || szName[ 1 ] == 'A' ) &&
            ( szName[ 2 ] == 'i' || szName[ 2 ] == 'I' ) && ( szName[ 3 ] == 'n' || szName[ 3 ] == 'N' ) &&
            szName[ 4 ] == '\0' );
}

static int sema_get_def_line( Def pDef );

static bool                             sema_is_const_expr( Expr pExpr );
static bool                             sema_is_const_expr_or( ExprOr pExpr );
static bool                             sema_is_const_expr_list( ExprListOpt pList );
static bool                             sema_is_const_expr_listtail( ExprListTail pTail );
static bool                             sema_is_const_subscript_list( SubscriptList pList );
static bool                             sema_is_const_subscript_tail( SubscriptListTail pTail );
static bool                             sema_check_static_dims( StaticArrayDimsOpt pDims );
static bool                             sema_check_static_decl( StaticDecl pDecl );
static bool                             sema_check_static_decls( StaticDeclsOpt pDecls );
static bool                             sema_check_topdecls( TopDecls pTopDecls );
typedef std::unordered_set<std::string> NameSet;
static const char                      *sema_get_qualifiedid_base( QualifiedId pQ );
static void                             sema_collect_param_names( ParamListOpt pOpt, NameSet &oNames );
static void                             sema_collect_local_names( LocalDeclsOpt pDecls, NameSet &oNames );
static void                             sema_collect_static_names( StaticDeclsOpt pDecls, NameSet &oNames );
static bool                             sema_check_stmtlist_nonempty( StmtListNonEmpty pList, const NameSet &oNames );
static bool                             sema_check_stmtlist( StmtList pList, const NameSet &oNames );
static bool                             sema_check_stmt( Stmt pStmt, const NameSet &oNames );
static bool                             sema_check_expr_declared( Expr pExpr, const NameSet &oNames );
static bool                             sema_check_expr_or_declared( ExprOr pExpr, const NameSet &oNames );
static bool                             sema_check_lhs_declared( LHS pLhs, const NameSet &oNames );
static bool                             sema_check_lvalue_declared( LValue pLVal, const NameSet &oNames );
static bool                             sema_check_arg_list( ArgListOpt pArgs, const NameSet &oNames );
static bool                             sema_check_expr_list( ExprListOpt pList, const NameSet &oNames );
static bool                             sema_check_expr_list_tail( ExprListTail pTail, const NameSet &oNames );
static bool                             sema_check_subscript_list( SubscriptList pList, const NameSet &oNames );
static bool                             sema_check_subscript_tail( SubscriptListTail pTail, const NameSet &oNames );
static bool                             sema_check_field_init_list( FieldInitList pList, const NameSet &oNames );
static bool                             sema_check_call_expr( CallExpr pCall, const NameSet &oNames );
static bool                             sema_check_if_cond( IfCond pCond, const NameSet &oNames );
static bool                             sema_check_while_cond( WhileCond pCond, const NameSet &oNames );
static bool                             sema_check_foreach_vars( ForEachVars pVars, const NameSet &oNames );
static bool                             sema_check_foreach_vars_tail( ForEachVarsTail pTail, const NameSet &oNames );
static bool                             sema_check_foreach_exprs( ForEachExprs pExprs, const NameSet &oNames );
static bool                             sema_check_foreach_exprs_tail( ForEachExprsTail pTail, const NameSet &oNames );
static bool                             sema_check_case_list( CaseList pList, const NameSet &oNames );
static bool                             sema_check_case_clause( CaseClause pClause, const NameSet &oNames );
static bool                             sema_check_case_cond_list( CaseCondList pList, const NameSet &oNames );
static bool                             sema_check_case_cond_clause( CaseCondClause pClause, const NameSet &oNames );
static bool                             sema_check_else_opt( ElseOpt pOpt, const NameSet &oNames );
static bool                             sema_check_otherwise_opt( OtherwiseOpt pOpt, const NameSet &oNames );
static bool                             sema_check_recover_opt( RecoverOpt pOpt, const NameSet &oNames );
static bool                             sema_check_break_expr_opt( BreakExprOpt pOpt, const NameSet &oNames );
static bool                             sema_check_return_expr_opt( ReturnExprOpt pOpt, const NameSet &oNames );
static bool                             sema_check_step_opt( StepOpt pOpt, const NameSet &oNames );
static bool                             sema_check_switch_cond( SwitchCond pCond, const NameSet &oNames );

static bool sema_has_duplicate_params( ParamListOpt pOpt, const char **pszDup )
{
   if( !pOpt || pOpt->kind == ParamListOpt_::is_OptParamListEmpty )
      return false;

   std::vector<const char *> seen;
   const char               *szFirst = pOpt->u.optParamListSome_.ident_;
   if( szFirst )
      seen.push_back( szFirst );

   ParamListTail pTail = pOpt->u.optParamListSome_.paramlisttail_;
   while( pTail && pTail->kind == ParamListTail_::is_ListParamTailCons )
   {
      const char *szParam = pTail->u.listParamTailCons_.ident_;
      if( szParam )
      {
         for( const char *szSeen : seen )
         {
            if( szSeen && strcmp( szSeen, szParam ) == 0 )
            {
               if( pszDup )
                  *pszDup = szParam;
               return true;
            }
         }
         seen.push_back( szParam );
      }
      pTail = pTail->u.listParamTailCons_.paramlisttail_;
   }

   return false;
}

static bool sema_check_def_params( Def pDef )
{
   if( !pDef )
      return true;

   ParamListOpt pOpt = NULL;
   switch( pDef->kind )
   {
   case Def_::is_DefFun:
      pOpt = pDef->u.defFun_.functiondef_->u.functionDefMain_.paramlistopt_;
      break;
   case Def_::is_DefProc:
      pOpt = pDef->u.defProc_.proceduredef_->u.procedureDefMain_.paramlistopt_;
      break;
   case Def_::is_DefMeth:
      pOpt = pDef->u.defMeth_.methoddef_->u.methodDefMain_.paramlistopt_;
      break;
   }

   const char *szDup = NULL;
   if( sema_has_duplicate_params( pOpt, &szDup ) )
   {
      char szMessage[ 256 ];
      snprintf( szMessage, sizeof( szMessage ), "Zduplikowany parametr: %s", szDup ? szDup : "" );
      sema_ReportError( szMessage, sema_get_def_line( pDef ) );
      return false;
   }

   return true;
}

static bool sema_is_const_expr_or( ExprOr pExpr )
{
   if( !pExpr )
      return true;

   switch( pExpr->kind )
   {
   case ExprOr_::is_ExprOrBase:
      return sema_is_const_expr( pExpr->u.exprOrBase_.expr_ );
   case ExprOr_::is_ExprOrDot:
      return sema_is_const_expr( pExpr->u.exprOrDot_.expr_ ) && sema_is_const_expr_or( pExpr->u.exprOrDot_.expror_ );
   }

   return false;
}

static bool sema_is_const_expr_list( ExprListOpt pList )
{
   if( !pList || pList->kind == ExprListOpt_::is_OptExprListEmpty )
      return true;

   return sema_is_const_expr( pList->u.optExprListSome_.expr_ ) &&
          sema_is_const_expr_listtail( pList->u.optExprListSome_.exprlisttail_ );
}

static bool sema_is_const_expr_listtail( ExprListTail pTail )
{
   while( pTail && pTail->kind == ExprListTail_::is_ListExprTailCons )
   {
      if( !sema_is_const_expr( pTail->u.listExprTailCons_.expr_ ) )
         return false;
      pTail = pTail->u.listExprTailCons_.exprlisttail_;
   }
   return true;
}

static bool sema_is_const_subscript_list( SubscriptList pList )
{
   if( !pList )
      return true;
   if( !sema_is_const_expr( pList->u.subscriptListSingle_.expr_ ) )
      return false;
   return sema_is_const_subscript_tail( pList->u.subscriptListSingle_.subscriptlisttail_ );
}

static bool sema_is_const_subscript_tail( SubscriptListTail pTail )
{
   while( pTail && pTail->kind == SubscriptListTail_::is_ListSubscriptTailCons )
   {
      if( !sema_is_const_expr( pTail->u.listSubscriptTailCons_.expr_ ) )
         return false;
      pTail = pTail->u.listSubscriptTailCons_.subscriptlisttail_;
   }
   return true;
}

static bool sema_is_const_expr( Expr pExpr )
{
   if( !pExpr )
      return true;

   switch( pExpr->kind )
   {
   case Expr_::is_ExprAssignBase:
      return sema_is_const_expr_or( pExpr->u.exprAssignBase_.expror_ );
   case Expr_::is_ExprAndDot:
      return sema_is_const_expr( pExpr->u.exprAndDot_.expr_1 ) && sema_is_const_expr( pExpr->u.exprAndDot_.expr_2 );
   case Expr_::is_ExprAndBase:
      return sema_is_const_expr( pExpr->u.exprAndBase_.expr_ );
   case Expr_::is_ExprNotDot:
      return sema_is_const_expr( pExpr->u.exprNotDot_.expr_ );
   case Expr_::is_ExprNotBase:
      return sema_is_const_expr( pExpr->u.exprNotBase_.expr_ );
   case Expr_::is_ExprEq:
      return sema_is_const_expr( pExpr->u.exprEq_.expr_1 ) && sema_is_const_expr( pExpr->u.exprEq_.expr_2 );
   case Expr_::is_ExprEq2:
      return sema_is_const_expr( pExpr->u.exprEq2_.expr_1 ) && sema_is_const_expr( pExpr->u.exprEq2_.expr_2 );
   case Expr_::is_ExprNeq:
      return sema_is_const_expr( pExpr->u.exprNeq_.expr_1 ) && sema_is_const_expr( pExpr->u.exprNeq_.expr_2 );
   case Expr_::is_ExprNeq2:
      return sema_is_const_expr( pExpr->u.exprNeq2_.expr_1 ) && sema_is_const_expr( pExpr->u.exprNeq2_.expr_2 );
   case Expr_::is_ExprNeq3:
      return sema_is_const_expr( pExpr->u.exprNeq3_.expr_1 ) && sema_is_const_expr( pExpr->u.exprNeq3_.expr_2 );
   case Expr_::is_ExprLt:
      return sema_is_const_expr( pExpr->u.exprLt_.expr_1 ) && sema_is_const_expr( pExpr->u.exprLt_.expr_2 );
   case Expr_::is_ExprLe:
      return sema_is_const_expr( pExpr->u.exprLe_.expr_1 ) && sema_is_const_expr( pExpr->u.exprLe_.expr_2 );
   case Expr_::is_ExprGt:
      return sema_is_const_expr( pExpr->u.exprGt_.expr_1 ) && sema_is_const_expr( pExpr->u.exprGt_.expr_2 );
   case Expr_::is_ExprGe:
      return sema_is_const_expr( pExpr->u.exprGe_.expr_1 ) && sema_is_const_expr( pExpr->u.exprGe_.expr_2 );
   case Expr_::is_ExprSubStr:
      return sema_is_const_expr( pExpr->u.exprSubStr_.expr_1 ) && sema_is_const_expr( pExpr->u.exprSubStr_.expr_2 );
   case Expr_::is_ExprRelBase:
      return sema_is_const_expr( pExpr->u.exprRelBase_.expr_ );
   case Expr_::is_ExprAdd:
      return sema_is_const_expr( pExpr->u.exprAdd_.expr_1 ) && sema_is_const_expr( pExpr->u.exprAdd_.expr_2 );
   case Expr_::is_ExprSub:
      return sema_is_const_expr( pExpr->u.exprSub_.expr_1 ) && sema_is_const_expr( pExpr->u.exprSub_.expr_2 );
   case Expr_::is_ExprAddBase:
      return sema_is_const_expr( pExpr->u.exprAddBase_.expr_ );
   case Expr_::is_ExprMul:
      return sema_is_const_expr( pExpr->u.exprMul_.expr_1 ) && sema_is_const_expr( pExpr->u.exprMul_.expr_2 );
   case Expr_::is_ExprDiv:
      return sema_is_const_expr( pExpr->u.exprDiv_.expr_1 ) && sema_is_const_expr( pExpr->u.exprDiv_.expr_2 );
   case Expr_::is_ExprMod:
      return sema_is_const_expr( pExpr->u.exprMod_.expr_1 ) && sema_is_const_expr( pExpr->u.exprMod_.expr_2 );
   case Expr_::is_ExprMulBase:
      return sema_is_const_expr( pExpr->u.exprMulBase_.expr_ );
   case Expr_::is_ExprPow1:
      return sema_is_const_expr( pExpr->u.exprPow1_.expr_1 ) && sema_is_const_expr( pExpr->u.exprPow1_.expr_2 );
   case Expr_::is_ExprPow2:
      return sema_is_const_expr( pExpr->u.exprPow2_.expr_1 ) && sema_is_const_expr( pExpr->u.exprPow2_.expr_2 );
   case Expr_::is_ExprPowBase:
      return sema_is_const_expr( pExpr->u.exprPowBase_.expr_ );
   case Expr_::is_ExprPlus:
      return sema_is_const_expr( pExpr->u.exprPlus_.expr_ );
   case Expr_::is_ExprMinus:
      return sema_is_const_expr( pExpr->u.exprMinus_.expr_ );
   case Expr_::is_ExprSignBase:
      return sema_is_const_expr( pExpr->u.exprSignBase_.expr_ );
   case Expr_::is_ExprLit:
      return true;
   case Expr_::is_ExprParen:
      return sema_is_const_expr( pExpr->u.exprParen_.expr_ );
   case Expr_::is_ExprArray:
      return sema_is_const_expr_list( pExpr->u.exprArray_.exprlistopt_ );
   default:
      return false;
   }
}

static std::string sema_normalize_ident( const char *szName )
{
   std::string oOut;
   if( !szName )
      return oOut;

   for( const unsigned char *p = ( const unsigned char * ) szName; *p; ++p )
   {
      char ch = ( char ) *p;
      if( ch >= 'A' && ch <= 'Z' )
         ch = ( char ) ( ch - 'A' + 'a' );
      oOut.push_back( ch );
   }

   return oOut;
}

static void sema_add_name( NameSet &oNames, const char *szName )
{
   if( !szName )
      return;
   oNames.insert( sema_normalize_ident( szName ) );
}

static bool sema_is_declared( const NameSet &oNames, const char *szName )
{
   if( !szName )
      return true;
   return oNames.find( sema_normalize_ident( szName ) ) != oNames.end();
}

static bool sema_require_declared( const NameSet &oNames, const char *szName )
{
   if( !szName )
      return true;
   if( sema_is_declared( oNames, szName ) )
      return true;

   char szMessage[ 256 ];
   snprintf( szMessage, sizeof( szMessage ), "Niezadeklarowana zmienna: %s", szName );
   sema_ReportError( szMessage, 0 );
   return false;
}

static const char *sema_get_qualifiedid_base( QualifiedId pQ )
{
   if( !pQ )
      return NULL;

   switch( pQ->kind )
   {
   case QualifiedId_::is_QualifiedIdSingle:
      return pQ->u.qualifiedIdSingle_.ident_;
   case QualifiedId_::is_QualifiedIdMore:
      return pQ->u.qualifiedIdMore_.ident_;
   }

   return NULL;
}

static void sema_collect_param_names( ParamListOpt pOpt, NameSet &oNames )
{
   if( !pOpt || pOpt->kind == ParamListOpt_::is_OptParamListEmpty )
      return;

   sema_add_name( oNames, pOpt->u.optParamListSome_.ident_ );

   ParamListTail pTail = pOpt->u.optParamListSome_.paramlisttail_;
   while( pTail && pTail->kind == ParamListTail_::is_ListParamTailCons )
   {
      sema_add_name( oNames, pTail->u.listParamTailCons_.ident_ );
      pTail = pTail->u.listParamTailCons_.paramlisttail_;
   }
}

static void sema_collect_identinit_list( IdentInitList pList, NameSet &oNames )
{
   if( !pList || pList->kind != IdentInitList_::is_ListIdentInitSingle )
      return;

   IdentInit pItem = pList->u.listIdentInitSingle_.identinit_;
   if( pItem && pItem->kind == IdentInit_::is_IdentInitItem )
      sema_add_name( oNames, pItem->u.identInitItem_.ident_ );

   IdentInitTail pTail = pList->u.listIdentInitSingle_.identinittail_;
   while( pTail && pTail->kind == IdentInitTail_::is_ListIdentInitTailCons )
   {
      IdentInit pNext = pTail->u.listIdentInitTailCons_.identinit_;
      if( pNext && pNext->kind == IdentInit_::is_IdentInitItem )
         sema_add_name( oNames, pNext->u.identInitItem_.ident_ );
      pTail = pTail->u.listIdentInitTailCons_.identinittail_;
   }
}

static void sema_collect_local_names( LocalDeclsOpt pDecls, NameSet &oNames )
{
   while( pDecls && pDecls->kind != LocalDeclsOpt_::is_ListLocalDeclEmpty )
   {
      if( pDecls->kind == LocalDeclsOpt_::is_ListLocalDeclCons )
      {
         LocalDecl pDecl = pDecls->u.listLocalDeclCons_.localdecl_;
         if( pDecl && pDecl->kind == LocalDecl_::is_LocalDeclItem )
            sema_collect_identinit_list( pDecl->u.localDeclItem_.identinitlist_, oNames );
         pDecls = pDecls->u.listLocalDeclCons_.localdeclsopt_;
      }
      else if( pDecls->kind == LocalDeclsOpt_::is_ListLocalDeclLine )
      {
         pDecls = pDecls->u.listLocalDeclLine_.localdeclsopt_;
      }
      else
      {
         break;
      }
   }
}

static void sema_collect_static_item_list( StaticItemList pList, NameSet &oNames )
{
   if( !pList || pList->kind != StaticItemList_::is_StaticItemListSingle )
      return;

   StaticItem pItem = pList->u.staticItemListSingle_.staticitem_;
   if( pItem && pItem->kind == StaticItem_::is_StaticItemMain )
      sema_add_name( oNames, pItem->u.staticItemMain_.ident_ );

   StaticItemTail pTail = pList->u.staticItemListSingle_.staticitemtail_;
   while( pTail && pTail->kind == StaticItemTail_::is_StaticItemTailCons )
   {
      StaticItem pNext = pTail->u.staticItemTailCons_.staticitem_;
      if( pNext && pNext->kind == StaticItem_::is_StaticItemMain )
         sema_add_name( oNames, pNext->u.staticItemMain_.ident_ );
      pTail = pTail->u.staticItemTailCons_.staticitemtail_;
   }
}

static void sema_collect_static_names( StaticDeclsOpt pDecls, NameSet &oNames )
{
   while( pDecls && pDecls->kind == StaticDeclsOpt_::is_ListStaticDeclCons )
   {
      StaticDecl pDecl = pDecls->u.listStaticDeclCons_.staticdecl_;
      if( pDecl )
      {
         if( pDecl->kind == StaticDecl_::is_StaticStructDecl )
         {
            sema_collect_identinit_list( pDecl->u.staticStructDecl_.identinitlist_, oNames );
         }
         else if( pDecl->kind == StaticDecl_::is_StaticDeclItem )
         {
            sema_collect_static_item_list( pDecl->u.staticDeclItem_.staticitemlist_, oNames );
         }
      }
      pDecls = pDecls->u.listStaticDeclCons_.staticdeclsopt_;
   }
}

static bool sema_check_identinit_list_inits( IdentInitList pList, const NameSet &oNames )
{
   if( !pList || pList->kind != IdentInitList_::is_ListIdentInitSingle )
      return true;

   IdentInit pItem = pList->u.listIdentInitSingle_.identinit_;
   if( pItem && pItem->kind == IdentInit_::is_IdentInitItem )
   {
      InitOpt pInit = pItem->u.identInitItem_.initopt_;
      if( pInit && pInit->kind == InitOpt_::is_OptInitSome )
      {
         if( !sema_check_expr_declared( pInit->u.optInitSome_.expr_, oNames ) )
            return false;
      }
   }

   IdentInitTail pTail = pList->u.listIdentInitSingle_.identinittail_;
   while( pTail && pTail->kind == IdentInitTail_::is_ListIdentInitTailCons )
   {
      IdentInit pNext = pTail->u.listIdentInitTailCons_.identinit_;
      if( pNext && pNext->kind == IdentInit_::is_IdentInitItem )
      {
         InitOpt pInit = pNext->u.identInitItem_.initopt_;
         if( pInit && pInit->kind == InitOpt_::is_OptInitSome )
         {
            if( !sema_check_expr_declared( pInit->u.optInitSome_.expr_, oNames ) )
               return false;
         }
      }
      pTail = pTail->u.listIdentInitTailCons_.identinittail_;
   }

   return true;
}

static bool sema_check_local_inits( LocalDeclsOpt pDecls, const NameSet &oNames )
{
   while( pDecls && pDecls->kind != LocalDeclsOpt_::is_ListLocalDeclEmpty )
   {
      if( pDecls->kind == LocalDeclsOpt_::is_ListLocalDeclCons )
      {
         LocalDecl pDecl = pDecls->u.listLocalDeclCons_.localdecl_;
         if( pDecl && pDecl->kind == LocalDecl_::is_LocalDeclItem )
         {
            if( !sema_check_identinit_list_inits( pDecl->u.localDeclItem_.identinitlist_, oNames ) )
               return false;
         }
         pDecls = pDecls->u.listLocalDeclCons_.localdeclsopt_;
      }
      else if( pDecls->kind == LocalDeclsOpt_::is_ListLocalDeclLine )
      {
         pDecls = pDecls->u.listLocalDeclLine_.localdeclsopt_;
      }
      else
      {
         break;
      }
   }

   return true;
}

static bool sema_check_stmtlist_nonempty( StmtListNonEmpty pList, const NameSet &oNames )
{
   if( !pList || pList->kind != StmtListNonEmpty_::is_ListStmtNonEmpty )
      return true;

   if( !sema_check_stmt( pList->u.listStmtNonEmpty_.stmt_, oNames ) )
      return false;
   return sema_check_stmtlist( pList->u.listStmtNonEmpty_.stmtlist_, oNames );
}

static bool sema_check_stmtlist( StmtList pList, const NameSet &oNames )
{
   while( pList )
   {
      if( pList->kind == StmtList_::is_ListStmtEmpty )
         return true;
      if( pList->kind == StmtList_::is_ListStmtLine )
      {
         pList = pList->u.listStmtLine_.stmtlist_;
         continue;
      }
      if( pList->kind == StmtList_::is_ListStmtCons )
      {
         if( !sema_check_stmt( pList->u.listStmtCons_.stmt_, oNames ) )
            return false;
         pList = pList->u.listStmtCons_.stmtlist_;
         continue;
      }
      break;
   }

   return true;
}

static bool sema_check_stmt( Stmt pStmt, const NameSet &oNames )
{
   if( !pStmt )
      return true;

   switch( pStmt->kind )
   {
   case Stmt_::is_StmtAssignInl:
      return sema_check_lhs_declared( pStmt->u.stmtAssignInl_.lhs_, oNames ) &&
             sema_check_expr_declared( pStmt->u.stmtAssignInl_.expr_, oNames );
   case Stmt_::is_StmtAssignAdd:
      return sema_check_lhs_declared( pStmt->u.stmtAssignAdd_.lhs_, oNames ) &&
             sema_check_expr_declared( pStmt->u.stmtAssignAdd_.expr_, oNames );
   case Stmt_::is_StmtAssignSub:
      return sema_check_lhs_declared( pStmt->u.stmtAssignSub_.lhs_, oNames ) &&
             sema_check_expr_declared( pStmt->u.stmtAssignSub_.expr_, oNames );
   case Stmt_::is_StmtAssignMul:
      return sema_check_lhs_declared( pStmt->u.stmtAssignMul_.lhs_, oNames ) &&
             sema_check_expr_declared( pStmt->u.stmtAssignMul_.expr_, oNames );
   case Stmt_::is_StmtAssignDiv:
      return sema_check_lhs_declared( pStmt->u.stmtAssignDiv_.lhs_, oNames ) &&
             sema_check_expr_declared( pStmt->u.stmtAssignDiv_.expr_, oNames );
   case Stmt_::is_StmtAssignMod:
      return sema_check_lhs_declared( pStmt->u.stmtAssignMod_.lhs_, oNames ) &&
             sema_check_expr_declared( pStmt->u.stmtAssignMod_.expr_, oNames );
   case Stmt_::is_StmtAssignPow:
      return sema_check_lhs_declared( pStmt->u.stmtAssignPow_.lhs_, oNames ) &&
             sema_check_expr_declared( pStmt->u.stmtAssignPow_.expr_, oNames );
   case Stmt_::is_StmtAssignPow2:
      return sema_check_lhs_declared( pStmt->u.stmtAssignPow2_.lhs_, oNames ) &&
             sema_check_expr_declared( pStmt->u.stmtAssignPow2_.expr_, oNames );
   case Stmt_::is_StmtCall:
      return sema_check_call_expr( pStmt->u.stmtCall_.callexpr_, oNames );
   case Stmt_::is_StmtIf:
   {
      if( !sema_check_if_cond( pStmt->u.stmtIf_.ifcond_, oNames ) )
         return false;
      if( !sema_check_stmtlist( pStmt->u.stmtIf_.stmtlist_, oNames ) )
         return false;

      ElseIfList pElseIf = pStmt->u.stmtIf_.elseiflist_;
      while( pElseIf && pElseIf->kind == ElseIfList_::is_ListElseIfCons )
      {
         ElseIf pClause = pElseIf->u.listElseIfCons_.elseif_;
         if( pClause && pClause->kind == ElseIf_::is_ElseIfClause )
         {
            if( !sema_check_if_cond( pClause->u.elseIfClause_.ifcond_, oNames ) )
               return false;
            if( !sema_check_stmtlist( pClause->u.elseIfClause_.stmtlist_, oNames ) )
               return false;
         }
         pElseIf = pElseIf->u.listElseIfCons_.elseiflist_;
      }

      return sema_check_else_opt( pStmt->u.stmtIf_.elseopt_, oNames );
   }
   case Stmt_::is_StmtDoWhile:
      return sema_check_while_cond( pStmt->u.stmtDoWhile_.whilecond_, oNames ) &&
             sema_check_stmtlist( pStmt->u.stmtDoWhile_.stmtlist_, oNames );
   case Stmt_::is_StmtFor:
      if( !sema_require_declared( oNames, pStmt->u.stmtFor_.ident_ ) )
         return false;
      if( !sema_check_expr_declared( pStmt->u.stmtFor_.expr_1, oNames ) )
         return false;
      if( !sema_check_expr_declared( pStmt->u.stmtFor_.expr_2, oNames ) )
         return false;
      if( !sema_check_step_opt( pStmt->u.stmtFor_.stepopt_, oNames ) )
         return false;
      return sema_check_stmtlist( pStmt->u.stmtFor_.stmtlist_, oNames );
   case Stmt_::is_StmtForEach:
      return sema_check_foreach_vars( pStmt->u.stmtForEach_.foreachvars_, oNames ) &&
             sema_check_foreach_exprs( pStmt->u.stmtForEach_.foreachexprs_, oNames ) &&
             sema_check_stmtlist( pStmt->u.stmtForEach_.stmtlist_, oNames );
   case Stmt_::is_StmtSwitch:
      return sema_check_switch_cond( pStmt->u.stmtSwitch_.switchcond_, oNames ) &&
             sema_check_case_list( pStmt->u.stmtSwitch_.caselist_, oNames ) &&
             sema_check_otherwise_opt( pStmt->u.stmtSwitch_.otherwiseopt_, oNames );
   case Stmt_::is_StmtDoCase:
      return sema_check_case_cond_list( pStmt->u.stmtDoCase_.casecondlist_, oNames ) &&
             sema_check_otherwise_opt( pStmt->u.stmtDoCase_.otherwiseopt_, oNames );
   case Stmt_::is_StmtSequence:
      return sema_check_stmtlist( pStmt->u.stmtSequence_.stmtlist_, oNames ) &&
             sema_check_recover_opt( pStmt->u.stmtSequence_.recoveropt_, oNames );
   case Stmt_::is_StmtBreak:
      return sema_check_break_expr_opt( pStmt->u.stmtBreak_.breakexpropt_, oNames );
   case Stmt_::is_StmtExit:
   case Stmt_::is_StmtLoop:
      return true;
   case Stmt_::is_StmtReturn:
      return sema_check_return_expr_opt( pStmt->u.stmtReturn_.returnexpropt_, oNames );
   case Stmt_::is_StmtStructDecl:
      return true;
   }

   return true;
}

static bool sema_check_expr_or_declared( ExprOr pExpr, const NameSet &oNames )
{
   if( !pExpr )
      return true;

   switch( pExpr->kind )
   {
   case ExprOr_::is_ExprOrBase:
      return sema_check_expr_declared( pExpr->u.exprOrBase_.expr_, oNames );
   case ExprOr_::is_ExprOrDot:
      return sema_check_expr_declared( pExpr->u.exprOrDot_.expr_, oNames ) &&
             sema_check_expr_or_declared( pExpr->u.exprOrDot_.expror_, oNames );
   }

   return true;
}

static bool sema_check_expr_declared( Expr pExpr, const NameSet &oNames )
{
   if( !pExpr )
      return true;

   switch( pExpr->kind )
   {
   case Expr_::is_ExprAssignBase:
      return sema_check_expr_or_declared( pExpr->u.exprAssignBase_.expror_, oNames );
   case Expr_::is_ExprAndDot:
      return sema_check_expr_declared( pExpr->u.exprAndDot_.expr_1, oNames ) &&
             sema_check_expr_declared( pExpr->u.exprAndDot_.expr_2, oNames );
   case Expr_::is_ExprAndBase:
      return sema_check_expr_declared( pExpr->u.exprAndBase_.expr_, oNames );
   case Expr_::is_ExprNotDot:
      return sema_check_expr_declared( pExpr->u.exprNotDot_.expr_, oNames );
   case Expr_::is_ExprNotBase:
      return sema_check_expr_declared( pExpr->u.exprNotBase_.expr_, oNames );
   case Expr_::is_ExprEq:
      return sema_check_expr_declared( pExpr->u.exprEq_.expr_1, oNames ) &&
             sema_check_expr_declared( pExpr->u.exprEq_.expr_2, oNames );
   case Expr_::is_ExprEq2:
      return sema_check_expr_declared( pExpr->u.exprEq2_.expr_1, oNames ) &&
             sema_check_expr_declared( pExpr->u.exprEq2_.expr_2, oNames );
   case Expr_::is_ExprNeq:
      return sema_check_expr_declared( pExpr->u.exprNeq_.expr_1, oNames ) &&
             sema_check_expr_declared( pExpr->u.exprNeq_.expr_2, oNames );
   case Expr_::is_ExprNeq2:
      return sema_check_expr_declared( pExpr->u.exprNeq2_.expr_1, oNames ) &&
             sema_check_expr_declared( pExpr->u.exprNeq2_.expr_2, oNames );
   case Expr_::is_ExprNeq3:
      return sema_check_expr_declared( pExpr->u.exprNeq3_.expr_1, oNames ) &&
             sema_check_expr_declared( pExpr->u.exprNeq3_.expr_2, oNames );
   case Expr_::is_ExprLt:
      return sema_check_expr_declared( pExpr->u.exprLt_.expr_1, oNames ) &&
             sema_check_expr_declared( pExpr->u.exprLt_.expr_2, oNames );
   case Expr_::is_ExprLe:
      return sema_check_expr_declared( pExpr->u.exprLe_.expr_1, oNames ) &&
             sema_check_expr_declared( pExpr->u.exprLe_.expr_2, oNames );
   case Expr_::is_ExprGt:
      return sema_check_expr_declared( pExpr->u.exprGt_.expr_1, oNames ) &&
             sema_check_expr_declared( pExpr->u.exprGt_.expr_2, oNames );
   case Expr_::is_ExprGe:
      return sema_check_expr_declared( pExpr->u.exprGe_.expr_1, oNames ) &&
             sema_check_expr_declared( pExpr->u.exprGe_.expr_2, oNames );
   case Expr_::is_ExprSubStr:
      return sema_check_expr_declared( pExpr->u.exprSubStr_.expr_1, oNames ) &&
             sema_check_expr_declared( pExpr->u.exprSubStr_.expr_2, oNames );
   case Expr_::is_ExprRelBase:
      return sema_check_expr_declared( pExpr->u.exprRelBase_.expr_, oNames );
   case Expr_::is_ExprAdd:
      return sema_check_expr_declared( pExpr->u.exprAdd_.expr_1, oNames ) &&
             sema_check_expr_declared( pExpr->u.exprAdd_.expr_2, oNames );
   case Expr_::is_ExprSub:
      return sema_check_expr_declared( pExpr->u.exprSub_.expr_1, oNames ) &&
             sema_check_expr_declared( pExpr->u.exprSub_.expr_2, oNames );
   case Expr_::is_ExprAddBase:
      return sema_check_expr_declared( pExpr->u.exprAddBase_.expr_, oNames );
   case Expr_::is_ExprMul:
      return sema_check_expr_declared( pExpr->u.exprMul_.expr_1, oNames ) &&
             sema_check_expr_declared( pExpr->u.exprMul_.expr_2, oNames );
   case Expr_::is_ExprDiv:
      return sema_check_expr_declared( pExpr->u.exprDiv_.expr_1, oNames ) &&
             sema_check_expr_declared( pExpr->u.exprDiv_.expr_2, oNames );
   case Expr_::is_ExprMod:
      return sema_check_expr_declared( pExpr->u.exprMod_.expr_1, oNames ) &&
             sema_check_expr_declared( pExpr->u.exprMod_.expr_2, oNames );
   case Expr_::is_ExprMulBase:
      return sema_check_expr_declared( pExpr->u.exprMulBase_.expr_, oNames );
   case Expr_::is_ExprPow1:
      return sema_check_expr_declared( pExpr->u.exprPow1_.expr_1, oNames ) &&
             sema_check_expr_declared( pExpr->u.exprPow1_.expr_2, oNames );
   case Expr_::is_ExprPow2:
      return sema_check_expr_declared( pExpr->u.exprPow2_.expr_1, oNames ) &&
             sema_check_expr_declared( pExpr->u.exprPow2_.expr_2, oNames );
   case Expr_::is_ExprPowBase:
      return sema_check_expr_declared( pExpr->u.exprPowBase_.expr_, oNames );
   case Expr_::is_ExprAddrOf:
      return sema_check_lhs_declared( pExpr->u.exprAddrOf_.lhs_, oNames );
   case Expr_::is_ExprDeref:
      return sema_check_expr_declared( pExpr->u.exprDeref_.expr_, oNames );
   case Expr_::is_ExprDecPre:
      return sema_check_expr_declared( pExpr->u.exprDecPre_.expr_, oNames );
   case Expr_::is_ExprIncPre:
      return sema_check_expr_declared( pExpr->u.exprIncPre_.expr_, oNames );
   case Expr_::is_ExprPlus:
      return sema_check_expr_declared( pExpr->u.exprPlus_.expr_, oNames );
   case Expr_::is_ExprMinus:
      return sema_check_expr_declared( pExpr->u.exprMinus_.expr_, oNames );
   case Expr_::is_ExprSignBase:
      return sema_check_expr_declared( pExpr->u.exprSignBase_.expr_, oNames );
   case Expr_::is_ExprLit:
      return true;
   case Expr_::is_ExprCall:
      return sema_check_call_expr( pExpr->u.exprCall_.callexpr_, oNames );
   case Expr_::is_ExprQId:
      return sema_require_declared( oNames, sema_get_qualifiedid_base( pExpr->u.exprQId_.qualifiedid_ ) );
   case Expr_::is_ExprParen:
      return sema_check_expr_declared( pExpr->u.exprParen_.expr_, oNames );
   case Expr_::is_ExprIndex:
      return sema_check_expr_declared( pExpr->u.exprIndex_.expr_, oNames ) &&
             sema_check_subscript_list( pExpr->u.exprIndex_.subscriptlist_, oNames );
   case Expr_::is_ExprArray:
      return sema_check_expr_list( pExpr->u.exprArray_.exprlistopt_, oNames );
   case Expr_::is_ExprBlock:
   {
      NameSet oBlockNames = oNames;
      sema_collect_param_names( pExpr->u.exprBlock_.paramlistopt_, oBlockNames );
      return sema_check_expr_list( pExpr->u.exprBlock_.exprlistopt_, oBlockNames );
   }
   case Expr_::is_ExprMethodCall:
      return sema_check_expr_declared( pExpr->u.exprMethodCall_.expr_, oNames ) &&
             sema_check_arg_list( pExpr->u.exprMethodCall_.arglistopt_, oNames );
   case Expr_::is_ExprFieldAccess:
      return sema_check_expr_declared( pExpr->u.exprFieldAccess_.expr_, oNames );
   case Expr_::is_ExprStructInitNamed:
      return sema_check_field_init_list( pExpr->u.exprStructInitNamed_.fieldinitlist_, oNames );
   case Expr_::is_ExprStructAnon:
      return sema_check_field_init_list( pExpr->u.exprStructAnon_.fieldinitlist_, oNames );
   case Expr_::is_ExprDecPost:
      return sema_check_expr_declared( pExpr->u.exprDecPost_.expr_, oNames );
   case Expr_::is_ExprIncPost:
      return sema_check_expr_declared( pExpr->u.exprIncPost_.expr_, oNames );
   }

   return true;
}

static bool sema_check_lhs_declared( LHS pLhs, const NameSet &oNames )
{
   if( !pLhs )
      return true;

   switch( pLhs->kind )
   {
   case LHS_::is_LHSVal:
      return sema_check_lvalue_declared( pLhs->u.lHSVal_.lvalue_, oNames );
   case LHS_::is_LHSDeref:
      return sema_check_expr_declared( pLhs->u.lHSDeref_.expr_, oNames );
   }

   return true;
}

static bool sema_check_lvalue_declared( LValue pLVal, const NameSet &oNames )
{
   if( !pLVal )
      return true;

   switch( pLVal->kind )
   {
   case LValue_::is_LValQId:
      return sema_require_declared( oNames, sema_get_qualifiedid_base( pLVal->u.lValQId_.qualifiedid_ ) );
   case LValue_::is_LValField:
      return sema_check_lvalue_declared( pLVal->u.lValField_.lvalue_, oNames );
   case LValue_::is_LValIndex:
      return sema_check_lvalue_declared( pLVal->u.lValIndex_.lvalue_, oNames ) &&
             sema_check_subscript_list( pLVal->u.lValIndex_.subscriptlist_, oNames );
   }

   return true;
}

static bool sema_check_arg_list( ArgListOpt pArgs, const NameSet &oNames )
{
   if( !pArgs || pArgs->kind == ArgListOpt_::is_OptArgListEmpty )
      return true;

   if( !sema_check_expr_declared( pArgs->u.optArgListSome_.expr_, oNames ) )
      return false;

   ArgListTail pTail = pArgs->u.optArgListSome_.arglisttail_;
   while( pTail && pTail->kind == ArgListTail_::is_ListArgTailCons )
   {
      if( !sema_check_expr_declared( pTail->u.listArgTailCons_.expr_, oNames ) )
         return false;
      pTail = pTail->u.listArgTailCons_.arglisttail_;
   }

   return true;
}

static bool sema_check_expr_list( ExprListOpt pList, const NameSet &oNames )
{
   if( !pList || pList->kind == ExprListOpt_::is_OptExprListEmpty )
      return true;

   return sema_check_expr_declared( pList->u.optExprListSome_.expr_, oNames ) &&
          sema_check_expr_list_tail( pList->u.optExprListSome_.exprlisttail_, oNames );
}

static bool sema_check_expr_list_tail( ExprListTail pTail, const NameSet &oNames )
{
   while( pTail && pTail->kind == ExprListTail_::is_ListExprTailCons )
   {
      if( !sema_check_expr_declared( pTail->u.listExprTailCons_.expr_, oNames ) )
         return false;
      pTail = pTail->u.listExprTailCons_.exprlisttail_;
   }
   return true;
}

static bool sema_check_subscript_list( SubscriptList pList, const NameSet &oNames )
{
   if( !pList )
      return true;

   if( !sema_check_expr_declared( pList->u.subscriptListSingle_.expr_, oNames ) )
      return false;
   return sema_check_subscript_tail( pList->u.subscriptListSingle_.subscriptlisttail_, oNames );
}

static bool sema_check_subscript_tail( SubscriptListTail pTail, const NameSet &oNames )
{
   while( pTail && pTail->kind == SubscriptListTail_::is_ListSubscriptTailCons )
   {
      if( !sema_check_expr_declared( pTail->u.listSubscriptTailCons_.expr_, oNames ) )
         return false;
      pTail = pTail->u.listSubscriptTailCons_.subscriptlisttail_;
   }
   return true;
}

static bool sema_check_field_init_list( FieldInitList pList, const NameSet &oNames )
{
   if( !pList )
      return true;

   if( pList->kind == FieldInitList_::is_FieldInitListSingle )
   {
      FieldInit pItem = pList->u.fieldInitListSingle_.fieldinit_;
      if( pItem && pItem->kind == FieldInit_::is_FieldInitItem )
         return sema_check_expr_declared( pItem->u.fieldInitItem_.expr_, oNames );
      return true;
   }

   if( pList->kind == FieldInitList_::is_FieldInitListCons )
   {
      FieldInit pItem = pList->u.fieldInitListCons_.fieldinit_;
      if( pItem && pItem->kind == FieldInit_::is_FieldInitItem )
      {
         if( !sema_check_expr_declared( pItem->u.fieldInitItem_.expr_, oNames ) )
            return false;
      }
      return sema_check_field_init_list( pList->u.fieldInitListCons_.fieldinitlist_, oNames );
   }

   return true;
}

static bool sema_check_call_expr( CallExpr pCall, const NameSet &oNames )
{
   if( !pCall || pCall->kind != CallExpr_::is_CallExprMain )
      return true;

   return sema_check_arg_list( pCall->u.callExprMain_.arglistopt_, oNames );
}

static bool sema_check_if_cond( IfCond pCond, const NameSet &oNames )
{
   if( !pCond || pCond->kind != IfCond_::is_IfCondExpr )
      return true;
   return sema_check_expr_declared( pCond->u.ifCondExpr_.expr_, oNames );
}

static bool sema_check_while_cond( WhileCond pCond, const NameSet &oNames )
{
   if( !pCond || pCond->kind != WhileCond_::is_WhileCondExpr )
      return true;
   return sema_check_expr_declared( pCond->u.whileCondExpr_.expr_, oNames );
}

static bool sema_check_foreach_vars( ForEachVars pVars, const NameSet &oNames )
{
   if( !pVars || pVars->kind != ForEachVars_::is_ForEachVarsMain )
      return true;

   if( !sema_require_declared( oNames, pVars->u.forEachVarsMain_.ident_ ) )
      return false;
   return sema_check_foreach_vars_tail( pVars->u.forEachVarsMain_.foreachvarstail_, oNames );
}

static bool sema_check_foreach_vars_tail( ForEachVarsTail pTail, const NameSet &oNames )
{
   while( pTail && pTail->kind == ForEachVarsTail_::is_ListForEachVarsTailCons )
   {
      if( !sema_require_declared( oNames, pTail->u.listForEachVarsTailCons_.ident_ ) )
         return false;
      pTail = pTail->u.listForEachVarsTailCons_.foreachvarstail_;
   }
   return true;
}

static bool sema_check_foreach_exprs( ForEachExprs pExprs, const NameSet &oNames )
{
   if( !pExprs || pExprs->kind != ForEachExprs_::is_ForEachExprsMain )
      return true;

   if( !sema_check_expr_declared( pExprs->u.forEachExprsMain_.expr_, oNames ) )
      return false;
   return sema_check_foreach_exprs_tail( pExprs->u.forEachExprsMain_.foreachexprstail_, oNames );
}

static bool sema_check_foreach_exprs_tail( ForEachExprsTail pTail, const NameSet &oNames )
{
   while( pTail && pTail->kind == ForEachExprsTail_::is_ListForEachExprsTailCons )
   {
      if( !sema_check_expr_declared( pTail->u.listForEachExprsTailCons_.expr_, oNames ) )
         return false;
      pTail = pTail->u.listForEachExprsTailCons_.foreachexprstail_;
   }
   return true;
}

static bool sema_check_case_list( CaseList pList, const NameSet &oNames )
{
   while( pList && pList->kind == CaseList_::is_ListCaseCons )
   {
      if( !sema_check_case_clause( pList->u.listCaseCons_.caseclause_, oNames ) )
         return false;
      pList = pList->u.listCaseCons_.caselist_;
   }
   return true;
}

static bool sema_check_case_clause( CaseClause pClause, const NameSet &oNames )
{
   if( !pClause || pClause->kind != CaseClause_::is_CaseClauseItem )
      return true;
   return sema_check_stmtlist( pClause->u.caseClauseItem_.stmtlist_, oNames );
}

static bool sema_check_case_cond_list( CaseCondList pList, const NameSet &oNames )
{
   while( pList && pList->kind == CaseCondList_::is_ListCaseCondCons )
   {
      if( !sema_check_case_cond_clause( pList->u.listCaseCondCons_.casecondclause_, oNames ) )
         return false;
      pList = pList->u.listCaseCondCons_.casecondlist_;
   }
   return true;
}

static bool sema_check_case_cond_clause( CaseCondClause pClause, const NameSet &oNames )
{
   if( !pClause || pClause->kind != CaseCondClause_::is_CaseCondClauseItem )
      return true;
   return sema_check_expr_declared( pClause->u.caseCondClauseItem_.expr_, oNames ) &&
          sema_check_stmtlist( pClause->u.caseCondClauseItem_.stmtlist_, oNames );
}

static bool sema_check_else_opt( ElseOpt pOpt, const NameSet &oNames )
{
   if( !pOpt || pOpt->kind != ElseOpt_::is_OptElseSome )
      return true;
   return sema_check_stmtlist( pOpt->u.optElseSome_.stmtlist_, oNames );
}

static bool sema_check_otherwise_opt( OtherwiseOpt pOpt, const NameSet &oNames )
{
   if( !pOpt || pOpt->kind != OtherwiseOpt_::is_OptOtherwiseSome )
      return true;
   return sema_check_stmtlist( pOpt->u.optOtherwiseSome_.stmtlist_, oNames );
}

static bool sema_check_recover_opt( RecoverOpt pOpt, const NameSet &oNames )
{
   if( !pOpt || pOpt->kind != RecoverOpt_::is_RecoverOptSome )
      return true;

   RecoverUsingOpt pUsing = pOpt->u.recoverOptSome_.recoverusingopt_;
   if( pUsing && pUsing->kind == RecoverUsingOpt_::is_RecoverUsingOptSome )
   {
      if( !sema_require_declared( oNames, pUsing->u.recoverUsingOptSome_.ident_ ) )
         return false;
   }

   return sema_check_stmtlist( pOpt->u.recoverOptSome_.stmtlist_, oNames );
}

static bool sema_check_break_expr_opt( BreakExprOpt pOpt, const NameSet &oNames )
{
   if( !pOpt || pOpt->kind != BreakExprOpt_::is_BreakExprOptSome )
      return true;
   return sema_check_expr_declared( pOpt->u.breakExprOptSome_.expr_, oNames );
}

static bool sema_check_return_expr_opt( ReturnExprOpt pOpt, const NameSet &oNames )
{
   if( !pOpt || pOpt->kind != ReturnExprOpt_::is_ReturnExprOptSome )
      return true;
   return sema_check_expr_declared( pOpt->u.returnExprOptSome_.expr_, oNames );
}

static bool sema_check_step_opt( StepOpt pOpt, const NameSet &oNames )
{
   if( !pOpt || pOpt->kind != StepOpt_::is_OptStepSome )
      return true;
   return sema_check_expr_declared( pOpt->u.optStepSome_.expr_, oNames );
}

static bool sema_check_switch_cond( SwitchCond pCond, const NameSet &oNames )
{
   if( !pCond || pCond->kind != SwitchCond_::is_SwitchCondExpr )
      return true;
   return sema_check_expr_declared( pCond->u.switchCondExpr_.expr_, oNames );
}

static bool sema_check_static_item( StaticItem pItem )
{
   if( !pItem || pItem->kind != StaticItem_::is_StaticItemMain )
      return true;

   bool lHasDims =
       ( pItem->u.staticItemMain_.staticarraydimsopt_ &&
         pItem->u.staticItemMain_.staticarraydimsopt_->kind != StaticArrayDimsOpt_::is_StaticArrayDimsEmpty );
   bool lHasInit = ( pItem->u.staticItemMain_.staticinitopt_ &&
                     pItem->u.staticItemMain_.staticinitopt_->kind != StaticInitOpt_::is_StaticInitEmpty );

   if( lHasDims )
   {
      if( lHasInit )
      {
         sema_ReportError( "Niedozwolona inicjalizacja tablicy statycznej", 0 );
         return false;
      }
      if( !sema_check_static_dims( pItem->u.staticItemMain_.staticarraydimsopt_ ) )
      {
         sema_ReportError( "Rozmiar tablicy statycznej musi byc stala", 0 );
         return false;
      }
   }

   if( lHasInit )
   {
      Expr pInit = pItem->u.staticItemMain_.staticinitopt_->u.staticInitSome_.expr_;
      if( !sema_is_const_expr( pInit ) )
      {
         sema_ReportError( "Inicjalizator statyczny musi byc stala", 0 );
         return false;
      }
   }

   return true;
}

static bool sema_check_static_dims( StaticArrayDimsOpt pDims )
{
   if( !pDims || pDims->kind == StaticArrayDimsOpt_::is_StaticArrayDimsEmpty )
      return true;

   if( pDims->kind == StaticArrayDimsOpt_::is_StaticArrayDimsCons )
   {
      if( !sema_is_const_subscript_list( pDims->u.staticArrayDimsCons_.subscriptlist_ ) )
         return false;
      return sema_check_static_dims( pDims->u.staticArrayDimsCons_.staticarraydimsopt_ );
   }

   return true;
}

static bool sema_check_static_item_list( StaticItemList pList )
{
   if( !pList )
      return true;
   if( pList->kind == StaticItemList_::is_StaticItemListSingle )
   {
      if( !sema_check_static_item( pList->u.staticItemListSingle_.staticitem_ ) )
         return false;
      StaticItemTail pTail = pList->u.staticItemListSingle_.staticitemtail_;
      while( pTail && pTail->kind == StaticItemTail_::is_StaticItemTailCons )
      {
         if( !sema_check_static_item( pTail->u.staticItemTailCons_.staticitem_ ) )
            return false;
         pTail = pTail->u.staticItemTailCons_.staticitemtail_;
      }
      return true;
   }
   return true;
}

static bool sema_check_static_decl( StaticDecl pDecl )
{
   if( !pDecl || pDecl->kind != StaticDecl_::is_StaticDeclItem )
      return true;
   return sema_check_static_item_list( pDecl->u.staticDeclItem_.staticitemlist_ );
}

static bool sema_check_static_decls( StaticDeclsOpt pDecls )
{
   if( !pDecls || pDecls->kind == StaticDeclsOpt_::is_ListStaticDeclEmpty )
      return true;
   if( pDecls->kind == StaticDeclsOpt_::is_ListStaticDeclCons )
   {
      if( !sema_check_static_decl( pDecls->u.listStaticDeclCons_.staticdecl_ ) )
         return false;
      return sema_check_static_decls( pDecls->u.listStaticDeclCons_.staticdeclsopt_ );
   }
   return true;
}

static bool sema_check_topdecls( TopDecls pTopDecls )
{
   if( !pTopDecls )
      return true;
   switch( pTopDecls->kind )
   {
   case TopDecls_::is_ListTopDeclEmpty:
      return true;
   case TopDecls_::is_ListTopDeclLine:
      return sema_check_topdecls( pTopDecls->u.listTopDeclLine_.topdecls_ );
   case TopDecls_::is_ListTopDeclCons:
   {
      TopDecl pTop = pTopDecls->u.listTopDeclCons_.topdecl_;
      if( pTop && pTop->kind == TopDecl_::is_TopDeclStatic )
      {
         if( !sema_check_static_decl( pTop->u.topDeclStatic_.staticdecl_ ) )
            return false;
      }
      return sema_check_topdecls( pTopDecls->u.listTopDeclCons_.topdecls_ );
   }
   }
   return true;
}

static bool sema_check_def_statics( Def pDef )
{
   if( !pDef )
      return true;
   switch( pDef->kind )
   {
   case Def_::is_DefFun:
      return sema_check_static_decls( pDef->u.defFun_.functiondef_->u.functionDefMain_.staticdeclsopt_ );
   case Def_::is_DefProc:
      return sema_check_static_decls( pDef->u.defProc_.proceduredef_->u.procedureDefMain_.staticdeclsopt_ );
   case Def_::is_DefMeth:
      return sema_check_static_decls( pDef->u.defMeth_.methoddef_->u.methodDefMain_.staticdeclsopt_ );
   }
   return true;
}

static bool sema_check_def_locals( Def pDef )
{
   if( !pDef )
      return true;

   ParamListOpt     pParams  = NULL;
   LocalDeclsOpt    pLocals  = NULL;
   StaticDeclsOpt   pStatics = NULL;
   StmtListNonEmpty pStmts   = NULL;

   switch( pDef->kind )
   {
   case Def_::is_DefFun:
      pParams  = pDef->u.defFun_.functiondef_->u.functionDefMain_.paramlistopt_;
      pLocals  = pDef->u.defFun_.functiondef_->u.functionDefMain_.localdeclsopt_;
      pStatics = pDef->u.defFun_.functiondef_->u.functionDefMain_.staticdeclsopt_;
      pStmts   = pDef->u.defFun_.functiondef_->u.functionDefMain_.stmtlistnonempty_;
      break;
   case Def_::is_DefProc:
      pParams  = pDef->u.defProc_.proceduredef_->u.procedureDefMain_.paramlistopt_;
      pLocals  = pDef->u.defProc_.proceduredef_->u.procedureDefMain_.localdeclsopt_;
      pStatics = pDef->u.defProc_.proceduredef_->u.procedureDefMain_.staticdeclsopt_;
      pStmts   = pDef->u.defProc_.proceduredef_->u.procedureDefMain_.stmtlistnonempty_;
      break;
   case Def_::is_DefMeth:
      pParams  = pDef->u.defMeth_.methoddef_->u.methodDefMain_.paramlistopt_;
      pLocals  = pDef->u.defMeth_.methoddef_->u.methodDefMain_.localdeclsopt_;
      pStatics = pDef->u.defMeth_.methoddef_->u.methodDefMain_.staticdeclsopt_;
      pStmts   = pDef->u.defMeth_.methoddef_->u.methodDefMain_.stmtlistnonempty_;
      break;
   }

   NameSet oNames;
   sema_collect_param_names( pParams, oNames );
   sema_collect_local_names( pLocals, oNames );
   sema_collect_static_names( pStatics, oNames );

   if( !sema_check_local_inits( pLocals, oNames ) )
      return false;

   return sema_check_stmtlist_nonempty( pStmts, oNames );
}

static bool sema_check_defs( Defs pDefs )
{
   if( !pDefs )
      return true;

   switch( pDefs->kind )
   {
   case Defs_::is_ListDefSingle:
      return sema_check_def_params( pDefs->u.listDefSingle_.def_ ) &&
             sema_check_def_statics( pDefs->u.listDefSingle_.def_ ) &&
             sema_check_def_locals( pDefs->u.listDefSingle_.def_ );
   case Defs_::is_ListDefCons:
      if( !sema_check_def_params( pDefs->u.listDefCons_.def_ ) )
         return false;
      if( !sema_check_def_statics( pDefs->u.listDefCons_.def_ ) )
         return false;
      if( !sema_check_def_locals( pDefs->u.listDefCons_.def_ ) )
         return false;
      return sema_check_defs( pDefs->u.listDefCons_.defs_ );
   }

   return true;
}

static int sema_get_def_line( Def pDef )
{
   ( void ) pDef;
   return 0;
}

static int sema_get_first_def_line( Defs pDefs )
{
   if( !pDefs )
      return 0;

   switch( pDefs->kind )
   {
   case Defs_::is_ListDefSingle:
      return sema_get_def_line( pDefs->u.listDefSingle_.def_ );
   case Defs_::is_ListDefCons:
      return sema_get_def_line( pDefs->u.listDefCons_.def_ );
   }

   return 0;
}

static bool sema_contains_main( Defs pDefs )
{
   if( !pDefs )
      return false;

   switch( pDefs->kind )
   {
   case Defs_::is_ListDefSingle:
   {
      Def pDef = pDefs->u.listDefSingle_.def_;
      if( !pDef )
         return false;
      if( pDef->kind == Def_::is_DefFun )
         return sema_is_main_ident( pDef->u.defFun_.functiondef_->u.functionDefMain_.ident_ );
      if( pDef->kind == Def_::is_DefProc )
         return sema_is_main_ident( pDef->u.defProc_.proceduredef_->u.procedureDefMain_.ident_ );
      return false;
   }
   case Defs_::is_ListDefCons:
      if( sema_contains_main( pDefs->u.listDefCons_.defs_ ) )
         return true;
      if( pDefs->u.listDefCons_.def_ )
      {
         Def pDef = pDefs->u.listDefCons_.def_;
         if( pDef->kind == Def_::is_DefFun )
            return sema_is_main_ident( pDef->u.defFun_.functiondef_->u.functionDefMain_.ident_ );
         if( pDef->kind == Def_::is_DefProc )
            return sema_is_main_ident( pDef->u.defProc_.proceduredef_->u.procedureDefMain_.ident_ );
      }
      return false;
   }

   return false;
}

bool sema_ValidateAst( Program pRoot )
{
   if( !pRoot || pRoot->kind != Program_::is_ProgramRoot )
      return false;

   if( !sema_check_topdecls( pRoot->u.programRoot_.topdecls_ ) )
      return false;

   if( !sema_check_defs( pRoot->u.programRoot_.defs_ ) )
      return false;

   if( !sema_contains_main( pRoot->u.programRoot_.defs_ ) )
   {
      sema_ReportError( "Brak funkcji/procedury main()", sema_get_first_def_line( pRoot->u.programRoot_.defs_ ) );
      return false;
   }

   return true;
}

void sema_ReportError( const char *szMessage, int nLine )
{
   fprintf( stderr, "Semantic error at line %d: %s\n", nLine, szMessage );
}

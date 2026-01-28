/*
 * ast.cpp - Minimal helpers for AST inspection
 *
 * Dumps the program structure (declarations, definitions, statement lists).
 * Used mainly for parser debugging, without modifying the tree.
 */

#include "ast.hpp"

#include <stdio.h>

static void ast_dump_stmt( Stmt pStmt, int nDepth );
static void ast_dump_stmtlist( StmtList pList, int nDepth );
static void ast_dump_stmtlistnonempty( StmtListNonEmpty pList, int nDepth );
static void ast_print_param_listopt( ParamListOpt pOpt );
static void ast_print_param_listtail( ParamListTail pTail );
static void ast_print_lhs( LHS pLhs );
static void ast_print_lvalue( LValue pLVal );

static void ast_indent( int nDepth )
{
   for( int i = 0; i < nDepth; ++i )
      fputs( "  ", stdout );
}

static const char *ast_ident_or_empty( Ident szIdent )
{
   return szIdent ? szIdent : "";
}

static const char *ast_enum_name( EnumDecl pEnum )
{
   if( !pEnum )
      return "";

   if( pEnum->kind == EnumDecl_::is_EnumDeclNamed )
      return ast_ident_or_empty( pEnum->u.enumDeclNamed_.ident_ );

   return "";
}

static void ast_dump_enum_item( EnumItem pItem, int nDepth )
{
   if( !pItem || pItem->kind != EnumItem_::is_EnumItemDecl )
      return;

   ast_indent( nDepth );
   fputs( "EnumItem: ", stdout );
   fputs( ast_ident_or_empty( pItem->u.enumItemDecl_.ident_ ), stdout );

   EnumValueOpt pVal = pItem->u.enumItemDecl_.enumvalueopt_;
   if( pVal && pVal->kind == EnumValueOpt_::is_OptEnumValueSome )
   {
      fputs( " = ", stdout );
      fputs( ast_ident_or_empty( pVal->u.optEnumValueSome_.intlit_ ), stdout );
   }
   fputc( '\n', stdout );
}

static void ast_dump_enum_listopt( EnumListOpt pList, int nDepth )
{
   if( !pList || pList->kind == EnumListOpt_::is_OptEnumListEmpty )
      return;

   if( pList->kind == EnumListOpt_::is_OptEnumListSome )
   {
      ast_dump_enum_item( pList->u.optEnumListSome_.enumitem_, nDepth );
      EnumListTail pTail = pList->u.optEnumListSome_.enumlisttail_;
      while( pTail )
      {
         if( pTail->kind == EnumListTail_::is_ListEnumTailCons )
         {
            ast_dump_enum_item( pTail->u.listEnumTailCons_.enumitem_, nDepth );
            pTail = pTail->u.listEnumTailCons_.enumlisttail_;
            continue;
         }
         if( pTail->kind == EnumListTail_::is_ListEnumTailSep )
         {
            pTail = nullptr;
            continue;
         }
         break;
      }
   }
}

static const char *ast_class_name( ClassDecl pClass )
{
   if( !pClass )
      return "";

   if( pClass->kind == ClassDecl_::is_ClassDeclMain )
   {
      ClassName pName = pClass->u.classDeclMain_.classname_;
      if( pName && pName->kind == ClassName_::is_ClassNameIdent )
         return ast_ident_or_empty( pName->u.classNameIdent_.ident_ );
   }

   return "";
}

static void ast_dump_topdecl( TopDecl pTopDecl, int nDepth )
{
   if( !pTopDecl )
      return;

   switch( pTopDecl->kind )
   {
   case TopDecl_::is_TopDeclEnum:
      ast_indent( nDepth );
      printf( "EnumDecl: %s\n", ast_enum_name( pTopDecl->u.topDeclEnum_.enumdecl_ ) );
      if( pTopDecl->u.topDeclEnum_.enumdecl_ )
      {
         EnumDecl pEnum = pTopDecl->u.topDeclEnum_.enumdecl_;
         EnumListOpt pList = nullptr;
         if( pEnum->kind == EnumDecl_::is_EnumDeclNamed )
            pList = pEnum->u.enumDeclNamed_.enumlistopt_;
         else if( pEnum->kind == EnumDecl_::is_EnumDeclAnon )
            pList = pEnum->u.enumDeclAnon_.enumlistopt_;
         ast_dump_enum_listopt( pList, nDepth + 1 );
      }
      break;
   case TopDecl_::is_TopDeclClass:
      ast_indent( nDepth );
      printf( "ClassDecl: %s\n", ast_class_name( pTopDecl->u.topDeclClass_.classdecl_ ) );
      break;
   case TopDecl_::is_TopDeclStruct:
      ast_indent( nDepth );
      puts( "StructDecl" );
      break;
   case TopDecl_::is_TopDeclPreproc:
      ast_indent( nDepth );
      puts( "Preproc" );
      break;
   case TopDecl_::is_TopDeclStatic:
      ast_indent( nDepth );
      puts( "StaticDecl" );
      break;
   }
}

static void ast_dump_topdecls( TopDecls pTopDecls, int nDepth )
{
   if( !pTopDecls )
      return;

   switch( pTopDecls->kind )
   {
   case TopDecls_::is_ListTopDeclEmpty:
      ast_indent( nDepth );
      puts( "TopDecls: <empty>" );
      break;
   case TopDecls_::is_ListTopDeclLine:
      ast_dump_topdecls( pTopDecls->u.listTopDeclLine_.topdecls_, nDepth );
      break;
   case TopDecls_::is_ListTopDeclCons:
      ast_dump_topdecl( pTopDecls->u.listTopDeclCons_.topdecl_, nDepth );
      ast_dump_topdecls( pTopDecls->u.listTopDeclCons_.topdecls_, nDepth );
      break;
   }
}

static void ast_dump_def( Def pDef, int nDepth )
{
   if( !pDef )
      return;

   switch( pDef->kind )
   {
   case Def_::is_DefFun:
      ast_indent( nDepth );
      fputs( "Function: ", stdout );
      fputs( ast_ident_or_empty( pDef->u.defFun_.functiondef_->u.functionDefMain_.ident_ ), stdout );
      ast_print_param_listopt( pDef->u.defFun_.functiondef_->u.functionDefMain_.paramlistopt_ );
      fputc( '\n', stdout );
      ast_dump_stmtlistnonempty( pDef->u.defFun_.functiondef_->u.functionDefMain_.stmtlistnonempty_, nDepth + 1 );
      break;
   case Def_::is_DefProc:
      ast_indent( nDepth );
      fputs( "Procedure: ", stdout );
      fputs( ast_ident_or_empty( pDef->u.defProc_.proceduredef_->u.procedureDefMain_.ident_ ), stdout );
      ast_print_param_listopt( pDef->u.defProc_.proceduredef_->u.procedureDefMain_.paramlistopt_ );
      fputc( '\n', stdout );
      ast_dump_stmtlistnonempty( pDef->u.defProc_.proceduredef_->u.procedureDefMain_.stmtlistnonempty_, nDepth + 1 );
      break;
   case Def_::is_DefMeth:
   {
      MethodDef   pMeth   = pDef->u.defMeth_.methoddef_;
      const char *szClass = "";
      if( pMeth && pMeth->kind == MethodDef_::is_MethodDefMain )
      {
         ClassName pName = pMeth->u.methodDefMain_.classname_;
         if( pName && pName->kind == ClassName_::is_ClassNameIdent )
            szClass = ast_ident_or_empty( pName->u.classNameIdent_.ident_ );
      }
      ast_indent( nDepth );
      fputs( "Method: ", stdout );
      fputs( szClass, stdout );
      fputc( '.', stdout );
      fputs( ast_ident_or_empty( pDef->u.defMeth_.methoddef_->u.methodDefMain_.ident_ ), stdout );
      ast_print_param_listopt( pDef->u.defMeth_.methoddef_->u.methodDefMain_.paramlistopt_ );
      fputc( '\n', stdout );
      ast_dump_stmtlistnonempty( pDef->u.defMeth_.methoddef_->u.methodDefMain_.stmtlistnonempty_, nDepth + 1 );
      break;
   }
   }
}

static void ast_dump_defs( Defs pDefs, int nDepth )
{
   if( !pDefs )
      return;

   switch( pDefs->kind )
   {
   case Defs_::is_ListDefSingle:
      ast_dump_def( pDefs->u.listDefSingle_.def_, nDepth );
      break;
   case Defs_::is_ListDefCons:
      ast_dump_def( pDefs->u.listDefCons_.def_, nDepth );
      ast_dump_defs( pDefs->u.listDefCons_.defs_, nDepth );
      break;
   }
}

static void ast_print_qualifiedid( QualifiedId pQId )
{
   if( !pQId )
      return;

   if( pQId->kind == QualifiedId_::is_QualifiedIdSingle )
   {
      fputs( ast_ident_or_empty( pQId->u.qualifiedIdSingle_.ident_ ), stdout );
      return;
   }

   if( pQId->kind == QualifiedId_::is_QualifiedIdMore )
   {
      fputs( ast_ident_or_empty( pQId->u.qualifiedIdMore_.ident_ ), stdout );
      fputc( '.', stdout );
      ast_print_qualifiedid( pQId->u.qualifiedIdMore_.qualifiedid_ );
   }
}

static void ast_print_lvalue( LValue pLVal )
{
   if( !pLVal )
      return;

   switch( pLVal->kind )
   {
   case LValue_::is_LValQId:
      ast_print_qualifiedid( pLVal->u.lValQId_.qualifiedid_ );
      break;
   case LValue_::is_LValField:
      ast_print_lvalue( pLVal->u.lValField_.lvalue_ );
      fputc( ':', stdout );
      fputs( ast_ident_or_empty( pLVal->u.lValField_.ident_ ), stdout );
      break;
   case LValue_::is_LValIndex:
      ast_print_lvalue( pLVal->u.lValIndex_.lvalue_ );
      fputs( "[...]", stdout );
      break;
   }
}

static void ast_print_lhs( LHS pLhs )
{
   if( !pLhs )
      return;

   switch( pLhs->kind )
   {
   case LHS_::is_LHSVal:
      ast_print_lvalue( pLhs->u.lHSVal_.lvalue_ );
      break;
   case LHS_::is_LHSDeref:
      fputs( "*(...)", stdout );
      break;
   }
}

static void ast_print_param_listopt( ParamListOpt pOpt )
{
   if( !pOpt || pOpt->kind == ParamListOpt_::is_OptParamListEmpty )
      return;

   fputc( '(', stdout );
   fputs( ast_ident_or_empty( pOpt->u.optParamListSome_.ident_ ), stdout );
   ast_print_param_listtail( pOpt->u.optParamListSome_.paramlisttail_ );
   fputc( ')', stdout );
}

static void ast_print_param_listtail( ParamListTail pTail )
{
   while( pTail && pTail->kind == ParamListTail_::is_ListParamTailCons )
   {
      fputs( ", ", stdout );
      fputs( ast_ident_or_empty( pTail->u.listParamTailCons_.ident_ ), stdout );
      pTail = pTail->u.listParamTailCons_.paramlisttail_;
   }
}

static void ast_dump_stmtlistnonempty( StmtListNonEmpty pList, int nDepth )
{
   if( !pList )
      return;

   if( pList->kind == StmtListNonEmpty_::is_ListStmtNonEmpty )
   {
      ast_dump_stmt( pList->u.listStmtNonEmpty_.stmt_, nDepth );
      ast_dump_stmtlist( pList->u.listStmtNonEmpty_.stmtlist_, nDepth );
   }
}

static void ast_dump_stmtlist( StmtList pList, int nDepth )
{
   if( !pList )
      return;

   if( pList->kind == StmtList_::is_ListStmtEmpty )
      return;

   if( pList->kind == StmtList_::is_ListStmtCons )
   {
      ast_dump_stmt( pList->u.listStmtCons_.stmt_, nDepth );
      ast_dump_stmtlist( pList->u.listStmtCons_.stmtlist_, nDepth );
   }
   else if( pList->kind == StmtList_::is_ListStmtLine )
   {
      ast_dump_stmtlist( pList->u.listStmtLine_.stmtlist_, nDepth );
   }
}

static void ast_dump_stmt( Stmt pStmt, int nDepth )
{
   if( !pStmt )
      return;

   ast_indent( nDepth );

   switch( pStmt->kind )
   {
   case Stmt_::is_StmtAssignInl:
      fputs( "Assign := ", stdout );
      ast_print_lhs( pStmt->u.stmtAssignInl_.lhs_ );
      fputc( '\n', stdout );
      break;
   case Stmt_::is_StmtAssignAdd:
      fputs( "Assign += ", stdout );
      ast_print_lhs( pStmt->u.stmtAssignAdd_.lhs_ );
      fputc( '\n', stdout );
      break;
   case Stmt_::is_StmtAssignSub:
      fputs( "Assign -= ", stdout );
      ast_print_lhs( pStmt->u.stmtAssignSub_.lhs_ );
      fputc( '\n', stdout );
      break;
   case Stmt_::is_StmtAssignMul:
      fputs( "Assign *= ", stdout );
      ast_print_lhs( pStmt->u.stmtAssignMul_.lhs_ );
      fputc( '\n', stdout );
      break;
   case Stmt_::is_StmtAssignDiv:
      fputs( "Assign /= ", stdout );
      ast_print_lhs( pStmt->u.stmtAssignDiv_.lhs_ );
      fputc( '\n', stdout );
      break;
   case Stmt_::is_StmtAssignMod:
      fputs( "Assign %= ", stdout );
      ast_print_lhs( pStmt->u.stmtAssignMod_.lhs_ );
      fputc( '\n', stdout );
      break;
   case Stmt_::is_StmtAssignPow:
      fputs( "Assign ^= ", stdout );
      ast_print_lhs( pStmt->u.stmtAssignPow_.lhs_ );
      fputc( '\n', stdout );
      break;
   case Stmt_::is_StmtAssignPow2:
      fputs( "Assign **= ", stdout );
      ast_print_lhs( pStmt->u.stmtAssignPow2_.lhs_ );
      fputc( '\n', stdout );
      break;
   case Stmt_::is_StmtCall:
      puts( "Call" );
      break;
   case Stmt_::is_StmtIf:
      puts( "If" );
      break;
   case Stmt_::is_StmtDoWhile:
      puts( "DoWhile" );
      break;
   case Stmt_::is_StmtFor:
      puts( "For" );
      break;
   case Stmt_::is_StmtForEach:
      puts( "ForEach" );
      break;
   case Stmt_::is_StmtSwitch:
      puts( "Switch" );
      break;
   case Stmt_::is_StmtDoCase:
      puts( "DoCase" );
      break;
   case Stmt_::is_StmtSequence:
      puts( "Sequence" );
      break;
   case Stmt_::is_StmtBreak:
      puts( "Break" );
      break;
   case Stmt_::is_StmtExit:
      puts( "Exit" );
      break;
   case Stmt_::is_StmtLoop:
      puts( "Loop" );
      break;
   case Stmt_::is_StmtReturn:
      puts( "Return" );
      break;
   default:
      puts( "Stmt: <unknown>" );
      break;
   }
}

/* ----------------------------------------------------------------------------
   ast_DumpProgram
   ------------------------------------------------------------------------- */
void ast_DumpProgram( Program p )
{
   if( !p )
   {
      puts( "<null Program>" );
      return;
   }

   if( p->kind != Program_::is_ProgramRoot )
   {
      puts( "<unknown Program>" );
      return;
   }

   puts( "Program" );
   ast_dump_topdecls( p->u.programRoot_.topdecls_, 1 );
   ast_dump_defs( p->u.programRoot_.defs_, 1 );
}

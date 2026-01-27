/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the variable and function names.  */
#define yyparse dory_parse
#define yylex   dory_lex
#define yyerror dory_error
#define yydebug dory_debug
#define yynerrs dory_nerrs

/* First part of user prologue.  */
#line 20 "dory.y"

/* Begin C preamble code */

#include "Absyn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YYMAXDEPTH 10000000

/* The type yyscan_t is defined by flex, but we need it in the parser already. */
#ifndef YY_TYPEDEF_YY_SCANNER_T
#   define YY_TYPEDEF_YY_SCANNER_T
typedef void *yyscan_t;
#endif

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern YY_BUFFER_STATE          dory__scan_string( const char *str, yyscan_t scanner );
extern void                     dory__delete_buffer( YY_BUFFER_STATE buf, yyscan_t scanner );

extern void  dory_lex_destroy( yyscan_t scanner );
extern char *dory_get_text( yyscan_t scanner );

extern yyscan_t dory__initialize_lexer( FILE *inp );

/* List reversal functions. */

/* End C preamble code */

#line 106 "Parser.c"

#ifndef YY_CAST
#   ifdef __cplusplus
#      define YY_CAST( Type, Val )             static_cast<Type>( Val )
#      define YY_REINTERPRET_CAST( Type, Val ) reinterpret_cast<Type>( Val )
#   else
#      define YY_CAST( Type, Val )             ( ( Type ) ( Val ) )
#      define YY_REINTERPRET_CAST( Type, Val ) ( ( Type ) ( Val ) )
#   endif
#endif
#ifndef YY_NULLPTR
#   if defined __cplusplus
#      if 201103L <= __cplusplus
#         define YY_NULLPTR nullptr
#      else
#         define YY_NULLPTR 0
#      endif
#   else
#      define YY_NULLPTR ( ( void * ) 0 )
#   endif
#endif

#include "Bison.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
   YYSYMBOL_YYEMPTY            = -2,
   YYSYMBOL_YYEOF              = 0,   /* "end of file"  */
   YYSYMBOL_YYerror            = 1,   /* error  */
   YYSYMBOL_YYUNDEF            = 2,   /* "invalid token"  */
   YYSYMBOL__ERROR_            = 3,   /* _ERROR_  */
   YYSYMBOL__BANGEQ            = 4,   /* _BANGEQ  */
   YYSYMBOL__HASH              = 5,   /* _HASH  */
   YYSYMBOL__DOLLAR            = 6,   /* _DOLLAR  */
   YYSYMBOL__PERCENT           = 7,   /* _PERCENT  */
   YYSYMBOL__PERCENTEQ         = 8,   /* _PERCENTEQ  */
   YYSYMBOL__AMP               = 9,   /* _AMP  */
   YYSYMBOL__LPAREN            = 10,  /* _LPAREN  */
   YYSYMBOL__RPAREN            = 11,  /* _RPAREN  */
   YYSYMBOL__STAR              = 12,  /* _STAR  */
   YYSYMBOL__DSTAR             = 13,  /* _DSTAR  */
   YYSYMBOL__SYMB_8            = 14,  /* _SYMB_8  */
   YYSYMBOL__STAREQ            = 15,  /* _STAREQ  */
   YYSYMBOL__PLUS              = 16,  /* _PLUS  */
   YYSYMBOL__DPLUS             = 17,  /* _DPLUS  */
   YYSYMBOL__PLUSEQ            = 18,  /* _PLUSEQ  */
   YYSYMBOL__COMMA             = 19,  /* _COMMA  */
   YYSYMBOL__MINUS             = 20,  /* _MINUS  */
   YYSYMBOL__DMINUS            = 21,  /* _DMINUS  */
   YYSYMBOL__MINUSEQ           = 22,  /* _MINUSEQ  */
   YYSYMBOL__DOT               = 23,  /* _DOT  */
   YYSYMBOL__SYMB_16           = 24,  /* _SYMB_16  */
   YYSYMBOL__SYMB_46           = 25,  /* _SYMB_46  */
   YYSYMBOL__SYMB_17           = 26,  /* _SYMB_17  */
   YYSYMBOL__SYMB_15           = 27,  /* _SYMB_15  */
   YYSYMBOL__SYMB_45           = 28,  /* _SYMB_45  */
   YYSYMBOL__SLASH             = 29,  /* _SLASH  */
   YYSYMBOL__SLASHEQ           = 30,  /* _SLASHEQ  */
   YYSYMBOL__COLON             = 31,  /* _COLON  */
   YYSYMBOL__COLONEQ           = 32,  /* _COLONEQ  */
   YYSYMBOL__LT                = 33,  /* _LT  */
   YYSYMBOL__LDARROW           = 34,  /* _LDARROW  */
   YYSYMBOL__LTGT              = 35,  /* _LTGT  */
   YYSYMBOL__EQ                = 36,  /* _EQ  */
   YYSYMBOL__DEQ               = 37,  /* _DEQ  */
   YYSYMBOL__GT                = 38,  /* _GT  */
   YYSYMBOL__GTEQ              = 39,  /* _GTEQ  */
   YYSYMBOL__LBRACK            = 40,  /* _LBRACK  */
   YYSYMBOL__RBRACK            = 41,  /* _RBRACK  */
   YYSYMBOL__CARET             = 42,  /* _CARET  */
   YYSYMBOL__CARETEQ           = 43,  /* _CARETEQ  */
   YYSYMBOL__KW_begin          = 44,  /* _KW_begin  */
   YYSYMBOL__KW_break          = 45,  /* _KW_break  */
   YYSYMBOL__KW_case           = 46,  /* _KW_case  */
   YYSYMBOL__KW_class          = 47,  /* _KW_class  */
   YYSYMBOL__KW_create         = 48,  /* _KW_create  */
   YYSYMBOL__KW_descend        = 49,  /* _KW_descend  */
   YYSYMBOL__KW_do             = 50,  /* _KW_do  */
   YYSYMBOL__KW_each           = 51,  /* _KW_each  */
   YYSYMBOL__KW_else           = 52,  /* _KW_else  */
   YYSYMBOL__KW_elseif         = 53,  /* _KW_elseif  */
   YYSYMBOL__KW_end            = 54,  /* _KW_end  */
   YYSYMBOL__KW_endcase        = 55,  /* _KW_endcase  */
   YYSYMBOL__KW_endclass       = 56,  /* _KW_endclass  */
   YYSYMBOL__KW_enddo          = 57,  /* _KW_enddo  */
   YYSYMBOL__KW_endenum        = 58,  /* _KW_endenum  */
   YYSYMBOL__KW_endfor         = 59,  /* _KW_endfor  */
   YYSYMBOL__KW_endif          = 60,  /* _KW_endif  */
   YYSYMBOL__KW_endsequence    = 61,  /* _KW_endsequence  */
   YYSYMBOL__KW_endstruct      = 62,  /* _KW_endstruct  */
   YYSYMBOL__KW_endswitch      = 63,  /* _KW_endswitch  */
   YYSYMBOL__KW_endwhile       = 64,  /* _KW_endwhile  */
   YYSYMBOL__KW_enum           = 65,  /* _KW_enum  */
   YYSYMBOL__KW_exit           = 66,  /* _KW_exit  */
   YYSYMBOL__SYMB_9            = 67,  /* _SYMB_9  */
   YYSYMBOL__KW_for            = 68,  /* _KW_for  */
   YYSYMBOL__KW_from           = 69,  /* _KW_from  */
   YYSYMBOL__KW_function       = 70,  /* _KW_function  */
   YYSYMBOL__SYMB_11           = 71,  /* _SYMB_11  */
   YYSYMBOL__KW_if             = 72,  /* _KW_if  */
   YYSYMBOL__KW_in             = 73,  /* _KW_in  */
   YYSYMBOL__KW_inherit        = 74,  /* _KW_inherit  */
   YYSYMBOL__KW_local          = 75,  /* _KW_local  */
   YYSYMBOL__KW_loop           = 76,  /* _KW_loop  */
   YYSYMBOL__KW_method         = 77,  /* _KW_method  */
   YYSYMBOL__KW_nil            = 78,  /* _KW_nil  */
   YYSYMBOL__KW_otherwise      = 79,  /* _KW_otherwise  */
   YYSYMBOL__KW_procedure      = 80,  /* _KW_procedure  */
   YYSYMBOL__SYMB_12           = 81,  /* _SYMB_12  */
   YYSYMBOL__KW_recover        = 82,  /* _KW_recover  */
   YYSYMBOL__KW_return         = 83,  /* _KW_return  */
   YYSYMBOL__KW_sequence       = 84,  /* _KW_sequence  */
   YYSYMBOL__KW_static         = 85,  /* _KW_static  */
   YYSYMBOL__KW_step           = 86,  /* _KW_step  */
   YYSYMBOL__KW_struct         = 87,  /* _KW_struct  */
   YYSYMBOL__KW_switch         = 88,  /* _KW_switch  */
   YYSYMBOL__KW_to             = 89,  /* _KW_to  */
   YYSYMBOL__KW_using          = 90,  /* _KW_using  */
   YYSYMBOL__KW_var            = 91,  /* _KW_var  */
   YYSYMBOL__SYMB_10           = 92,  /* _SYMB_10  */
   YYSYMBOL__KW_while          = 93,  /* _KW_while  */
   YYSYMBOL__LBRACE            = 94,  /* _LBRACE  */
   YYSYMBOL__BAR               = 95,  /* _BAR  */
   YYSYMBOL__RBRACE            = 96,  /* _RBRACE  */
   YYSYMBOL_T_FloatLit         = 97,  /* T_FloatLit  */
   YYSYMBOL_T_IntLit           = 98,  /* T_IntLit  */
   YYSYMBOL_T_LineEol          = 99,  /* T_LineEol  */
   YYSYMBOL_T_PreprocLine      = 100, /* T_PreprocLine  */
   YYSYMBOL_T_StrLit           = 101, /* T_StrLit  */
   YYSYMBOL__IDENT_            = 102, /* _IDENT_  */
   YYSYMBOL_YYACCEPT           = 103, /* $accept  */
   YYSYMBOL_Program            = 104, /* Program  */
   YYSYMBOL_LeadingEols        = 105, /* LeadingEols  */
   YYSYMBOL_TopDecls           = 106, /* TopDecls  */
   YYSYMBOL_TopDecl            = 107, /* TopDecl  */
   YYSYMBOL_Defs               = 108, /* Defs  */
   YYSYMBOL_PreprocDecl        = 109, /* PreprocDecl  */
   YYSYMBOL_ParamListOpt       = 110, /* ParamListOpt  */
   YYSYMBOL_ParamListTail      = 111, /* ParamListTail  */
   YYSYMBOL_Stmt               = 112, /* Stmt  */
   YYSYMBOL_ReturnExprOpt      = 113, /* ReturnExprOpt  */
   YYSYMBOL_StmtList           = 114, /* StmtList  */
   YYSYMBOL_StmtListNonEmpty   = 115, /* StmtListNonEmpty  */
   YYSYMBOL_ElseIfList         = 116, /* ElseIfList  */
   YYSYMBOL_ElseIf             = 117, /* ElseIf  */
   YYSYMBOL_ElseOpt            = 118, /* ElseOpt  */
   YYSYMBOL_IfCond             = 119, /* IfCond  */
   YYSYMBOL_EndIf              = 120, /* EndIf  */
   YYSYMBOL_EndIfOpt           = 121, /* EndIfOpt  */
   YYSYMBOL_DoOpt              = 122, /* DoOpt  */
   YYSYMBOL_WhileCond          = 123, /* WhileCond  */
   YYSYMBOL_EndDo              = 124, /* EndDo  */
   YYSYMBOL_EndDoOpt           = 125, /* EndDoOpt  */
   YYSYMBOL_EndCase            = 126, /* EndCase  */
   YYSYMBOL_EndCaseOpt         = 127, /* EndCaseOpt  */
   YYSYMBOL_StepOpt            = 128, /* StepOpt  */
   YYSYMBOL_EndFor             = 129, /* EndFor  */
   YYSYMBOL_EndForOpt          = 130, /* EndForOpt  */
   YYSYMBOL_ForEachVars        = 131, /* ForEachVars  */
   YYSYMBOL_ForEachVarsTail    = 132, /* ForEachVarsTail  */
   YYSYMBOL_ForEachExprs       = 133, /* ForEachExprs  */
   YYSYMBOL_ForEachExprsTail   = 134, /* ForEachExprsTail  */
   YYSYMBOL_ForEachDescOpt     = 135, /* ForEachDescOpt  */
   YYSYMBOL_CaseList           = 136, /* CaseList  */
   YYSYMBOL_CaseClause         = 137, /* CaseClause  */
   YYSYMBOL_CaseCondList       = 138, /* CaseCondList  */
   YYSYMBOL_CaseCondClause     = 139, /* CaseCondClause  */
   YYSYMBOL_OtherwiseOpt       = 140, /* OtherwiseOpt  */
   YYSYMBOL_BreakExprOpt       = 141, /* BreakExprOpt  */
   YYSYMBOL_RecoverOpt         = 142, /* RecoverOpt  */
   YYSYMBOL_RecoverUsingOpt    = 143, /* RecoverUsingOpt  */
   YYSYMBOL_EndSequence        = 144, /* EndSequence  */
   YYSYMBOL_EndSequenceOpt     = 145, /* EndSequenceOpt  */
   YYSYMBOL_SwitchCond         = 146, /* SwitchCond  */
   YYSYMBOL_EndSwitch          = 147, /* EndSwitch  */
   YYSYMBOL_EndSwitchOpt       = 148, /* EndSwitchOpt  */
   YYSYMBOL_EndStruct          = 149, /* EndStruct  */
   YYSYMBOL_EndStructOpt       = 150, /* EndStructOpt  */
   YYSYMBOL_EndClass           = 151, /* EndClass  */
   YYSYMBOL_EndClassOpt        = 152, /* EndClassOpt  */
   YYSYMBOL_EndEnum            = 153, /* EndEnum  */
   YYSYMBOL_EndEnumOpt         = 154, /* EndEnumOpt  */
   YYSYMBOL_CaseValue          = 155, /* CaseValue  */
   YYSYMBOL_StmtEnd            = 156, /* StmtEnd  */
   YYSYMBOL_EnumDecl           = 157, /* EnumDecl  */
   YYSYMBOL_EnumListOpt        = 158, /* EnumListOpt  */
   YYSYMBOL_EnumListTail       = 159, /* EnumListTail  */
   YYSYMBOL_EnumSep            = 160, /* EnumSep  */
   YYSYMBOL_EnumItem           = 161, /* EnumItem  */
   YYSYMBOL_EnumValueOpt       = 162, /* EnumValueOpt  */
   YYSYMBOL_LineEolOpt         = 163, /* LineEolOpt  */
   YYSYMBOL_StructDecl         = 164, /* StructDecl  */
   YYSYMBOL_StructBody         = 165, /* StructBody  */
   YYSYMBOL_StructMember       = 166, /* StructMember  */
   YYSYMBOL_FieldInitList      = 167, /* FieldInitList  */
   YYSYMBOL_FieldInit          = 168, /* FieldInit  */
   YYSYMBOL_StaticDecl         = 169, /* StaticDecl  */
   YYSYMBOL_ClassDecl          = 170, /* ClassDecl  */
   YYSYMBOL_CreateOpt          = 171, /* CreateOpt  */
   YYSYMBOL_ClassName          = 172, /* ClassName  */
   YYSYMBOL_InheritOpt         = 173, /* InheritOpt  */
   YYSYMBOL_InheritKeyword     = 174, /* InheritKeyword  */
   YYSYMBOL_StaticClassOpt     = 175, /* StaticClassOpt  */
   YYSYMBOL_ClassBody          = 176, /* ClassBody  */
   YYSYMBOL_ClassMember        = 177, /* ClassMember  */
   YYSYMBOL_VisibilityScope    = 178, /* VisibilityScope  */
   YYSYMBOL_VarList            = 179, /* VarList  */
   YYSYMBOL_VarDecl            = 180, /* VarDecl  */
   YYSYMBOL_VarInitOpt         = 181, /* VarInitOpt  */
   YYSYMBOL_MethodParamsOpt    = 182, /* MethodParamsOpt  */
   YYSYMBOL_Expr               = 183, /* Expr  */
   YYSYMBOL_ExprOr             = 184, /* ExprOr  */
   YYSYMBOL_Expr1              = 185, /* Expr1  */
   YYSYMBOL_Expr2              = 186, /* Expr2  */
   YYSYMBOL_Expr3              = 187, /* Expr3  */
   YYSYMBOL_Expr4              = 188, /* Expr4  */
   YYSYMBOL_Expr5              = 189, /* Expr5  */
   YYSYMBOL_Expr6              = 190, /* Expr6  */
   YYSYMBOL_Expr7              = 191, /* Expr7  */
   YYSYMBOL_Expr8              = 192, /* Expr8  */
   YYSYMBOL_CallExpr           = 193, /* CallExpr  */
   YYSYMBOL_ArgListOpt         = 194, /* ArgListOpt  */
   YYSYMBOL_ArgListTail        = 195, /* ArgListTail  */
   YYSYMBOL_ExprListOpt        = 196, /* ExprListOpt  */
   YYSYMBOL_ExprListTail       = 197, /* ExprListTail  */
   YYSYMBOL_QualifiedId        = 198, /* QualifiedId  */
   YYSYMBOL_LHS                = 199, /* LHS  */
   YYSYMBOL_LValue             = 200, /* LValue  */
   YYSYMBOL_SubscriptList      = 201, /* SubscriptList  */
   YYSYMBOL_SubscriptListTail  = 202, /* SubscriptListTail  */
   YYSYMBOL_Literal            = 203, /* Literal  */
   YYSYMBOL_Napis              = 204, /* Napis  */
   YYSYMBOL_Def                = 205, /* Def  */
   YYSYMBOL_FunctionDef        = 206, /* FunctionDef  */
   YYSYMBOL_ProcedureDef       = 207, /* ProcedureDef  */
   YYSYMBOL_StaticOpt          = 208, /* StaticOpt  */
   YYSYMBOL_LocalDeclsOpt      = 209, /* LocalDeclsOpt  */
   YYSYMBOL_LocalDecl          = 210, /* LocalDecl  */
   YYSYMBOL_StaticDeclsOpt     = 211, /* StaticDeclsOpt  */
   YYSYMBOL_StaticItemList     = 212, /* StaticItemList  */
   YYSYMBOL_StaticItemTail     = 213, /* StaticItemTail  */
   YYSYMBOL_StaticItem         = 214, /* StaticItem  */
   YYSYMBOL_StaticArrayDimsOpt = 215, /* StaticArrayDimsOpt  */
   YYSYMBOL_StaticInitOpt      = 216, /* StaticInitOpt  */
   YYSYMBOL_IdentInitList      = 217, /* IdentInitList  */
   YYSYMBOL_IdentInitTail      = 218, /* IdentInitTail  */
   YYSYMBOL_IdentInit          = 219, /* IdentInit  */
   YYSYMBOL_InitOpt            = 220, /* InitOpt  */
   YYSYMBOL_MethodDef          = 221  /* MethodDef  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;

/* Second part of user prologue.  */
#line 168 "dory.y"

void yyerror( YYLTYPE *loc, yyscan_t scanner, YYSTYPE *result, const char *msg )
{
   fprintf( stderr, "error: %d,%d: %s at %s\n", loc->first_line, loc->first_column, msg, dory_get_text( scanner ) );
}

int yyparse( yyscan_t scanner, YYSTYPE *result );

extern int yylex( YYSTYPE *lvalp, YYLTYPE *llocp, yyscan_t scanner );

#line 373 "Parser.c"

#ifdef short
#   undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
#   include <limits.h> /* INFRINGES ON USER NAME SPACE */
#   if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#      include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#      define YY_STDINT_H
#   endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
#   undef UINT_LEAST8_MAX
#   undef UINT_LEAST16_MAX
#   define UINT_LEAST8_MAX  255
#   define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif ( !defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H && UINT_LEAST8_MAX <= INT_MAX )
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif ( !defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H && UINT_LEAST16_MAX <= INT_MAX )
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
#   if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#      define YYPTRDIFF_T       __PTRDIFF_TYPE__
#      define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
#   elif defined PTRDIFF_MAX
#      ifndef ptrdiff_t
#         include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#      endif
#      define YYPTRDIFF_T       ptrdiff_t
#      define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
#   else
#      define YYPTRDIFF_T       long
#      define YYPTRDIFF_MAXIMUM LONG_MAX
#   endif
#endif

#ifndef YYSIZE_T
#   ifdef __SIZE_TYPE__
#      define YYSIZE_T __SIZE_TYPE__
#   elif defined size_t
#      define YYSIZE_T size_t
#   elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#      include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#      define YYSIZE_T size_t
#   else
#      define YYSIZE_T unsigned
#   endif
#endif

#define YYSIZE_MAXIMUM                                                                                                 \
   YY_CAST( YYPTRDIFF_T, ( YYPTRDIFF_MAXIMUM < YY_CAST( YYSIZE_T, -1 ) ? YYPTRDIFF_MAXIMUM : YY_CAST( YYSIZE_T, -1 ) ) )

#define YYSIZEOF( X ) YY_CAST( YYPTRDIFF_T, sizeof( X ) )

/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
#   if defined YYENABLE_NLS && YYENABLE_NLS
#      if ENABLE_NLS
#         include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#         define YY_( Msgid ) dgettext( "bison-runtime", Msgid )
#      endif
#   endif
#   ifndef YY_
#      define YY_( Msgid ) Msgid
#   endif
#endif

#ifndef YY_ATTRIBUTE_PURE
#   if defined __GNUC__ && 2 < __GNUC__ + ( 96 <= __GNUC_MINOR__ )
#      define YY_ATTRIBUTE_PURE __attribute__( ( __pure__ ) )
#   else
#      define YY_ATTRIBUTE_PURE
#   endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
#   if defined __GNUC__ && 2 < __GNUC__ + ( 7 <= __GNUC_MINOR__ )
#      define YY_ATTRIBUTE_UNUSED __attribute__( ( __unused__ ) )
#   else
#      define YY_ATTRIBUTE_UNUSED
#   endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if !defined lint || defined __GNUC__
#   define YY_USE( E ) ( ( void ) ( E ) )
#else
#   define YY_USE( E ) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && !defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
#   if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#      define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                                                                      \
         _Pragma( "GCC diagnostic push" ) _Pragma( "GCC diagnostic ignored \"-Wuninitialized\"" )
#   else
#      define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                                                                      \
         _Pragma( "GCC diagnostic push" ) _Pragma( "GCC diagnostic ignored \"-Wuninitialized\"" )                      \
             _Pragma( "GCC diagnostic ignored \"-Wmaybe-uninitialized\"" )
#   endif
#   define YY_IGNORE_MAYBE_UNINITIALIZED_END _Pragma( "GCC diagnostic pop" )
#else
#   define YY_INITIAL_VALUE( Value ) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
#   define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
#   define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
#   define YY_INITIAL_VALUE( Value ) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && !defined __ICC && 6 <= __GNUC__
#   define YY_IGNORE_USELESS_CAST_BEGIN                                                                                \
      _Pragma( "GCC diagnostic push" ) _Pragma( "GCC diagnostic ignored \"-Wuseless-cast\"" )
#   define YY_IGNORE_USELESS_CAST_END _Pragma( "GCC diagnostic pop" )
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
#   define YY_IGNORE_USELESS_CAST_BEGIN
#   define YY_IGNORE_USELESS_CAST_END
#endif

#define YY_ASSERT( E ) ( ( void ) ( 0 && ( E ) ) )

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

#   ifdef YYSTACK_USE_ALLOCA
#      if YYSTACK_USE_ALLOCA
#         ifdef __GNUC__
#            define YYSTACK_ALLOC __builtin_alloca
#         elif defined __BUILTIN_VA_ARG_INCR
#            include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#         elif defined _AIX
#            define YYSTACK_ALLOC __alloca
#         elif defined _MSC_VER
#            include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#            define alloca _alloca
#         else
#            define YYSTACK_ALLOC alloca
#            if !defined _ALLOCA_H && !defined EXIT_SUCCESS
#               include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
/* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#               ifndef EXIT_SUCCESS
#                  define EXIT_SUCCESS 0
#               endif
#            endif
#         endif
#      endif
#   endif

#   ifdef YYSTACK_ALLOC
/* Pacify GCC's 'empty if-body' warning.  */
#      define YYSTACK_FREE( Ptr )                                                                                      \
         do                                                                                                            \
         { /* empty */                                                                                                 \
            ;                                                                                                          \
         } while( 0 )
#      ifndef YYSTACK_ALLOC_MAXIMUM
/* The OS might guarantee only one guard page at the bottom of the stack,
   and a page size can be as small as 4096 bytes.  So we cannot safely
   invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
   to allow for a few compiler-allocated temporary stack slots.  */
#         define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#      endif
#   else
#      define YYSTACK_ALLOC YYMALLOC
#      define YYSTACK_FREE  YYFREE
#      ifndef YYSTACK_ALLOC_MAXIMUM
#         define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#      endif
#      if ( defined __cplusplus && !defined EXIT_SUCCESS &&                                                            \
            !( ( defined YYMALLOC || defined malloc ) && ( defined YYFREE || defined free ) ) )
#         include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#         ifndef EXIT_SUCCESS
#            define EXIT_SUCCESS 0
#         endif
#      endif
#      ifndef YYMALLOC
#         define YYMALLOC malloc
#         if !defined malloc && !defined EXIT_SUCCESS
void *malloc( YYSIZE_T ); /* INFRINGES ON USER NAME SPACE */
#         endif
#      endif
#      ifndef YYFREE
#         define YYFREE free
#         if !defined free && !defined EXIT_SUCCESS
void free( void * ); /* INFRINGES ON USER NAME SPACE */
#         endif
#      endif
#   endif
#endif /* !defined yyoverflow */

#if ( !defined yyoverflow && ( !defined __cplusplus || ( defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL &&           \
                                                         defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL ) ) )

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
   yy_state_t yyss_alloc;
   YYSTYPE    yyvs_alloc;
   YYLTYPE    yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
#   define YYSTACK_GAP_MAXIMUM ( YYSIZEOF( union yyalloc ) - 1 )

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
#   define YYSTACK_BYTES( N )                                                                                          \
      ( ( N ) * ( YYSIZEOF( yy_state_t ) + YYSIZEOF( YYSTYPE ) + YYSIZEOF( YYLTYPE ) ) + 2 * YYSTACK_GAP_MAXIMUM )

#   define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
#   define YYSTACK_RELOCATE( Stack_alloc, Stack )                                                                      \
      do                                                                                                               \
      {                                                                                                                \
         YYPTRDIFF_T yynewbytes;                                                                                       \
         YYCOPY( &yyptr->Stack_alloc, Stack, yysize );                                                                 \
         Stack      = &yyptr->Stack_alloc;                                                                             \
         yynewbytes = yystacksize * YYSIZEOF( *Stack ) + YYSTACK_GAP_MAXIMUM;                                          \
         yyptr += yynewbytes / YYSIZEOF( *yyptr );                                                                     \
      } while( 0 )

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
#   ifndef YYCOPY
#      if defined __GNUC__ && 1 < __GNUC__
#         define YYCOPY( Dst, Src, Count )                                                                             \
            __builtin_memcpy( Dst, Src, YY_CAST( YYSIZE_T, ( Count ) ) * sizeof( *( Src ) ) )
#      else
#         define YYCOPY( Dst, Src, Count )                                                                             \
            do                                                                                                         \
            {                                                                                                          \
               YYPTRDIFF_T yyi;                                                                                        \
               for( yyi = 0; yyi < ( Count ); yyi++ )                                                                  \
                  ( Dst )[ yyi ] = ( Src )[ yyi ];                                                                     \
            } while( 0 )
#      endif
#   endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL 3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST 481

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS 103
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS 119
/* YYNRULES -- Number of rules.  */
#define YYNRULES 271
/* YYNSTATES -- Number of states.  */
#define YYNSTATES 499

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK 357

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE( YYX )                                                                                             \
   ( 0 <= ( YYX ) && ( YYX ) <= YYMAXUTOK ? YY_CAST( yysymbol_kind_t, yytranslate[ YYX ] ) : YYSYMBOL_YYUNDEF )

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] = {
    0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,   2,   2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,   2,   2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,   2,   2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,   2,   2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,   2,   2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,   2,   2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,   2,   2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,   2,   2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,   2,   2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16,  17,  18, 19, 20, 21, 22, 23, 24,
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,  45,  46, 47, 48, 49, 50, 51, 52,
    53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72,  73,  74, 75, 76, 77, 78, 79, 80,
    81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102 };

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] = {
    0,   404, 404, 406, 408, 409, 410, 412, 413, 414, 415, 416, 418, 419, 421, 423, 424, 426, 427, 429, 430, 431, 432,
    433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 451, 452, 454, 455, 456, 458,
    460, 461, 463, 465, 466, 468, 470, 471, 473, 474, 476, 477, 479, 481, 482, 483, 485, 486, 487, 489, 490, 492, 493,
    495, 496, 498, 499, 501, 502, 504, 506, 507, 509, 511, 512, 514, 515, 517, 518, 520, 522, 523, 525, 527, 528, 530,
    531, 533, 534, 536, 537, 539, 540, 542, 543, 545, 547, 548, 550, 551, 553, 554, 556, 557, 559, 560, 562, 563, 565,
    566, 568, 569, 571, 572, 574, 576, 577, 579, 580, 582, 583, 584, 586, 587, 589, 591, 592, 594, 595, 597, 599, 600,
    602, 604, 605, 607, 609, 610, 612, 614, 615, 617, 619, 620, 622, 623, 625, 626, 628, 629, 631, 632, 633, 635, 636,
    637, 638, 640, 641, 643, 645, 646, 648, 649, 651, 653, 654, 656, 657, 659, 660, 662, 663, 664, 665, 666, 667, 668,
    669, 670, 671, 672, 674, 675, 676, 678, 679, 680, 681, 683, 684, 685, 687, 688, 689, 690, 691, 692, 693, 695, 696,
    697, 698, 699, 700, 701, 702, 703, 704, 705, 706, 707, 709, 711, 712, 714, 715, 717, 718, 720, 721, 723, 724, 726,
    727, 729, 730, 731, 733, 735, 736, 738, 739, 740, 741, 742, 743, 745, 747, 748, 749, 751, 753, 755, 756, 758, 759,
    760, 762, 764, 765, 767, 769, 770, 772, 774, 775, 777, 778, 780, 782, 783, 785, 787, 788, 790 };
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL( State ) YY_CAST( yysymbol_kind_t, yystos[ State ] )

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name( yysymbol_kind_t yysymbol ) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] = { "\"end of file\"",
                                       "error",
                                       "\"invalid token\"",
                                       "_ERROR_",
                                       "_BANGEQ",
                                       "_HASH",
                                       "_DOLLAR",
                                       "_PERCENT",
                                       "_PERCENTEQ",
                                       "_AMP",
                                       "_LPAREN",
                                       "_RPAREN",
                                       "_STAR",
                                       "_DSTAR",
                                       "_SYMB_8",
                                       "_STAREQ",
                                       "_PLUS",
                                       "_DPLUS",
                                       "_PLUSEQ",
                                       "_COMMA",
                                       "_MINUS",
                                       "_DMINUS",
                                       "_MINUSEQ",
                                       "_DOT",
                                       "_SYMB_16",
                                       "_SYMB_46",
                                       "_SYMB_17",
                                       "_SYMB_15",
                                       "_SYMB_45",
                                       "_SLASH",
                                       "_SLASHEQ",
                                       "_COLON",
                                       "_COLONEQ",
                                       "_LT",
                                       "_LDARROW",
                                       "_LTGT",
                                       "_EQ",
                                       "_DEQ",
                                       "_GT",
                                       "_GTEQ",
                                       "_LBRACK",
                                       "_RBRACK",
                                       "_CARET",
                                       "_CARETEQ",
                                       "_KW_begin",
                                       "_KW_break",
                                       "_KW_case",
                                       "_KW_class",
                                       "_KW_create",
                                       "_KW_descend",
                                       "_KW_do",
                                       "_KW_each",
                                       "_KW_else",
                                       "_KW_elseif",
                                       "_KW_end",
                                       "_KW_endcase",
                                       "_KW_endclass",
                                       "_KW_enddo",
                                       "_KW_endenum",
                                       "_KW_endfor",
                                       "_KW_endif",
                                       "_KW_endsequence",
                                       "_KW_endstruct",
                                       "_KW_endswitch",
                                       "_KW_endwhile",
                                       "_KW_enum",
                                       "_KW_exit",
                                       "_SYMB_9",
                                       "_KW_for",
                                       "_KW_from",
                                       "_KW_function",
                                       "_SYMB_11",
                                       "_KW_if",
                                       "_KW_in",
                                       "_KW_inherit",
                                       "_KW_local",
                                       "_KW_loop",
                                       "_KW_method",
                                       "_KW_nil",
                                       "_KW_otherwise",
                                       "_KW_procedure",
                                       "_SYMB_12",
                                       "_KW_recover",
                                       "_KW_return",
                                       "_KW_sequence",
                                       "_KW_static",
                                       "_KW_step",
                                       "_KW_struct",
                                       "_KW_switch",
                                       "_KW_to",
                                       "_KW_using",
                                       "_KW_var",
                                       "_SYMB_10",
                                       "_KW_while",
                                       "_LBRACE",
                                       "_BAR",
                                       "_RBRACE",
                                       "T_FloatLit",
                                       "T_IntLit",
                                       "T_LineEol",
                                       "T_PreprocLine",
                                       "T_StrLit",
                                       "_IDENT_",
                                       "$accept",
                                       "Program",
                                       "LeadingEols",
                                       "TopDecls",
                                       "TopDecl",
                                       "Defs",
                                       "PreprocDecl",
                                       "ParamListOpt",
                                       "ParamListTail",
                                       "Stmt",
                                       "ReturnExprOpt",
                                       "StmtList",
                                       "StmtListNonEmpty",
                                       "ElseIfList",
                                       "ElseIf",
                                       "ElseOpt",
                                       "IfCond",
                                       "EndIf",
                                       "EndIfOpt",
                                       "DoOpt",
                                       "WhileCond",
                                       "EndDo",
                                       "EndDoOpt",
                                       "EndCase",
                                       "EndCaseOpt",
                                       "StepOpt",
                                       "EndFor",
                                       "EndForOpt",
                                       "ForEachVars",
                                       "ForEachVarsTail",
                                       "ForEachExprs",
                                       "ForEachExprsTail",
                                       "ForEachDescOpt",
                                       "CaseList",
                                       "CaseClause",
                                       "CaseCondList",
                                       "CaseCondClause",
                                       "OtherwiseOpt",
                                       "BreakExprOpt",
                                       "RecoverOpt",
                                       "RecoverUsingOpt",
                                       "EndSequence",
                                       "EndSequenceOpt",
                                       "SwitchCond",
                                       "EndSwitch",
                                       "EndSwitchOpt",
                                       "EndStruct",
                                       "EndStructOpt",
                                       "EndClass",
                                       "EndClassOpt",
                                       "EndEnum",
                                       "EndEnumOpt",
                                       "CaseValue",
                                       "StmtEnd",
                                       "EnumDecl",
                                       "EnumListOpt",
                                       "EnumListTail",
                                       "EnumSep",
                                       "EnumItem",
                                       "EnumValueOpt",
                                       "LineEolOpt",
                                       "StructDecl",
                                       "StructBody",
                                       "StructMember",
                                       "FieldInitList",
                                       "FieldInit",
                                       "StaticDecl",
                                       "ClassDecl",
                                       "CreateOpt",
                                       "ClassName",
                                       "InheritOpt",
                                       "InheritKeyword",
                                       "StaticClassOpt",
                                       "ClassBody",
                                       "ClassMember",
                                       "VisibilityScope",
                                       "VarList",
                                       "VarDecl",
                                       "VarInitOpt",
                                       "MethodParamsOpt",
                                       "Expr",
                                       "ExprOr",
                                       "Expr1",
                                       "Expr2",
                                       "Expr3",
                                       "Expr4",
                                       "Expr5",
                                       "Expr6",
                                       "Expr7",
                                       "Expr8",
                                       "CallExpr",
                                       "ArgListOpt",
                                       "ArgListTail",
                                       "ExprListOpt",
                                       "ExprListTail",
                                       "QualifiedId",
                                       "LHS",
                                       "LValue",
                                       "SubscriptList",
                                       "SubscriptListTail",
                                       "Literal",
                                       "Napis",
                                       "Def",
                                       "FunctionDef",
                                       "ProcedureDef",
                                       "StaticOpt",
                                       "LocalDeclsOpt",
                                       "LocalDecl",
                                       "StaticDeclsOpt",
                                       "StaticItemList",
                                       "StaticItemTail",
                                       "StaticItem",
                                       "StaticArrayDimsOpt",
                                       "StaticInitOpt",
                                       "IdentInitList",
                                       "IdentInitTail",
                                       "IdentInit",
                                       "InitOpt",
                                       "MethodDef",
                                       YY_NULLPTR };

static const char *yysymbol_name( yysymbol_kind_t yysymbol )
{
   return yytname[ yysymbol ];
}
#endif

#define YYPACT_NINF ( -335 )

#define yypact_value_is_default( Yyn ) ( ( Yyn ) == YYPACT_NINF )

#define YYTABLE_NINF ( -146 )

#define yytable_value_is_error( Yyn ) 0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] = {
    -335, 65,   150,  -335, -335, 89,   -61,  -10,  150,  -21,  45,   150,  -335, -335, -335, -335, -335, 94,   5,
    73,   80,   147,  -21,  173,  95,   -335, -335, -335, 111,  111,  111,  -335, 45,   -335, -335, -335, -335, 98,
    179,  -24,  -15,  5,    120,  60,   188,  -335, 129,  -335, 153,  -335, 136,  139,  149,  -335, -335, 49,   160,
    -335, 198,  -335, -21,  176,  -335, -335, 5,    -24,  244,  -21,  258,  -9,   60,   158,  158,  158,  158,  158,
    -335, 60,   -335, -335, 184,  11,   -335, -335, -335, 256,  261,  257,  265,  -335, 282,  157,  107,  31,   -335,
    100,  -335, -4,   249,  -335, -335, 60,   -335, 173,  183,  101,  153,  281,  283,  286,  -335, -335, 207,  98,
    -335, -335, -335, -335, -335, -335, -15,  -21,  60,   -335, -335, 120,  -335, 158,  -335, -335, 108,  288,  -335,
    -335, -335, -335, -335, -335, 201,  204,  289,  213,  208,  60,   -335, 60,   60,   158,  158,  158,  158,  158,
    158,  158,  158,  158,  158,  158,  158,  158,  158,  158,  158,  158,  -335, -335, 209,  60,   60,   201,  147,
    -335, -335, 280,  214,  303,  237,  -335, -21,  -335, 204,  204,  204,  -335, 226,  -335, -335, -335, -335, 258,
    -335, 224,  60,   -335, 295,  234,  309,  312,  239,  60,   -335, -335, -335, 261,  265,  -335, 157,  157,  157,
    157,  157,  157,  157,  157,  157,  157,  107,  107,  31,   31,   31,   -335, -335, 322,  302,  325,  334,  250,
    -335, 60,   -335, -335, 183,  -335, -335, -335, 336,  337,  339,  122,  -335, -335, 310,  60,   -335, 201,  251,
    -335, 60,   289,  -335, 60,   -335, 60,   -335, -335, -335, -335, -335, 253,  317,  266,  -335, -335, 264,  -335,
    183,  -335, -6,   122,  269,  -335, -335, -335, 312,  273,  -335, 359,  325,  -50,  98,   -50,  361,  274,  327,
    -335, -21,  -335, -335, -335, -335, -335, -335, 120,  -50,  290,  -50,  277,  290,  204,  279,  -335, -335, -335,
    -335, -21,  -335, 290,  379,  -335, -50,  379,  368,  -335, -335, -335, 296,  60,   335,  -21,  -27,  60,   -21,
    60,   60,   140,  -335, 291,  -335, -21,  372,  187,  290,  -335, -335, 284,  -21,  -335, 293,  -335, 285,  353,
    294,  -335, -335, -21,  -335, 299,  -335, 140,  140,  -335, 60,   -335, 60,   60,   60,   60,   60,   60,   60,
    60,   379,  140,  -335, 342,  370,  321,  60,   140,  -335, 344,  -335, -335, 304,  -335, -21,  -21,  -21,  -21,
    -21,  -21,  -21,  -21,  -335, 319,  60,   323,  342,  306,  -335, 60,   315,  352,  106,  323,  344,  140,  -335,
    -335, -335, -335, -335, -335, -335, -335, 316,  105,  311,  314,  171,  -335, 370,  360,  392,  60,   60,   362,
    352,  -335, 318,  -335, 90,   -335, 7,    313,  326,  338,  -335, -21,  140,  140,  374,  -335, -21,  -335, -335,
    328,  60,   -335, 332,  329,  333,  83,   -335, 140,  343,  -335, -21,  -41,  -335, -335, -21,  -335, 140,  -335,
    -335, -335, -335, -335, -335, -335, -335, 140,  392,  60,   340,  140,  140,  349,  -335, -21,  -335, -335, -335,
    -335, -335, -335, -335, -335, -335, 117,  -335, -335, 140,  -335, -335, -335, -335, -335, 365,  -335, -21,  117,
    -335, -335, -335, -21,  -335 };

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] = {
    3,   0,   4,   1,   146, 0,   0,   0,   4,   0,   0,   4,   10,  7,   9,   11,  8,   0,   123, 0,   0,   261, 0,
    258, 0,   6,   120, 14,  249, 249, 249, 2,   12,  244, 245, 246, 5,   0,   131, 0,   125, 123, 0,   0,   263, 143,
    0,   257, 136, 250, 0,   0,   0,   13,  147, 148, 0,   130, 116, 114, 0,   133, 129, 124, 127, 0,   269, 0,   266,
    0,   0,   0,   0,   0,   0,   0,   239, 0,   238, 237, 0,   223, 241, 240, 243, 227, 235, 170, 172, 174, 176, 187,
    190, 194, 197, 204, 206, 207, 0,   205, 242, 0,   260, 258, 0,   0,   136, 0,   0,   0,   150, 151, 152, 0,   132,
    117, 115, 122, 134, 128, 125, 0,   0,   268, 142, 0,   265, 0,   231, 198, 229, 0,   199, 202, 201, 203, 200, 175,
    0,   15,  225, 0,   0,   0,   234, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   217, 216, 0,   0,   219, 0,   261, 264, 259, 166, 0,   163, 108, 106, 0,   137, 15,  15,  15,  153,
    0,   149, 126, 121, 270, 266, 230, 0,   0,   208, 0,   0,   139, 17,  0,   0,   224, 210, 228, 235, 171, 173, 179,
    181, 186, 182, 183, 180, 177, 178, 184, 185, 188, 189, 193, 191, 192, 195, 196, 213, 0,   221, 0,   0,   262, 0,
    165, 138, 0,   109, 107, 135, 0,   0,   0,   154, 267, 232, 0,   0,   215, 0,   0,   16,  223, 225, 236, 219, 209,
    0,   220, 218, 214, 167, 164, 0,   0,   0,   159, 161, 0,   162, 0,   160, 0,   154, 0,   233, 141, 140, 17,  0,
    226, 0,   221, 251, 0,   251, 168, 0,   112, 110, 0,   155, 156, 18,  211, 212, 222, 0,   251, 255, 251, 0,   255,
    15,  0,   157, 113, 111, 144, 0,   252, 255, 56,  253, 251, 56,  0,   158, 254, 256, 0,   91,  57,  0,   0,   0,
    0,   40,  0,   42,  247, 0,   39,  0,   231, 0,   255, 248, 169, 0,   0,   92,  0,   36,  0,   0,   0,   51,  37,
    0,   41,  0,   101, 42,  42,  45,  0,   27,  0,   0,   0,   0,   0,   0,   0,   0,   56,  42,  35,  86,  76,  0,
    0,   42,  38,  83,  44,  43,  0,   58,  0,   0,   0,   0,   0,   0,   0,   0,   271, 93,  0,   89,  86,  0,   75,
    0,   0,   46,  0,   89,  83,  42,  24,  26,  22,  20,  21,  23,  19,  25,  95,  0,   0,   0,   0,   87,  76,  81,
    79,  0,   0,   49,  46,  118, 0,   119, 0,   84,  0,   0,   0,   99,  97,  0,   42,  42,  67,  65,  0,   77,  82,
    0,   0,   78,  69,  0,   0,   0,   47,  42,  104, 102, 0,   62,  59,  60,  0,   96,  42,  100, 98,  34,  88,  90,
    68,  66,  33,  42,  79,  0,   0,   42,  42,  54,  52,  0,   85,  105, 103, 32,  63,  64,  61,  29,  94,  0,   80,
    70,  42,  48,  50,  55,  53,  28,  73,  71,  0,   0,   74,  72,  31,  0,   30 };

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] = {
    -335, -335, -335, 210,  -335, 402,  -335, -169, 161,  -294, -335, -334, -271, 18,   -335, -335, 21,
    -335, -335, -335, -335, -335, -335, -335, -335, -335, -55,  -335, -335, 28,   -335, -23,  -335, 46,
    -335, 55,   -335, 53,   -335, -335, -335, -335, -335, -335, -335, -335, -335, -335, -335, -335, 385,
    -335, -335, -22,  -335, 403,  341,  -335, 388,  -335, -335, 93,   347,  -335, -162, -335, 124,  -335,
    -335, -111, -335, -335, -335, 185,  -335, -335, -213, -335, -335, -335, -19,  -335, 320,  -67,  -335,
    118,  96,   87,   -17,  -335, -303, 205,  180,  211,  215,  -68,  391,  -335, -149, 254,  -335, 69,
    -335, -335, -335, 232,  -194, -335, -265, -335, 366,  422,  301,  -335, 181,  287,  348,  -335, -335 };

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] = {
    0,   1,   2,   10,  11,  31,  12,  198, 247, 350, 345, 351, 326, 417, 418, 443, 342, 471, 488, 327,
    374, 452, 478, 434, 461, 466, 492, 495, 367, 390, 413, 439, 437, 395, 396, 387, 388, 410, 336, 407,
    426, 429, 456, 347, 448, 474, 178, 234, 286, 303, 60,  116, 420, 27,  13,  39,  63,  64,  40,  57,
    119, 328, 105, 106, 195, 196, 307, 16,  17,  55,  112, 113, 184, 268, 269, 270, 174, 175, 230, 300,
    86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96,  226, 254, 141, 200, 97,  331, 130, 98,  144,
    99,  100, 32,  33,  34,  50,  295, 296, 308, 22,  47,  23,  44,  102, 67,  126, 68,  123, 35 };

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] = {
    45,  128, 185, 127, 61,  329, 168, 227, 329, 476, 137, 236, 237, 238,  325, 372, 373, 325, 224, 258, 69,  70,  329,
    71,  340, 293, 20,  72,  73,  385, 58,  74,  75,  311, 59,  393, 76,   77,  117, 78,  333, 21,  315, 242, 162, 124,
    329, 329, 284, 294, 285, 131, 477, 283, 132, 133, 134, 135, 136, 329,  329, 449, 140, 424, 450, 3,   329, 362, 325,
    69,  70,  451, 71,  163, 202, 341, 72,  73,  26,  205, 74,  75,  171,  273, 62,  76,  77,  298, 78,  79,  169, 384,
    24,  85,  329, 14,  458, 459, 80,  187, 306, 14,  309, 188, 14,  81,   139, 38,  82,  83,  190, 472, 84,  85,  159,
    28,  332, 164, 110, 160, 480, 165, 29,  111, 203, 30,  15,  329, 329,  481, 312, 166, 15,  485, 486, 15,  161, 469,
    79,  191, 167, 37,  329, 470, 446, 221, 222, 80,  192, 225, 493, 329,  127, 447, 81,  176, 235, 82,  83,  427, 329,
    84,  85,  177, 329, 329, 428, 69,  70,  297, 71,  490, 41,  157, 72,   73,  491, 158, 74,  75,  249, 329, 42,  76,
    316, 317, 78,  43,  18,  262, 318, 19,  46,  263, 48,  354, 49,  -145, 4,   264, 54,  355, 356, 265, 419, 357, 319,
    84,  320, 358, 257, 56,  321, 266, 267, 5,   322, 359, 25,  360, 101,  36,  66,  323, 272, 432, 433, 7,   324, 140,
    361, 21,  225, -56, 278, 6,   79,  7,   107, 349, 330, 108, 85,  330,  104, 80,  218, 219, 220, 8,   9,   109, 81,
    216, 217, 82,  83,  330, 114, 84,  85,  51,  52,  115, 304, 206, 207,  208, 209, 210, 211, 212, 213, 214, 215, 118,
    122, 125, 138, 142, 143, 330, 330, 314, 145, 173, 147, 148, 149, 146,  170, 180, 183, 181, 330, 330, 182, 339, 337,
    193, 344, 330, 343, 194, 346, 348, 197, 353, 199, 201, 85,  223, 229,  231, 364, 150, 151, 152, 153, 154, 155, 156,
    232, 370, 233, 239, 241, 243, 245, 330, 244, 246, 251, 375, 248, 376,  377, 378, 379, 380, 381, 382, 383, 252, 253,
    255, 256, 259, 260, 392, 261, 271, 279, 274, 398, 399, 400, 401, 402,  403, 404, 405, 330, 330, 280, 281, 282, 408,
    288, 290, 291, 299, 414, 301, 302, 6,   310, 330, 313, 334, 335, 338,  168, 363, 352, 368, 330, 366, 386, 389, 394,
    127, 365, 369, 391, 330, 440, 343, 371, 330, 330, 406, 409, 397, 415,  416, 425, 457, 412, 436, 430, 438, 462, 431,
    442, 453, 330, 445, 465, 464, 460, 487, 455, 316, 317, 454, 475, 463,  467, 318, 479, 473, 468, 494, 53,  289, 444,
    441, 497, 484, 435, 482, 423, 411, 65,  319, 483, 320, 422, 489, 121,  321, 120, 179, 287, 322, 277, 250, 292, 275,
    129, 186, 323, 421, 276, 204, 7,   324, 103, 172, 496, 228, 0,   189,  305, 498, 240, 0,   0,   0,   0,   85 };

static const yytype_int16 yycheck[] = {
    22,  69,  113, 12,  19,  308, 10,  169, 311, 50,  77,  180, 181, 182, 308, 349, 350, 311, 167, 232, 9,   10,  325,
    12,  51,  75,  87,  16,  17,  363, 54,  20,  21,  298, 58,  369, 25,  26,  60,  28,  311, 102, 307, 192, 13,  67,
    349, 350, 54,  99,  56,  70,  93,  266, 71,  72,  73,  74,  75,  362, 363, 54,  81,  397, 57,  0,   369, 332, 362,
    9,   10,  64,  12,  42,  142, 102, 16,  17,  99,  146, 20,  21,  101, 245, 99,  25,  26,  281, 28,  78,  94,  362,
    102, 102, 397, 2,   430, 431, 87,  121, 294, 8,   296, 122, 11,  94,  95,  102, 97,  98,  127, 445, 101, 102, 7,
    70,  310, 17,  69,  12,  454, 21,  77,  74,  143, 80,  2,   430, 431, 463, 299, 31,  8,   467, 468, 11,  29,  54,
    78,  31,  40,  47,  445, 60,  54,  162, 163, 87,  40,  168, 484, 454, 12,  63,  94,  54,  178, 97,  98,  54,  463,
    101, 102, 62,  467, 468, 61,  9,   10,  280, 12,  54,  99,  16,  16,  17,  59,  20,  20,  21,  199, 484, 102, 25,
    44,  45,  28,  40,  99,  67,  50,  102, 19,  71,  99,  8,   85,  47,  48,  77,  102, 14,  15,  81,  98,  18,  66,
    101, 68,  22,  229, 32,  72,  91,  92,  65,  76,  30,  8,   32,  32,  11,  102, 83,  243, 54,  55,  87,  88,  248,
    43,  102, 251, 93,  253, 85,  78,  87,  102, 99,  308, 102, 102, 311, 91,  87,  159, 160, 161, 99,  100, 102, 94,
    157, 158, 97,  98,  325, 98,  101, 102, 29,  30,  65,  286, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 99,
    32,  19,  94,  23,  19,  349, 350, 305, 27,  102, 4,   5,   6,   24,  41,  10,  85,  10,  362, 363, 10,  319, 317,
    11,  322, 369, 321, 102, 323, 324, 102, 329, 19,  96,  102, 102, 32,  99,  336, 33,  34,  35,  36,  37,  38,  39,
    19,  345, 87,  99,  102, 32,  19,  397, 96,  19,  10,  352, 95,  354, 355, 356, 357, 358, 359, 360, 361, 41,  19,
    11,  96,  11,  11,  368, 11,  41,  99,  102, 376, 377, 378, 379, 380, 381, 382, 383, 430, 431, 47,  99,  102, 386,
    99,  96,  11,  10,  391, 99,  47,  85,  99,  445, 99,  11,  84,  46,  10,  99,  93,  32,  454, 102, 46,  19,  46,
    12,  99,  99,  73,  463, 415, 416, 99,  467, 468, 82,  79,  99,  89,  53,  90,  429, 102, 49,  99,  19,  434, 99,
    52,  102, 484, 99,  86,  438, 46,  72,  84,  44,  45,  99,  448, 99,  99,  50,  452, 88,  99,  68,  32,  274, 418,
    416, 493, 99,  412, 464, 396, 388, 41,  66,  465, 68,  395, 471, 65,  72,  64,  106, 269, 76,  251, 203, 278, 248,
    69,  120, 83,  394, 249, 145, 87,  88,  46,  103, 492, 170, -1,  125, 293, 497, 189, -1,  -1,  -1,  -1,  102 };

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] = {
    0,   104, 105, 0,   48,  65,  85,  87,  99,  100, 106, 107, 109, 157, 164, 169, 170, 171, 99,  102, 87,  102, 212,
    214, 102, 106, 99,  156, 70,  77,  80,  108, 205, 206, 207, 221, 106, 47,  102, 158, 161, 99,  102, 40,  215, 156,
    19,  213, 99,  85,  208, 208, 208, 108, 102, 172, 32,  162, 54,  58,  153, 19,  99,  159, 160, 158, 102, 217, 219,
    9,   10,  12,  16,  17,  20,  21,  25,  26,  28,  78,  87,  94,  97,  98,  101, 102, 183, 184, 185, 186, 187, 188,
    189, 190, 191, 192, 193, 198, 201, 203, 204, 32,  216, 214, 91,  165, 166, 102, 102, 102, 69,  74,  173, 174, 98,
    65,  154, 156, 99,  163, 161, 153, 32,  220, 156, 19,  218, 12,  198, 199, 200, 183, 191, 191, 191, 191, 191, 186,
    94,  95,  183, 196, 23,  19,  202, 27,  24,  4,   5,   6,   33,  34,  35,  36,  37,  38,  39,  16,  20,  7,   12,
    29,  13,  42,  17,  21,  31,  40,  10,  94,  41,  183, 213, 102, 179, 180, 54,  62,  149, 165, 10,  10,  10,  85,
    175, 172, 159, 156, 183, 219, 191, 31,  40,  11,  102, 167, 168, 102, 110, 19,  197, 96,  198, 183, 185, 186, 188,
    188, 188, 188, 188, 188, 188, 188, 188, 188, 189, 189, 190, 190, 190, 191, 191, 102, 201, 183, 194, 167, 215, 32,
    181, 99,  19,  87,  150, 156, 110, 110, 110, 99,  218, 102, 201, 32,  96,  19,  19,  111, 95,  183, 202, 10,  41,
    19,  195, 11,  96,  183, 179, 11,  11,  11,  67,  71,  77,  81,  91,  92,  176, 177, 178, 41,  183, 167, 102, 196,
    197, 194, 183, 99,  47,  99,  102, 179, 54,  56,  151, 176, 99,  111, 96,  11,  195, 75,  99,  209, 210, 172, 209,
    10,  182, 99,  47,  152, 156, 217, 209, 169, 211, 209, 99,  211, 110, 99,  156, 211, 44,  45,  50,  66,  68,  72,
    76,  83,  88,  112, 115, 122, 164, 193, 198, 199, 209, 115, 11,  84,  141, 183, 46,  156, 51,  102, 119, 183, 156,
    113, 183, 146, 183, 99,  112, 114, 93,  156, 8,   14,  15,  18,  22,  30,  32,  43,  211, 99,  156, 99,  102, 131,
    32,  99,  156, 99,  114, 114, 123, 183, 183, 183, 183, 183, 183, 183, 183, 183, 115, 114, 46,  138, 139, 19,  132,
    73,  183, 114, 46,  136, 137, 99,  156, 156, 156, 156, 156, 156, 156, 156, 82,  142, 183, 79,  140, 138, 102, 133,
    183, 89,  53,  116, 117, 98,  155, 204, 140, 136, 114, 90,  143, 54,  61,  144, 99,  99,  54,  55,  126, 132, 49,
    135, 19,  134, 183, 119, 52,  118, 116, 99,  54,  63,  147, 54,  57,  64,  124, 102, 99,  84,  145, 156, 114, 114,
    46,  127, 156, 99,  183, 86,  128, 99,  99,  54,  60,  120, 114, 88,  148, 156, 50,  93,  125, 156, 114, 114, 134,
    183, 99,  114, 114, 72,  121, 156, 54,  59,  129, 114, 68,  130, 156, 129, 156 };

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] = {
    0,   103, 104, 105, 106, 106, 106, 107, 107, 107, 107, 107, 108, 108, 109, 110, 110, 111, 111, 112, 112, 112, 112,
    112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 113, 113, 114, 114, 114, 115,
    116, 116, 117, 118, 118, 119, 120, 120, 121, 121, 122, 122, 123, 124, 124, 124, 125, 125, 125, 126, 126, 127, 127,
    128, 128, 129, 129, 130, 130, 131, 132, 132, 133, 134, 134, 135, 135, 136, 136, 137, 138, 138, 139, 140, 140, 141,
    141, 142, 142, 143, 143, 144, 144, 145, 145, 146, 147, 147, 148, 148, 149, 149, 150, 150, 151, 151, 152, 152, 153,
    153, 154, 154, 155, 155, 156, 157, 157, 158, 158, 159, 159, 159, 160, 160, 161, 162, 162, 163, 163, 164, 165, 165,
    166, 167, 167, 168, 169, 169, 170, 171, 171, 172, 173, 173, 174, 174, 175, 175, 176, 176, 177, 177, 177, 178, 178,
    178, 178, 179, 179, 180, 181, 181, 182, 182, 183, 184, 184, 185, 185, 186, 186, 187, 187, 187, 187, 187, 187, 187,
    187, 187, 187, 187, 188, 188, 188, 189, 189, 189, 189, 190, 190, 190, 191, 191, 191, 191, 191, 191, 191, 192, 192,
    192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 193, 194, 194, 195, 195, 196, 196, 197, 197, 198, 198, 199,
    199, 200, 200, 200, 201, 202, 202, 203, 203, 203, 203, 203, 203, 204, 205, 205, 205, 206, 207, 208, 208, 209, 209,
    209, 210, 211, 211, 212, 213, 213, 214, 215, 215, 216, 216, 217, 218, 218, 219, 220, 220, 221 };

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] = {
    0, 2, 3, 0, 0, 2, 2, 1, 1, 1,  1,  1, 1, 2, 2, 0, 2, 0, 3, 4, 4, 4, 4, 4, 4, 4, 4, 2, 8, 7, 11, 10, 7, 7,
    7, 3, 2, 2, 3, 1, 0, 1, 0, 2,  2,  2, 0, 2, 4, 0, 3, 1, 1, 2, 0, 1, 0, 1, 1, 1, 1, 2, 0, 1, 1,  1,  2, 0,
    1, 0, 2, 1, 2, 0, 1, 2, 0, 3,  2,  0, 3, 0, 1, 0, 2, 4, 0, 2, 4, 0, 3, 0, 1, 0, 4, 0, 2, 1, 2,  0,  1, 1,
    1, 2, 0, 1, 1, 2, 0, 1, 1, 2,  0,  1, 1, 2, 0, 1, 1, 1, 1, 6, 5, 0, 2, 0, 3, 1, 2, 1, 2, 0, 2,  0,  1, 6,
    0, 2, 3, 1, 3, 3, 5, 3, 9, 0,  1,  1, 0, 2, 1, 1, 0, 1, 0, 2, 2, 3, 4, 1, 1, 1, 1, 1, 3, 2, 0,  2,  0, 3,
    1, 3, 1, 3, 1, 2, 1, 3, 3, 3,  3,  3, 3, 3, 3, 3, 3, 1, 3, 3, 1, 3, 3, 3, 1, 3, 3, 1, 2, 2, 2,  2,  2, 2,
    1, 1, 1, 1, 3, 4, 3, 6, 6, 3,  4,  4, 2, 2, 4, 0, 2, 0, 3, 0, 2, 0, 3, 1, 3, 1, 2, 1, 3, 4, 2,  0,  3, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 10, 10, 0, 1, 0, 2, 2, 3, 0, 2, 2, 0, 3, 3, 0, 4, 0, 2, 2, 0, 3, 2,  0,  2, 12 };

enum
{
   YYENOMEM = -2
};

#define yyerrok   ( yyerrstatus = 0 )
#define yyclearin ( yychar = YYEMPTY )

#define YYACCEPT goto yyacceptlab
#define YYABORT  goto yyabortlab
#define YYERROR  goto yyerrorlab
#define YYNOMEM  goto yyexhaustedlab

#define YYRECOVERING() ( !!yyerrstatus )

#define YYBACKUP( Token, Value )                                                                                       \
   do                                                                                                                  \
      if( yychar == YYEMPTY )                                                                                          \
      {                                                                                                                \
         yychar = ( Token );                                                                                           \
         yylval = ( Value );                                                                                           \
         YYPOPSTACK( yylen );                                                                                          \
         yystate = *yyssp;                                                                                             \
         goto yybackup;                                                                                                \
      }                                                                                                                \
      else                                                                                                             \
      {                                                                                                                \
         yyerror( &yylloc, scanner, result, YY_( "syntax error: cannot back up" ) );                                   \
         YYERROR;                                                                                                      \
      }                                                                                                                \
   while( 0 )

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
#   define YYLLOC_DEFAULT( Current, Rhs, N )                                                                           \
      do                                                                                                               \
         if( N )                                                                                                       \
         {                                                                                                             \
            ( Current ).first_line   = YYRHSLOC( Rhs, 1 ).first_line;                                                  \
            ( Current ).first_column = YYRHSLOC( Rhs, 1 ).first_column;                                                \
            ( Current ).last_line    = YYRHSLOC( Rhs, N ).last_line;                                                   \
            ( Current ).last_column  = YYRHSLOC( Rhs, N ).last_column;                                                 \
         }                                                                                                             \
         else                                                                                                          \
         {                                                                                                             \
            ( Current ).first_line = ( Current ).last_line = YYRHSLOC( Rhs, 0 ).last_line;                             \
            ( Current ).first_column = ( Current ).last_column = YYRHSLOC( Rhs, 0 ).last_column;                       \
         }                                                                                                             \
      while( 0 )
#endif

#define YYRHSLOC( Rhs, K ) ( ( Rhs )[ K ] )

/* Enable debugging if requested.  */
#if YYDEBUG

#   ifndef YYFPRINTF
#      include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#      define YYFPRINTF fprintf
#   endif

#   define YYDPRINTF( Args )                                                                                           \
      do                                                                                                               \
      {                                                                                                                \
         if( yydebug )                                                                                                 \
            YYFPRINTF Args;                                                                                            \
      } while( 0 )

/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#   ifndef YYLOCATION_PRINT

#      if defined YY_LOCATION_PRINT

/* Temporary convenience wrapper in case some people defined the
   undocumented and private YY_LOCATION_PRINT macros.  */
#         define YYLOCATION_PRINT( File, Loc ) YY_LOCATION_PRINT( File, *( Loc ) )

#      elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int yy_location_print_( FILE *yyo, YYLTYPE const *const yylocp )
{
   int res     = 0;
   int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
   if( 0 <= yylocp->first_line )
   {
      res += YYFPRINTF( yyo, "%d", yylocp->first_line );
      if( 0 <= yylocp->first_column )
         res += YYFPRINTF( yyo, ".%d", yylocp->first_column );
   }
   if( 0 <= yylocp->last_line )
   {
      if( yylocp->first_line < yylocp->last_line )
      {
         res += YYFPRINTF( yyo, "-%d", yylocp->last_line );
         if( 0 <= end_col )
            res += YYFPRINTF( yyo, ".%d", end_col );
      }
      else if( 0 <= end_col && yylocp->first_column < end_col )
         res += YYFPRINTF( yyo, "-%d", end_col );
   }
   return res;
}

#         define YYLOCATION_PRINT               yy_location_print_

/* Temporary convenience wrapper in case some people defined the
   undocumented and private YY_LOCATION_PRINT macros.  */
#         define YY_LOCATION_PRINT( File, Loc ) YYLOCATION_PRINT( File, &( Loc ) )

#      else

#         define YYLOCATION_PRINT( File, Loc ) ( ( void ) 0 )
/* Temporary convenience wrapper in case some people defined the
   undocumented and private YY_LOCATION_PRINT macros.  */
#         define YY_LOCATION_PRINT             YYLOCATION_PRINT

#      endif
#   endif /* !defined YYLOCATION_PRINT */

#   define YY_SYMBOL_PRINT( Title, Kind, Value, Location )                                                             \
      do                                                                                                               \
      {                                                                                                                \
         if( yydebug )                                                                                                 \
         {                                                                                                             \
            YYFPRINTF( stderr, "%s ", Title );                                                                         \
            yy_symbol_print( stderr, Kind, Value, Location, scanner, result );                                         \
            YYFPRINTF( stderr, "\n" );                                                                                 \
         }                                                                                                             \
      } while( 0 )

/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void yy_symbol_value_print( FILE *yyo, yysymbol_kind_t yykind, YYSTYPE const *const yyvaluep,
                                   YYLTYPE const *const yylocationp, yyscan_t scanner, YYSTYPE *result )
{
   FILE *yyoutput = yyo;
   YY_USE( yyoutput );
   YY_USE( yylocationp );
   YY_USE( scanner );
   YY_USE( result );
   if( !yyvaluep )
      return;
   YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
   YY_USE( yykind );
   YY_IGNORE_MAYBE_UNINITIALIZED_END
}

/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void yy_symbol_print( FILE *yyo, yysymbol_kind_t yykind, YYSTYPE const *const yyvaluep,
                             YYLTYPE const *const yylocationp, yyscan_t scanner, YYSTYPE *result )
{
   YYFPRINTF( yyo, "%s %s (", yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name( yykind ) );

   YYLOCATION_PRINT( yyo, yylocationp );
   YYFPRINTF( yyo, ": " );
   yy_symbol_value_print( yyo, yykind, yyvaluep, yylocationp, scanner, result );
   YYFPRINTF( yyo, ")" );
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void yy_stack_print( yy_state_t *yybottom, yy_state_t *yytop )
{
   YYFPRINTF( stderr, "Stack now" );
   for( ; yybottom <= yytop; yybottom++ )
   {
      int yybot = *yybottom;
      YYFPRINTF( stderr, " %d", yybot );
   }
   YYFPRINTF( stderr, "\n" );
}

#   define YY_STACK_PRINT( Bottom, Top )                                                                               \
      do                                                                                                               \
      {                                                                                                                \
         if( yydebug )                                                                                                 \
            yy_stack_print( ( Bottom ), ( Top ) );                                                                     \
      } while( 0 )

/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void yy_reduce_print( yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, yyscan_t scanner,
                             YYSTYPE *result )
{
   int yylno  = yyrline[ yyrule ];
   int yynrhs = yyr2[ yyrule ];
   int yyi;
   YYFPRINTF( stderr, "Reducing stack by rule %d (line %d):\n", yyrule - 1, yylno );
   /* The symbols being reduced.  */
   for( yyi = 0; yyi < yynrhs; yyi++ )
   {
      YYFPRINTF( stderr, "   $%d = ", yyi + 1 );
      yy_symbol_print( stderr, YY_ACCESSING_SYMBOL( +yyssp[ yyi + 1 - yynrhs ] ), &yyvsp[ ( yyi + 1 ) - ( yynrhs ) ],
                       &( yylsp[ ( yyi + 1 ) - ( yynrhs ) ] ), scanner, result );
      YYFPRINTF( stderr, "\n" );
   }
}

#   define YY_REDUCE_PRINT( Rule )                                                                                     \
      do                                                                                                               \
      {                                                                                                                \
         if( yydebug )                                                                                                 \
            yy_reduce_print( yyssp, yyvsp, yylsp, Rule, scanner, result );                                             \
      } while( 0 )

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
#   define YYDPRINTF( Args ) ( ( void ) 0 )
#   define YY_SYMBOL_PRINT( Title, Kind, Value, Location )
#   define YY_STACK_PRINT( Bottom, Top )
#   define YY_REDUCE_PRINT( Rule )
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
#   define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
#   define YYMAXDEPTH 10000
#endif

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void yydestruct( const char *yymsg, yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp,
                        yyscan_t scanner, YYSTYPE *result )
{
   YY_USE( yyvaluep );
   YY_USE( yylocationp );
   YY_USE( scanner );
   YY_USE( result );
   if( !yymsg )
      yymsg = "Deleting";
   YY_SYMBOL_PRINT( yymsg, yykind, yyvaluep, yylocationp );

   YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
   YY_USE( yykind );
   YY_IGNORE_MAYBE_UNINITIALIZED_END
}

/*----------.
| yyparse.  |
`----------*/

int yyparse( yyscan_t scanner, YYSTYPE *result )
{
   /* Lookahead token kind.  */
   int yychar;

   /* The semantic value of the lookahead symbol.  */
   /* Default value used for initialization, for pacifying older GCCs
      or non-GCC compilers.  */
   YY_INITIAL_VALUE( static YYSTYPE yyval_default; )
   YYSTYPE yylval YY_INITIAL_VALUE( = yyval_default );

   /* Location data for the lookahead symbol.  */
   static YYLTYPE yyloc_default
#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
       = { 1, 1, 1, 1 }
#endif
   ;
   YYLTYPE yylloc = yyloc_default;

   /* Number of syntax errors so far.  */
   int yynerrs = 0;

   yy_state_fast_t yystate = 0;
   /* Number of tokens to shift before error messages enabled.  */
   int yyerrstatus = 0;

   /* Refer to the stacks through separate pointers, to allow yyoverflow
      to reallocate them elsewhere.  */

   /* Their size.  */
   YYPTRDIFF_T yystacksize = YYINITDEPTH;

   /* The state stack: array, bottom, top.  */
   yy_state_t  yyssa[ YYINITDEPTH ];
   yy_state_t *yyss  = yyssa;
   yy_state_t *yyssp = yyss;

   /* The semantic value stack: array, bottom, top.  */
   YYSTYPE  yyvsa[ YYINITDEPTH ];
   YYSTYPE *yyvs  = yyvsa;
   YYSTYPE *yyvsp = yyvs;

   /* The location stack: array, bottom, top.  */
   YYLTYPE  yylsa[ YYINITDEPTH ];
   YYLTYPE *yyls  = yylsa;
   YYLTYPE *yylsp = yyls;

   int yyn;
   /* The return value of yyparse.  */
   int yyresult;
   /* Lookahead symbol kind.  */
   yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
   /* The variables used to return semantic value and location from the
      action routines.  */
   YYSTYPE yyval;
   YYLTYPE yyloc;

   /* The locations where the error started and ended.  */
   YYLTYPE yyerror_range[ 3 ];

#define YYPOPSTACK( N ) ( yyvsp -= ( N ), yyssp -= ( N ), yylsp -= ( N ) )

   /* The number of symbols on the RHS of the reduced rule.
      Keep to zero when no symbol should be popped.  */
   int yylen = 0;

   YYDPRINTF( ( stderr, "Starting parse\n" ) );

   yychar = YYEMPTY; /* Cause a token to be read.  */

   yylsp[ 0 ] = yylloc;
   goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
   /* In all cases, when you get here, the value and location stacks
      have just been pushed.  So pushing a state here evens the stacks.  */
   yyssp++;

/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
   YYDPRINTF( ( stderr, "Entering state %d\n", yystate ) );
   YY_ASSERT( 0 <= yystate && yystate < YYNSTATES );
   YY_IGNORE_USELESS_CAST_BEGIN
   *yyssp = YY_CAST( yy_state_t, yystate );
   YY_IGNORE_USELESS_CAST_END
   YY_STACK_PRINT( yyss, yyssp );

   if( yyss + yystacksize - 1 <= yyssp )
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
      YYNOMEM;
#else
   {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

#   if defined yyoverflow
      {
         /* Give user a chance to reallocate the stack.  Use copies of
            these so that the &'s don't force the real ones into
            memory.  */
         yy_state_t *yyss1 = yyss;
         YYSTYPE    *yyvs1 = yyvs;
         YYLTYPE    *yyls1 = yyls;

         /* Each stack pointer address is followed by the size of the
            data in use in that stack, in bytes.  This used to be a
            conditional around just the two extra args, but that might
            be undefined if yyoverflow is a macro.  */
         yyoverflow( YY_( "memory exhausted" ), &yyss1, yysize * YYSIZEOF( *yyssp ), &yyvs1,
                     yysize * YYSIZEOF( *yyvsp ), &yyls1, yysize * YYSIZEOF( *yylsp ), &yystacksize );
         yyss = yyss1;
         yyvs = yyvs1;
         yyls = yyls1;
      }
#   else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if( YYMAXDEPTH <= yystacksize )
         YYNOMEM;
      yystacksize *= 2;
      if( YYMAXDEPTH < yystacksize )
         yystacksize = YYMAXDEPTH;

      {
         yy_state_t    *yyss1 = yyss;
         union yyalloc *yyptr =
             YY_CAST( union yyalloc *, YYSTACK_ALLOC( YY_CAST( YYSIZE_T, YYSTACK_BYTES( yystacksize ) ) ) );
         if( !yyptr )
            YYNOMEM;
         YYSTACK_RELOCATE( yyss_alloc, yyss );
         YYSTACK_RELOCATE( yyvs_alloc, yyvs );
         YYSTACK_RELOCATE( yyls_alloc, yyls );
#      undef YYSTACK_RELOCATE
         if( yyss1 != yyssa )
            YYSTACK_FREE( yyss1 );
      }
#   endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF( ( stderr, "Stack size increased to %ld\n", YY_CAST( long, yystacksize ) ) );
      YY_IGNORE_USELESS_CAST_END

      if( yyss + yystacksize - 1 <= yyssp )
         YYABORT;
   }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

   if( yystate == YYFINAL )
      YYACCEPT;

   goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:
   /* Do appropriate processing given the current state.  Read a
      lookahead token if we need one and don't already have one.  */

   /* First try to decide what to do without reference to lookahead token.  */
   yyn = yypact[ yystate ];
   if( yypact_value_is_default( yyn ) )
      goto yydefault;

   /* Not known => get a lookahead token if don't already have one.  */

   /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
   if( yychar == YYEMPTY )
   {
      YYDPRINTF( ( stderr, "Reading a token\n" ) );
      yychar = yylex( &yylval, &yylloc, scanner );
   }

   if( yychar <= YYEOF )
   {
      yychar  = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF( ( stderr, "Now at end of input.\n" ) );
   }
   else if( yychar == YYerror )
   {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar             = YYUNDEF;
      yytoken            = YYSYMBOL_YYerror;
      yyerror_range[ 1 ] = yylloc;
      goto yyerrlab1;
   }
   else
   {
      yytoken = YYTRANSLATE( yychar );
      YY_SYMBOL_PRINT( "Next token is", yytoken, &yylval, &yylloc );
   }

   /* If the proper action on seeing token YYTOKEN is to reduce or to
      detect an error, take that action.  */
   yyn += yytoken;
   if( yyn < 0 || YYLAST < yyn || yycheck[ yyn ] != yytoken )
      goto yydefault;
   yyn = yytable[ yyn ];
   if( yyn <= 0 )
   {
      if( yytable_value_is_error( yyn ) )
         goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
   }

   /* Count tokens shifted since error; after three, turn off error
      status.  */
   if( yyerrstatus )
      yyerrstatus--;

   /* Shift the lookahead token.  */
   YY_SYMBOL_PRINT( "Shifting", yytoken, &yylval, &yylloc );
   yystate = yyn;
   YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
   *++yyvsp = yylval;
   YY_IGNORE_MAYBE_UNINITIALIZED_END
   *++yylsp = yylloc;

   /* Discard the shifted token.  */
   yychar = YYEMPTY;
   goto yynewstate;

/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
   yyn = yydefact[ yystate ];
   if( yyn == 0 )
      goto yyerrlab;
   goto yyreduce;

/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
   /* yyn is the number of a rule to reduce with.  */
   yylen = yyr2[ yyn ];

   /* If YYLEN is nonzero, implement the default value of the action:
      '$$ = $1'.

      Otherwise, the following line sets YYVAL to garbage.
      This behavior is undocumented and Bison
      users should not rely upon it.  Assigning to YYVAL
      unconditionally makes the parser a bit smaller, and it avoids a
      GCC warning that YYVAL may be used uninitialized.  */
   yyval = yyvsp[ 1 - yylen ];

   /* Default location. */
   YYLLOC_DEFAULT( yyloc, ( yylsp - yylen ), yylen );
   yyerror_range[ 1 ] = yyloc;
   YY_REDUCE_PRINT( yyn );
   switch( yyn )
   {
   case 2: /* Program: LeadingEols TopDecls Defs  */
#line 404 "dory.y"
   {
      ( yyval.program_ ) =
          make_ProgramRoot( ( yyvsp[ -2 ].leadingeols_ ), ( yyvsp[ -1 ].topdecls_ ), ( yyvsp[ 0 ].defs_ ) );
      result->program_ = ( yyval.program_ );
   }
#line 1838 "Parser.c"
   break;

   case 3: /* LeadingEols: %empty  */
#line 406 "dory.y"
   {
      ( yyval.leadingeols_ ) = make_LeadingEolsEmpty();
   }
#line 1844 "Parser.c"
   break;

   case 4: /* TopDecls: %empty  */
#line 408 "dory.y"
   {
      ( yyval.topdecls_ ) = make_ListTopDeclEmpty();
   }
#line 1850 "Parser.c"
   break;

   case 5: /* TopDecls: TopDecl TopDecls  */
#line 409 "dory.y"
   {
      ( yyval.topdecls_ ) = make_ListTopDeclCons( ( yyvsp[ -1 ].topdecl_ ), ( yyvsp[ 0 ].topdecls_ ) );
   }
#line 1856 "Parser.c"
   break;

   case 6: /* TopDecls: T_LineEol TopDecls  */
#line 410 "dory.y"
   {
      ( yyval.topdecls_ ) = make_ListTopDeclLine( ( yyvsp[ -1 ]._string ), ( yyvsp[ 0 ].topdecls_ ) );
   }
#line 1862 "Parser.c"
   break;

   case 7: /* TopDecl: EnumDecl  */
#line 412 "dory.y"
   {
      ( yyval.topdecl_ ) = make_TopDeclEnum( ( yyvsp[ 0 ].enumdecl_ ) );
   }
#line 1868 "Parser.c"
   break;

   case 8: /* TopDecl: ClassDecl  */
#line 413 "dory.y"
   {
      ( yyval.topdecl_ ) = make_TopDeclClass( ( yyvsp[ 0 ].classdecl_ ) );
   }
#line 1874 "Parser.c"
   break;

   case 9: /* TopDecl: StructDecl  */
#line 414 "dory.y"
   {
      ( yyval.topdecl_ ) = make_TopDeclStruct( ( yyvsp[ 0 ].structdecl_ ) );
   }
#line 1880 "Parser.c"
   break;

   case 10: /* TopDecl: PreprocDecl  */
#line 415 "dory.y"
   {
      ( yyval.topdecl_ ) = make_TopDeclPreproc( ( yyvsp[ 0 ].preprocdecl_ ) );
   }
#line 1886 "Parser.c"
   break;

   case 11: /* TopDecl: StaticDecl  */
#line 416 "dory.y"
   {
      ( yyval.topdecl_ ) = make_TopDeclStatic( ( yyvsp[ 0 ].staticdecl_ ) );
   }
#line 1892 "Parser.c"
   break;

   case 12: /* Defs: Def  */
#line 418 "dory.y"
   {
      ( yyval.defs_ ) = make_ListDefSingle( ( yyvsp[ 0 ].def_ ) );
   }
#line 1898 "Parser.c"
   break;

   case 13: /* Defs: Def Defs  */
#line 419 "dory.y"
   {
      ( yyval.defs_ ) = make_ListDefCons( ( yyvsp[ -1 ].def_ ), ( yyvsp[ 0 ].defs_ ) );
   }
#line 1904 "Parser.c"
   break;

   case 14: /* PreprocDecl: T_PreprocLine StmtEnd  */
#line 421 "dory.y"
   {
      ( yyval.preprocdecl_ ) = make_PreprocDeclLine( ( yyvsp[ -1 ]._string ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 1910 "Parser.c"
   break;

   case 15: /* ParamListOpt: %empty  */
#line 423 "dory.y"
   {
      ( yyval.paramlistopt_ ) = make_OptParamListEmpty();
   }
#line 1916 "Parser.c"
   break;

   case 16: /* ParamListOpt: _IDENT_ ParamListTail  */
#line 424 "dory.y"
   {
      ( yyval.paramlistopt_ ) = make_OptParamListSome( ( yyvsp[ -1 ]._string ), ( yyvsp[ 0 ].paramlisttail_ ) );
   }
#line 1922 "Parser.c"
   break;

   case 17: /* ParamListTail: %empty  */
#line 426 "dory.y"
   {
      ( yyval.paramlisttail_ ) = make_ListParamTailEmpty();
   }
#line 1928 "Parser.c"
   break;

   case 18: /* ParamListTail: _COMMA _IDENT_ ParamListTail  */
#line 427 "dory.y"
   {
      ( yyval.paramlisttail_ ) = make_ListParamTailCons( ( yyvsp[ -1 ]._string ), ( yyvsp[ 0 ].paramlisttail_ ) );
   }
#line 1934 "Parser.c"
   break;

   case 19: /* Stmt: LHS _COLONEQ Expr StmtEnd  */
#line 429 "dory.y"
   {
      ( yyval.stmt_ ) = make_StmtAssignInl( ( yyvsp[ -3 ].lhs_ ), ( yyvsp[ -1 ].expr_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 1940 "Parser.c"
   break;

   case 20: /* Stmt: LHS _PLUSEQ Expr StmtEnd  */
#line 430 "dory.y"
   {
      ( yyval.stmt_ ) = make_StmtAssignAdd( ( yyvsp[ -3 ].lhs_ ), ( yyvsp[ -1 ].expr_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 1946 "Parser.c"
   break;

   case 21: /* Stmt: LHS _MINUSEQ Expr StmtEnd  */
#line 431 "dory.y"
   {
      ( yyval.stmt_ ) = make_StmtAssignSub( ( yyvsp[ -3 ].lhs_ ), ( yyvsp[ -1 ].expr_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 1952 "Parser.c"
   break;

   case 22: /* Stmt: LHS _STAREQ Expr StmtEnd  */
#line 432 "dory.y"
   {
      ( yyval.stmt_ ) = make_StmtAssignMul( ( yyvsp[ -3 ].lhs_ ), ( yyvsp[ -1 ].expr_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 1958 "Parser.c"
   break;

   case 23: /* Stmt: LHS _SLASHEQ Expr StmtEnd  */
#line 433 "dory.y"
   {
      ( yyval.stmt_ ) = make_StmtAssignDiv( ( yyvsp[ -3 ].lhs_ ), ( yyvsp[ -1 ].expr_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 1964 "Parser.c"
   break;

   case 24: /* Stmt: LHS _PERCENTEQ Expr StmtEnd  */
#line 434 "dory.y"
   {
      ( yyval.stmt_ ) = make_StmtAssignMod( ( yyvsp[ -3 ].lhs_ ), ( yyvsp[ -1 ].expr_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 1970 "Parser.c"
   break;

   case 25: /* Stmt: LHS _CARETEQ Expr StmtEnd  */
#line 435 "dory.y"
   {
      ( yyval.stmt_ ) = make_StmtAssignPow( ( yyvsp[ -3 ].lhs_ ), ( yyvsp[ -1 ].expr_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 1976 "Parser.c"
   break;

   case 26: /* Stmt: LHS _SYMB_8 Expr StmtEnd  */
#line 436 "dory.y"
   {
      ( yyval.stmt_ ) = make_StmtAssignPow2( ( yyvsp[ -3 ].lhs_ ), ( yyvsp[ -1 ].expr_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 1982 "Parser.c"
   break;

   case 27: /* Stmt: CallExpr StmtEnd  */
#line 437 "dory.y"
   {
      ( yyval.stmt_ ) = make_StmtCall( ( yyvsp[ -1 ].callexpr_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 1988 "Parser.c"
   break;

   case 28: /* Stmt: _KW_if IfCond T_LineEol StmtList ElseIfList ElseOpt EndIf StmtEnd  */
#line 438 "dory.y"
   {
      ( yyval.stmt_ ) = make_StmtIf( ( yyvsp[ -6 ].ifcond_ ), ( yyvsp[ -5 ]._string ), ( yyvsp[ -4 ].stmtlist_ ),
                                     ( yyvsp[ -3 ].elseiflist_ ), ( yyvsp[ -2 ].elseopt_ ), ( yyvsp[ -1 ].endif_ ),
                                     ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 1994 "Parser.c"
   break;

   case 29: /* Stmt: DoOpt _KW_while WhileCond T_LineEol StmtList EndDo StmtEnd  */
#line 439 "dory.y"
   {
      ( yyval.stmt_ ) = make_StmtDoWhile( ( yyvsp[ -6 ].doopt_ ), ( yyvsp[ -4 ].whilecond_ ), ( yyvsp[ -3 ]._string ),
                                          ( yyvsp[ -2 ].stmtlist_ ), ( yyvsp[ -1 ].enddo_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 2000 "Parser.c"
   break;

   case 30: /* Stmt: _KW_for _IDENT_ _COLONEQ Expr _KW_to Expr StepOpt T_LineEol StmtList EndFor StmtEnd  */
#line 440 "dory.y"
   {
      ( yyval.stmt_ ) = make_StmtFor( ( yyvsp[ -9 ]._string ), ( yyvsp[ -7 ].expr_ ), ( yyvsp[ -5 ].expr_ ),
                                      ( yyvsp[ -4 ].stepopt_ ), ( yyvsp[ -3 ]._string ), ( yyvsp[ -2 ].stmtlist_ ),
                                      ( yyvsp[ -1 ].endfor_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 2006 "Parser.c"
   break;

   case 31: /* Stmt: _KW_for _KW_each ForEachVars _KW_in ForEachExprs ForEachDescOpt T_LineEol StmtList EndFor StmtEnd
             */
#line 441 "dory.y"
   {
      ( yyval.stmt_ ) = make_StmtForEach( ( yyvsp[ -7 ].foreachvars_ ), ( yyvsp[ -5 ].foreachexprs_ ),
                                          ( yyvsp[ -4 ].foreachdescopt_ ), ( yyvsp[ -3 ]._string ),
                                          ( yyvsp[ -2 ].stmtlist_ ), ( yyvsp[ -1 ].endfor_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 2012 "Parser.c"
   break;

   case 32: /* Stmt: _KW_switch SwitchCond T_LineEol CaseList OtherwiseOpt EndSwitch StmtEnd  */
#line 442 "dory.y"
   {
      ( yyval.stmt_ ) =
          make_StmtSwitch( ( yyvsp[ -5 ].switchcond_ ), ( yyvsp[ -4 ]._string ), ( yyvsp[ -3 ].caselist_ ),
                           ( yyvsp[ -2 ].otherwiseopt_ ), ( yyvsp[ -1 ].endswitch_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 2018 "Parser.c"
   break;

   case 33: /* Stmt: _KW_do _KW_case T_LineEol CaseCondList OtherwiseOpt EndCase StmtEnd  */
#line 443 "dory.y"
   {
      ( yyval.stmt_ ) =
          make_StmtDoCase( ( yyvsp[ -4 ]._string ), ( yyvsp[ -3 ].casecondlist_ ), ( yyvsp[ -2 ].otherwiseopt_ ),
                           ( yyvsp[ -1 ].endcase_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 2024 "Parser.c"
   break;

   case 34: /* Stmt: _KW_begin _KW_sequence T_LineEol StmtList RecoverOpt EndSequence StmtEnd  */
#line 444 "dory.y"
   {
      ( yyval.stmt_ ) =
          make_StmtSequence( ( yyvsp[ -4 ]._string ), ( yyvsp[ -3 ].stmtlist_ ), ( yyvsp[ -2 ].recoveropt_ ),
                             ( yyvsp[ -1 ].endsequence_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 2030 "Parser.c"
   break;

   case 35: /* Stmt: _KW_break BreakExprOpt StmtEnd  */
#line 445 "dory.y"
   {
      ( yyval.stmt_ ) = make_StmtBreak( ( yyvsp[ -1 ].breakexpropt_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 2036 "Parser.c"
   break;

   case 36: /* Stmt: _KW_exit StmtEnd  */
#line 446 "dory.y"
   {
      ( yyval.stmt_ ) = make_StmtExit( ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 2042 "Parser.c"
   break;

   case 37: /* Stmt: _KW_loop StmtEnd  */
#line 447 "dory.y"
   {
      ( yyval.stmt_ ) = make_StmtLoop( ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 2048 "Parser.c"
   break;

   case 38: /* Stmt: _KW_return ReturnExprOpt StmtEnd  */
#line 448 "dory.y"
   {
      ( yyval.stmt_ ) = make_StmtReturn( ( yyvsp[ -1 ].returnexpropt_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 2054 "Parser.c"
   break;

   case 39: /* Stmt: StructDecl  */
#line 449 "dory.y"
   {
      ( yyval.stmt_ ) = make_StmtStructDecl( ( yyvsp[ 0 ].structdecl_ ) );
   }
#line 2060 "Parser.c"
   break;

   case 40: /* ReturnExprOpt: %empty  */
#line 451 "dory.y"
   {
      ( yyval.returnexpropt_ ) = make_ReturnExprOptEmpty();
   }
#line 2066 "Parser.c"
   break;

   case 41: /* ReturnExprOpt: Expr  */
#line 452 "dory.y"
   {
      ( yyval.returnexpropt_ ) = make_ReturnExprOptSome( ( yyvsp[ 0 ].expr_ ) );
   }
#line 2072 "Parser.c"
   break;

   case 42: /* StmtList: %empty  */
#line 454 "dory.y"
   {
      ( yyval.stmtlist_ ) = make_ListStmtEmpty();
   }
#line 2078 "Parser.c"
   break;

   case 43: /* StmtList: Stmt StmtList  */
#line 455 "dory.y"
   {
      ( yyval.stmtlist_ ) = make_ListStmtCons( ( yyvsp[ -1 ].stmt_ ), ( yyvsp[ 0 ].stmtlist_ ) );
   }
#line 2084 "Parser.c"
   break;

   case 44: /* StmtList: T_LineEol StmtList  */
#line 456 "dory.y"
   {
      ( yyval.stmtlist_ ) = make_ListStmtLine( ( yyvsp[ -1 ]._string ), ( yyvsp[ 0 ].stmtlist_ ) );
   }
#line 2090 "Parser.c"
   break;

   case 45: /* StmtListNonEmpty: Stmt StmtList  */
#line 458 "dory.y"
   {
      ( yyval.stmtlistnonempty_ ) = make_ListStmtNonEmpty( ( yyvsp[ -1 ].stmt_ ), ( yyvsp[ 0 ].stmtlist_ ) );
   }
#line 2096 "Parser.c"
   break;

   case 46: /* ElseIfList: %empty  */
#line 460 "dory.y"
   {
      ( yyval.elseiflist_ ) = make_ListElseIfEmpty();
   }
#line 2102 "Parser.c"
   break;

   case 47: /* ElseIfList: ElseIf ElseIfList  */
#line 461 "dory.y"
   {
      ( yyval.elseiflist_ ) = make_ListElseIfCons( ( yyvsp[ -1 ].elseif_ ), ( yyvsp[ 0 ].elseiflist_ ) );
   }
#line 2108 "Parser.c"
   break;

   case 48: /* ElseIf: _KW_elseif IfCond T_LineEol StmtList  */
#line 463 "dory.y"
   {
      ( yyval.elseif_ ) =
          make_ElseIfClause( ( yyvsp[ -2 ].ifcond_ ), ( yyvsp[ -1 ]._string ), ( yyvsp[ 0 ].stmtlist_ ) );
   }
#line 2114 "Parser.c"
   break;

   case 49: /* ElseOpt: %empty  */
#line 465 "dory.y"
   {
      ( yyval.elseopt_ ) = make_OptElseEmpty();
   }
#line 2120 "Parser.c"
   break;

   case 50: /* ElseOpt: _KW_else T_LineEol StmtList  */
#line 466 "dory.y"
   {
      ( yyval.elseopt_ ) = make_OptElseSome( ( yyvsp[ -1 ]._string ), ( yyvsp[ 0 ].stmtlist_ ) );
   }
#line 2126 "Parser.c"
   break;

   case 51: /* IfCond: Expr  */
#line 468 "dory.y"
   {
      ( yyval.ifcond_ ) = make_IfCondExpr( ( yyvsp[ 0 ].expr_ ) );
   }
#line 2132 "Parser.c"
   break;

   case 52: /* EndIf: _KW_endif  */
#line 470 "dory.y"
   {
      ( yyval.endif_ ) = make_EndIfWord();
   }
#line 2138 "Parser.c"
   break;

   case 53: /* EndIf: _KW_end EndIfOpt  */
#line 471 "dory.y"
   {
      ( yyval.endif_ ) = make_EndIfClause( ( yyvsp[ 0 ].endifopt_ ) );
   }
#line 2144 "Parser.c"
   break;

   case 54: /* EndIfOpt: %empty  */
#line 473 "dory.y"
   {
      ( yyval.endifopt_ ) = make_EndIfOptEmpty();
   }
#line 2150 "Parser.c"
   break;

   case 55: /* EndIfOpt: _KW_if  */
#line 474 "dory.y"
   {
      ( yyval.endifopt_ ) = make_EndIfOptIf();
   }
#line 2156 "Parser.c"
   break;

   case 56: /* DoOpt: %empty  */
#line 476 "dory.y"
   {
      ( yyval.doopt_ ) = make_DoOptEmpty();
   }
#line 2162 "Parser.c"
   break;

   case 57: /* DoOpt: _KW_do  */
#line 477 "dory.y"
   {
      ( yyval.doopt_ ) = make_DoOptDo();
   }
#line 2168 "Parser.c"
   break;

   case 58: /* WhileCond: Expr  */
#line 479 "dory.y"
   {
      ( yyval.whilecond_ ) = make_WhileCondExpr( ( yyvsp[ 0 ].expr_ ) );
   }
#line 2174 "Parser.c"
   break;

   case 59: /* EndDo: _KW_enddo  */
#line 481 "dory.y"
   {
      ( yyval.enddo_ ) = make_EndDoWord();
   }
#line 2180 "Parser.c"
   break;

   case 60: /* EndDo: _KW_endwhile  */
#line 482 "dory.y"
   {
      ( yyval.enddo_ ) = make_EndWhileWord();
   }
#line 2186 "Parser.c"
   break;

   case 61: /* EndDo: _KW_end EndDoOpt  */
#line 483 "dory.y"
   {
      ( yyval.enddo_ ) = make_EndDoClause( ( yyvsp[ 0 ].enddoopt_ ) );
   }
#line 2192 "Parser.c"
   break;

   case 62: /* EndDoOpt: %empty  */
#line 485 "dory.y"
   {
      ( yyval.enddoopt_ ) = make_EndDoOptEmpty();
   }
#line 2198 "Parser.c"
   break;

   case 63: /* EndDoOpt: _KW_do  */
#line 486 "dory.y"
   {
      ( yyval.enddoopt_ ) = make_EndDoOptDo();
   }
#line 2204 "Parser.c"
   break;

   case 64: /* EndDoOpt: _KW_while  */
#line 487 "dory.y"
   {
      ( yyval.enddoopt_ ) = make_EndDoOptWhile();
   }
#line 2210 "Parser.c"
   break;

   case 65: /* EndCase: _KW_endcase  */
#line 489 "dory.y"
   {
      ( yyval.endcase_ ) = make_EndCaseWord();
   }
#line 2216 "Parser.c"
   break;

   case 66: /* EndCase: _KW_end EndCaseOpt  */
#line 490 "dory.y"
   {
      ( yyval.endcase_ ) = make_EndCaseClause( ( yyvsp[ 0 ].endcaseopt_ ) );
   }
#line 2222 "Parser.c"
   break;

   case 67: /* EndCaseOpt: %empty  */
#line 492 "dory.y"
   {
      ( yyval.endcaseopt_ ) = make_EndCaseOptEmpty();
   }
#line 2228 "Parser.c"
   break;

   case 68: /* EndCaseOpt: _KW_case  */
#line 493 "dory.y"
   {
      ( yyval.endcaseopt_ ) = make_EndCaseOptCase();
   }
#line 2234 "Parser.c"
   break;

   case 69: /* StepOpt: %empty  */
#line 495 "dory.y"
   {
      ( yyval.stepopt_ ) = make_OptStepEmpty();
   }
#line 2240 "Parser.c"
   break;

   case 70: /* StepOpt: _KW_step Expr  */
#line 496 "dory.y"
   {
      ( yyval.stepopt_ ) = make_OptStepSome( ( yyvsp[ 0 ].expr_ ) );
   }
#line 2246 "Parser.c"
   break;

   case 71: /* EndFor: _KW_endfor  */
#line 498 "dory.y"
   {
      ( yyval.endfor_ ) = make_EndForWord();
   }
#line 2252 "Parser.c"
   break;

   case 72: /* EndFor: _KW_end EndForOpt  */
#line 499 "dory.y"
   {
      ( yyval.endfor_ ) = make_EndForClause( ( yyvsp[ 0 ].endforopt_ ) );
   }
#line 2258 "Parser.c"
   break;

   case 73: /* EndForOpt: %empty  */
#line 501 "dory.y"
   {
      ( yyval.endforopt_ ) = make_EndForOptEmpty();
   }
#line 2264 "Parser.c"
   break;

   case 74: /* EndForOpt: _KW_for  */
#line 502 "dory.y"
   {
      ( yyval.endforopt_ ) = make_EndForOptFor();
   }
#line 2270 "Parser.c"
   break;

   case 75: /* ForEachVars: _IDENT_ ForEachVarsTail  */
#line 504 "dory.y"
   {
      ( yyval.foreachvars_ ) = make_ForEachVarsMain( ( yyvsp[ -1 ]._string ), ( yyvsp[ 0 ].foreachvarstail_ ) );
   }
#line 2276 "Parser.c"
   break;

   case 76: /* ForEachVarsTail: %empty  */
#line 506 "dory.y"
   {
      ( yyval.foreachvarstail_ ) = make_ListForEachVarsTailEmpty();
   }
#line 2282 "Parser.c"
   break;

   case 77: /* ForEachVarsTail: _COMMA _IDENT_ ForEachVarsTail  */
#line 507 "dory.y"
   {
      ( yyval.foreachvarstail_ ) =
          make_ListForEachVarsTailCons( ( yyvsp[ -1 ]._string ), ( yyvsp[ 0 ].foreachvarstail_ ) );
   }
#line 2288 "Parser.c"
   break;

   case 78: /* ForEachExprs: Expr ForEachExprsTail  */
#line 509 "dory.y"
   {
      ( yyval.foreachexprs_ ) = make_ForEachExprsMain( ( yyvsp[ -1 ].expr_ ), ( yyvsp[ 0 ].foreachexprstail_ ) );
   }
#line 2294 "Parser.c"
   break;

   case 79: /* ForEachExprsTail: %empty  */
#line 511 "dory.y"
   {
      ( yyval.foreachexprstail_ ) = make_ListForEachExprsTailEmpty();
   }
#line 2300 "Parser.c"
   break;

   case 80: /* ForEachExprsTail: _COMMA Expr ForEachExprsTail  */
#line 512 "dory.y"
   {
      ( yyval.foreachexprstail_ ) =
          make_ListForEachExprsTailCons( ( yyvsp[ -1 ].expr_ ), ( yyvsp[ 0 ].foreachexprstail_ ) );
   }
#line 2306 "Parser.c"
   break;

   case 81: /* ForEachDescOpt: %empty  */
#line 514 "dory.y"
   {
      ( yyval.foreachdescopt_ ) = make_ForEachDescOptEmpty();
   }
#line 2312 "Parser.c"
   break;

   case 82: /* ForEachDescOpt: _KW_descend  */
#line 515 "dory.y"
   {
      ( yyval.foreachdescopt_ ) = make_ForEachDescOptSome();
   }
#line 2318 "Parser.c"
   break;

   case 83: /* CaseList: %empty  */
#line 517 "dory.y"
   {
      ( yyval.caselist_ ) = make_ListCaseEmpty();
   }
#line 2324 "Parser.c"
   break;

   case 84: /* CaseList: CaseClause CaseList  */
#line 518 "dory.y"
   {
      ( yyval.caselist_ ) = make_ListCaseCons( ( yyvsp[ -1 ].caseclause_ ), ( yyvsp[ 0 ].caselist_ ) );
   }
#line 2330 "Parser.c"
   break;

   case 85: /* CaseClause: _KW_case CaseValue T_LineEol StmtList  */
#line 520 "dory.y"
   {
      ( yyval.caseclause_ ) =
          make_CaseClauseItem( ( yyvsp[ -2 ].casevalue_ ), ( yyvsp[ -1 ]._string ), ( yyvsp[ 0 ].stmtlist_ ) );
   }
#line 2336 "Parser.c"
   break;

   case 86: /* CaseCondList: %empty  */
#line 522 "dory.y"
   {
      ( yyval.casecondlist_ ) = make_ListCaseCondEmpty();
   }
#line 2342 "Parser.c"
   break;

   case 87: /* CaseCondList: CaseCondClause CaseCondList  */
#line 523 "dory.y"
   {
      ( yyval.casecondlist_ ) = make_ListCaseCondCons( ( yyvsp[ -1 ].casecondclause_ ), ( yyvsp[ 0 ].casecondlist_ ) );
   }
#line 2348 "Parser.c"
   break;

   case 88: /* CaseCondClause: _KW_case Expr T_LineEol StmtList  */
#line 525 "dory.y"
   {
      ( yyval.casecondclause_ ) =
          make_CaseCondClauseItem( ( yyvsp[ -2 ].expr_ ), ( yyvsp[ -1 ]._string ), ( yyvsp[ 0 ].stmtlist_ ) );
   }
#line 2354 "Parser.c"
   break;

   case 89: /* OtherwiseOpt: %empty  */
#line 527 "dory.y"
   {
      ( yyval.otherwiseopt_ ) = make_OptOtherwiseEmpty();
   }
#line 2360 "Parser.c"
   break;

   case 90: /* OtherwiseOpt: _KW_otherwise T_LineEol StmtList  */
#line 528 "dory.y"
   {
      ( yyval.otherwiseopt_ ) = make_OptOtherwiseSome( ( yyvsp[ -1 ]._string ), ( yyvsp[ 0 ].stmtlist_ ) );
   }
#line 2366 "Parser.c"
   break;

   case 91: /* BreakExprOpt: %empty  */
#line 530 "dory.y"
   {
      ( yyval.breakexpropt_ ) = make_BreakExprOptEmpty();
   }
#line 2372 "Parser.c"
   break;

   case 92: /* BreakExprOpt: Expr  */
#line 531 "dory.y"
   {
      ( yyval.breakexpropt_ ) = make_BreakExprOptSome( ( yyvsp[ 0 ].expr_ ) );
   }
#line 2378 "Parser.c"
   break;

   case 93: /* RecoverOpt: %empty  */
#line 533 "dory.y"
   {
      ( yyval.recoveropt_ ) = make_RecoverOptEmpty();
   }
#line 2384 "Parser.c"
   break;

   case 94: /* RecoverOpt: _KW_recover RecoverUsingOpt T_LineEol StmtList  */
#line 534 "dory.y"
   {
      ( yyval.recoveropt_ ) =
          make_RecoverOptSome( ( yyvsp[ -2 ].recoverusingopt_ ), ( yyvsp[ -1 ]._string ), ( yyvsp[ 0 ].stmtlist_ ) );
   }
#line 2390 "Parser.c"
   break;

   case 95: /* RecoverUsingOpt: %empty  */
#line 536 "dory.y"
   {
      ( yyval.recoverusingopt_ ) = make_RecoverUsingOptEmpty();
   }
#line 2396 "Parser.c"
   break;

   case 96: /* RecoverUsingOpt: _KW_using _IDENT_  */
#line 537 "dory.y"
   {
      ( yyval.recoverusingopt_ ) = make_RecoverUsingOptSome( ( yyvsp[ 0 ]._string ) );
   }
#line 2402 "Parser.c"
   break;

   case 97: /* EndSequence: _KW_endsequence  */
#line 539 "dory.y"
   {
      ( yyval.endsequence_ ) = make_EndSequenceWord();
   }
#line 2408 "Parser.c"
   break;

   case 98: /* EndSequence: _KW_end EndSequenceOpt  */
#line 540 "dory.y"
   {
      ( yyval.endsequence_ ) = make_EndSequenceClause( ( yyvsp[ 0 ].endsequenceopt_ ) );
   }
#line 2414 "Parser.c"
   break;

   case 99: /* EndSequenceOpt: %empty  */
#line 542 "dory.y"
   {
      ( yyval.endsequenceopt_ ) = make_EndSequenceOptEmpty();
   }
#line 2420 "Parser.c"
   break;

   case 100: /* EndSequenceOpt: _KW_sequence  */
#line 543 "dory.y"
   {
      ( yyval.endsequenceopt_ ) = make_EndSequenceOptSeq();
   }
#line 2426 "Parser.c"
   break;

   case 101: /* SwitchCond: Expr  */
#line 545 "dory.y"
   {
      ( yyval.switchcond_ ) = make_SwitchCondExpr( ( yyvsp[ 0 ].expr_ ) );
   }
#line 2432 "Parser.c"
   break;

   case 102: /* EndSwitch: _KW_endswitch  */
#line 547 "dory.y"
   {
      ( yyval.endswitch_ ) = make_EndSwitchWord();
   }
#line 2438 "Parser.c"
   break;

   case 103: /* EndSwitch: _KW_end EndSwitchOpt  */
#line 548 "dory.y"
   {
      ( yyval.endswitch_ ) = make_EndSwitchClause( ( yyvsp[ 0 ].endswitchopt_ ) );
   }
#line 2444 "Parser.c"
   break;

   case 104: /* EndSwitchOpt: %empty  */
#line 550 "dory.y"
   {
      ( yyval.endswitchopt_ ) = make_EndSwitchOptEmpty();
   }
#line 2450 "Parser.c"
   break;

   case 105: /* EndSwitchOpt: _KW_switch  */
#line 551 "dory.y"
   {
      ( yyval.endswitchopt_ ) = make_EndSwitchOptSwitch();
   }
#line 2456 "Parser.c"
   break;

   case 106: /* EndStruct: _KW_endstruct  */
#line 553 "dory.y"
   {
      ( yyval.endstruct_ ) = make_EndStructWord();
   }
#line 2462 "Parser.c"
   break;

   case 107: /* EndStruct: _KW_end EndStructOpt  */
#line 554 "dory.y"
   {
      ( yyval.endstruct_ ) = make_EndStructClause( ( yyvsp[ 0 ].endstructopt_ ) );
   }
#line 2468 "Parser.c"
   break;

   case 108: /* EndStructOpt: %empty  */
#line 556 "dory.y"
   {
      ( yyval.endstructopt_ ) = make_EndStructOptEmpty();
   }
#line 2474 "Parser.c"
   break;

   case 109: /* EndStructOpt: _KW_struct  */
#line 557 "dory.y"
   {
      ( yyval.endstructopt_ ) = make_EndStructOptStruct();
   }
#line 2480 "Parser.c"
   break;

   case 110: /* EndClass: _KW_endclass  */
#line 559 "dory.y"
   {
      ( yyval.endclass_ ) = make_EndClassWord();
   }
#line 2486 "Parser.c"
   break;

   case 111: /* EndClass: _KW_end EndClassOpt  */
#line 560 "dory.y"
   {
      ( yyval.endclass_ ) = make_EndClassClause( ( yyvsp[ 0 ].endclassopt_ ) );
   }
#line 2492 "Parser.c"
   break;

   case 112: /* EndClassOpt: %empty  */
#line 562 "dory.y"
   {
      ( yyval.endclassopt_ ) = make_EndClassOptEmpty();
   }
#line 2498 "Parser.c"
   break;

   case 113: /* EndClassOpt: _KW_class  */
#line 563 "dory.y"
   {
      ( yyval.endclassopt_ ) = make_EndClassOptClass();
   }
#line 2504 "Parser.c"
   break;

   case 114: /* EndEnum: _KW_endenum  */
#line 565 "dory.y"
   {
      ( yyval.endenum_ ) = make_EndEnumWord();
   }
#line 2510 "Parser.c"
   break;

   case 115: /* EndEnum: _KW_end EndEnumOpt  */
#line 566 "dory.y"
   {
      ( yyval.endenum_ ) = make_EndEnumClause( ( yyvsp[ 0 ].endenumopt_ ) );
   }
#line 2516 "Parser.c"
   break;

   case 116: /* EndEnumOpt: %empty  */
#line 568 "dory.y"
   {
      ( yyval.endenumopt_ ) = make_EndEnumOptEmpty();
   }
#line 2522 "Parser.c"
   break;

   case 117: /* EndEnumOpt: _KW_enum  */
#line 569 "dory.y"
   {
      ( yyval.endenumopt_ ) = make_EndEnumOptEnum();
   }
#line 2528 "Parser.c"
   break;

   case 118: /* CaseValue: T_IntLit  */
#line 571 "dory.y"
   {
      ( yyval.casevalue_ ) = make_CaseValueInt( ( yyvsp[ 0 ]._string ) );
   }
#line 2534 "Parser.c"
   break;

   case 119: /* CaseValue: Napis  */
#line 572 "dory.y"
   {
      ( yyval.casevalue_ ) = make_CaseValueStr( ( yyvsp[ 0 ].napis_ ) );
   }
#line 2540 "Parser.c"
   break;

   case 120: /* StmtEnd: T_LineEol  */
#line 574 "dory.y"
   {
      ( yyval.stmtend_ ) = make_StmtEndLine( ( yyvsp[ 0 ]._string ) );
   }
#line 2546 "Parser.c"
   break;

   case 121: /* EnumDecl: _KW_enum _IDENT_ T_LineEol EnumListOpt EndEnum StmtEnd  */
#line 576 "dory.y"
   {
      ( yyval.enumdecl_ ) =
          make_EnumDeclNamed( ( yyvsp[ -4 ]._string ), ( yyvsp[ -3 ]._string ), ( yyvsp[ -2 ].enumlistopt_ ),
                              ( yyvsp[ -1 ].endenum_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 2552 "Parser.c"
   break;

   case 122: /* EnumDecl: _KW_enum T_LineEol EnumListOpt EndEnum StmtEnd  */
#line 577 "dory.y"
   {
      ( yyval.enumdecl_ ) = make_EnumDeclAnon( ( yyvsp[ -3 ]._string ), ( yyvsp[ -2 ].enumlistopt_ ),
                                               ( yyvsp[ -1 ].endenum_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 2558 "Parser.c"
   break;

   case 123: /* EnumListOpt: %empty  */
#line 579 "dory.y"
   {
      ( yyval.enumlistopt_ ) = make_OptEnumListEmpty();
   }
#line 2564 "Parser.c"
   break;

   case 124: /* EnumListOpt: EnumItem EnumListTail  */
#line 580 "dory.y"
   {
      ( yyval.enumlistopt_ ) = make_OptEnumListSome( ( yyvsp[ -1 ].enumitem_ ), ( yyvsp[ 0 ].enumlisttail_ ) );
   }
#line 2570 "Parser.c"
   break;

   case 125: /* EnumListTail: %empty  */
#line 582 "dory.y"
   {
      ( yyval.enumlisttail_ ) = make_ListEnumTailEmpty();
   }
#line 2576 "Parser.c"
   break;

   case 126: /* EnumListTail: EnumSep EnumItem EnumListTail  */
#line 583 "dory.y"
   {
      ( yyval.enumlisttail_ ) =
          make_ListEnumTailCons( ( yyvsp[ -2 ].enumsep_ ), ( yyvsp[ -1 ].enumitem_ ), ( yyvsp[ 0 ].enumlisttail_ ) );
   }
#line 2582 "Parser.c"
   break;

   case 127: /* EnumListTail: EnumSep  */
#line 584 "dory.y"
   {
      ( yyval.enumlisttail_ ) = make_ListEnumTailSep( ( yyvsp[ 0 ].enumsep_ ) );
   }
#line 2588 "Parser.c"
   break;

   case 128: /* EnumSep: _COMMA LineEolOpt  */
#line 586 "dory.y"
   {
      ( yyval.enumsep_ ) = make_EnumSepComma( ( yyvsp[ 0 ].lineeolopt_ ) );
   }
#line 2594 "Parser.c"
   break;

   case 129: /* EnumSep: T_LineEol  */
#line 587 "dory.y"
   {
      ( yyval.enumsep_ ) = make_EnumSepLine( ( yyvsp[ 0 ]._string ) );
   }
#line 2600 "Parser.c"
   break;

   case 130: /* EnumItem: _IDENT_ EnumValueOpt  */
#line 589 "dory.y"
   {
      ( yyval.enumitem_ ) = make_EnumItemDecl( ( yyvsp[ -1 ]._string ), ( yyvsp[ 0 ].enumvalueopt_ ) );
   }
#line 2606 "Parser.c"
   break;

   case 131: /* EnumValueOpt: %empty  */
#line 591 "dory.y"
   {
      ( yyval.enumvalueopt_ ) = make_OptEnumValueEmpty();
   }
#line 2612 "Parser.c"
   break;

   case 132: /* EnumValueOpt: _COLONEQ T_IntLit  */
#line 592 "dory.y"
   {
      ( yyval.enumvalueopt_ ) = make_OptEnumValueSome( ( yyvsp[ 0 ]._string ) );
   }
#line 2618 "Parser.c"
   break;

   case 133: /* LineEolOpt: %empty  */
#line 594 "dory.y"
   {
      ( yyval.lineeolopt_ ) = make_OptLineEolEmpty();
   }
#line 2624 "Parser.c"
   break;

   case 134: /* LineEolOpt: T_LineEol  */
#line 595 "dory.y"
   {
      ( yyval.lineeolopt_ ) = make_OptLineEolSome( ( yyvsp[ 0 ]._string ) );
   }
#line 2630 "Parser.c"
   break;

   case 135: /* StructDecl: _KW_struct _IDENT_ T_LineEol StructBody EndStruct StmtEnd  */
#line 597 "dory.y"
   {
      ( yyval.structdecl_ ) =
          make_StructDeclMain( ( yyvsp[ -4 ]._string ), ( yyvsp[ -3 ]._string ), ( yyvsp[ -2 ].structbody_ ),
                               ( yyvsp[ -1 ].endstruct_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 2636 "Parser.c"
   break;

   case 136: /* StructBody: %empty  */
#line 599 "dory.y"
   {
      ( yyval.structbody_ ) = make_StructBodyEmpty();
   }
#line 2642 "Parser.c"
   break;

   case 137: /* StructBody: StructMember StructBody  */
#line 600 "dory.y"
   {
      ( yyval.structbody_ ) = make_StructBodyCons( ( yyvsp[ -1 ].structmember_ ), ( yyvsp[ 0 ].structbody_ ) );
   }
#line 2648 "Parser.c"
   break;

   case 138: /* StructMember: _KW_var VarList T_LineEol  */
#line 602 "dory.y"
   {
      ( yyval.structmember_ ) = make_StructMemberVar( ( yyvsp[ -1 ].varlist_ ), ( yyvsp[ 0 ]._string ) );
   }
#line 2654 "Parser.c"
   break;

   case 139: /* FieldInitList: FieldInit  */
#line 604 "dory.y"
   {
      ( yyval.fieldinitlist_ ) = make_FieldInitListSingle( ( yyvsp[ 0 ].fieldinit_ ) );
   }
#line 2660 "Parser.c"
   break;

   case 140: /* FieldInitList: FieldInit _COMMA FieldInitList  */
#line 605 "dory.y"
   {
      ( yyval.fieldinitlist_ ) = make_FieldInitListCons( ( yyvsp[ -2 ].fieldinit_ ), ( yyvsp[ 0 ].fieldinitlist_ ) );
   }
#line 2666 "Parser.c"
   break;

   case 141: /* FieldInit: _IDENT_ _COLONEQ Expr  */
#line 607 "dory.y"
   {
      ( yyval.fieldinit_ ) = make_FieldInitItem( ( yyvsp[ -2 ]._string ), ( yyvsp[ 0 ].expr_ ) );
   }
#line 2672 "Parser.c"
   break;

   case 142: /* StaticDecl: _KW_static _KW_struct _IDENT_ IdentInitList StmtEnd  */
#line 609 "dory.y"
   {
      ( yyval.staticdecl_ ) =
          make_StaticStructDecl( ( yyvsp[ -2 ]._string ), ( yyvsp[ -1 ].identinitlist_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 2678 "Parser.c"
   break;

   case 143: /* StaticDecl: _KW_static StaticItemList StmtEnd  */
#line 610 "dory.y"
   {
      ( yyval.staticdecl_ ) = make_StaticDeclItem( ( yyvsp[ -1 ].staticitemlist_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 2684 "Parser.c"
   break;

   case 144: /* ClassDecl: CreateOpt _KW_class ClassName InheritOpt StaticClassOpt T_LineEol ClassBody EndClass StmtEnd
              */
#line 612 "dory.y"
   {
      ( yyval.classdecl_ ) =
          make_ClassDeclMain( ( yyvsp[ -8 ].createopt_ ), ( yyvsp[ -6 ].classname_ ), ( yyvsp[ -5 ].inheritopt_ ),
                              ( yyvsp[ -4 ].staticclassopt_ ), ( yyvsp[ -3 ]._string ), ( yyvsp[ -2 ].classbody_ ),
                              ( yyvsp[ -1 ].endclass_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 2690 "Parser.c"
   break;

   case 145: /* CreateOpt: %empty  */
#line 614 "dory.y"
   {
      ( yyval.createopt_ ) = make_OptCreateEmpty();
   }
#line 2696 "Parser.c"
   break;

   case 146: /* CreateOpt: _KW_create  */
#line 615 "dory.y"
   {
      ( yyval.createopt_ ) = make_OptCreateSome();
   }
#line 2702 "Parser.c"
   break;

   case 147: /* ClassName: _IDENT_  */
#line 617 "dory.y"
   {
      ( yyval.classname_ ) = make_ClassNameIdent( ( yyvsp[ 0 ]._string ) );
   }
#line 2708 "Parser.c"
   break;

   case 148: /* InheritOpt: %empty  */
#line 619 "dory.y"
   {
      ( yyval.inheritopt_ ) = make_OptInheritEmpty();
   }
#line 2714 "Parser.c"
   break;

   case 149: /* InheritOpt: InheritKeyword ClassName  */
#line 620 "dory.y"
   {
      ( yyval.inheritopt_ ) = make_OptInheritSome( ( yyvsp[ -1 ].inheritkeyword_ ), ( yyvsp[ 0 ].classname_ ) );
   }
#line 2720 "Parser.c"
   break;

   case 150: /* InheritKeyword: _KW_from  */
#line 622 "dory.y"
   {
      ( yyval.inheritkeyword_ ) = make_InheritFrom();
   }
#line 2726 "Parser.c"
   break;

   case 151: /* InheritKeyword: _KW_inherit  */
#line 623 "dory.y"
   {
      ( yyval.inheritkeyword_ ) = make_InheritInherit();
   }
#line 2732 "Parser.c"
   break;

   case 152: /* StaticClassOpt: %empty  */
#line 625 "dory.y"
   {
      ( yyval.staticclassopt_ ) = make_OptStaticClassEmpty();
   }
#line 2738 "Parser.c"
   break;

   case 153: /* StaticClassOpt: _KW_static  */
#line 626 "dory.y"
   {
      ( yyval.staticclassopt_ ) = make_OptStaticClassSome();
   }
#line 2744 "Parser.c"
   break;

   case 154: /* ClassBody: %empty  */
#line 628 "dory.y"
   {
      ( yyval.classbody_ ) = make_ClassBodyEmpty();
   }
#line 2750 "Parser.c"
   break;

   case 155: /* ClassBody: ClassMember ClassBody  */
#line 629 "dory.y"
   {
      ( yyval.classbody_ ) = make_ClassBodyCons( ( yyvsp[ -1 ].classmember_ ), ( yyvsp[ 0 ].classbody_ ) );
   }
#line 2756 "Parser.c"
   break;

   case 156: /* ClassMember: VisibilityScope T_LineEol  */
#line 631 "dory.y"
   {
      ( yyval.classmember_ ) = make_ClassMemberVisibility( ( yyvsp[ -1 ].visibilityscope_ ), ( yyvsp[ 0 ]._string ) );
   }
#line 2762 "Parser.c"
   break;

   case 157: /* ClassMember: _KW_var VarList T_LineEol  */
#line 632 "dory.y"
   {
      ( yyval.classmember_ ) = make_ClassMemberVar( ( yyvsp[ -1 ].varlist_ ), ( yyvsp[ 0 ]._string ) );
   }
#line 2768 "Parser.c"
   break;

   case 158: /* ClassMember: _KW_method _IDENT_ MethodParamsOpt T_LineEol  */
#line 633 "dory.y"
   {
      ( yyval.classmember_ ) =
          make_ClassMemberMethod( ( yyvsp[ -2 ]._string ), ( yyvsp[ -1 ].methodparamsopt_ ), ( yyvsp[ 0 ]._string ) );
   }
#line 2774 "Parser.c"
   break;

   case 159: /* VisibilityScope: _SYMB_9  */
#line 635 "dory.y"
   {
      ( yyval.visibilityscope_ ) = make_VisibilityExported();
   }
#line 2780 "Parser.c"
   break;

   case 160: /* VisibilityScope: _SYMB_10  */
#line 636 "dory.y"
   {
      ( yyval.visibilityscope_ ) = make_VisibilityVisible();
   }
#line 2786 "Parser.c"
   break;

   case 161: /* VisibilityScope: _SYMB_11  */
#line 637 "dory.y"
   {
      ( yyval.visibilityscope_ ) = make_VisibilityHidden();
   }
#line 2792 "Parser.c"
   break;

   case 162: /* VisibilityScope: _SYMB_12  */
#line 638 "dory.y"
   {
      ( yyval.visibilityscope_ ) = make_VisibilityProtected();
   }
#line 2798 "Parser.c"
   break;

   case 163: /* VarList: VarDecl  */
#line 640 "dory.y"
   {
      ( yyval.varlist_ ) = make_VarListSingle( ( yyvsp[ 0 ].vardecl_ ) );
   }
#line 2804 "Parser.c"
   break;

   case 164: /* VarList: VarDecl _COMMA VarList  */
#line 641 "dory.y"
   {
      ( yyval.varlist_ ) = make_VarListCons( ( yyvsp[ -2 ].vardecl_ ), ( yyvsp[ 0 ].varlist_ ) );
   }
#line 2810 "Parser.c"
   break;

   case 165: /* VarDecl: _IDENT_ VarInitOpt  */
#line 643 "dory.y"
   {
      ( yyval.vardecl_ ) = make_VarDeclItem( ( yyvsp[ -1 ]._string ), ( yyvsp[ 0 ].varinitopt_ ) );
   }
#line 2816 "Parser.c"
   break;

   case 166: /* VarInitOpt: %empty  */
#line 645 "dory.y"
   {
      ( yyval.varinitopt_ ) = make_OptVarInitEmpty();
   }
#line 2822 "Parser.c"
   break;

   case 167: /* VarInitOpt: _COLONEQ Expr  */
#line 646 "dory.y"
   {
      ( yyval.varinitopt_ ) = make_OptVarInitSome( ( yyvsp[ 0 ].expr_ ) );
   }
#line 2828 "Parser.c"
   break;

   case 168: /* MethodParamsOpt: %empty  */
#line 648 "dory.y"
   {
      ( yyval.methodparamsopt_ ) = make_MethodParamsEmpty();
   }
#line 2834 "Parser.c"
   break;

   case 169: /* MethodParamsOpt: _LPAREN ParamListOpt _RPAREN  */
#line 649 "dory.y"
   {
      ( yyval.methodparamsopt_ ) = make_MethodParamsSome( ( yyvsp[ -1 ].paramlistopt_ ) );
   }
#line 2840 "Parser.c"
   break;

   case 170: /* Expr: ExprOr  */
#line 651 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprAssignBase( ( yyvsp[ 0 ].expror_ ) );
   }
#line 2846 "Parser.c"
   break;

   case 171: /* ExprOr: ExprOr _SYMB_15 Expr1  */
#line 653 "dory.y"
   {
      ( yyval.expror_ ) = make_ExprOrDot( ( yyvsp[ -2 ].expror_ ), ( yyvsp[ 0 ].expr_ ) );
   }
#line 2852 "Parser.c"
   break;

   case 172: /* ExprOr: Expr1  */
#line 654 "dory.y"
   {
      ( yyval.expror_ ) = make_ExprOrBase( ( yyvsp[ 0 ].expr_ ) );
   }
#line 2858 "Parser.c"
   break;

   case 173: /* Expr1: Expr1 _SYMB_16 Expr2  */
#line 656 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprAndDot( ( yyvsp[ -2 ].expr_ ), ( yyvsp[ 0 ].expr_ ) );
   }
#line 2864 "Parser.c"
   break;

   case 174: /* Expr1: Expr2  */
#line 657 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprAndBase( ( yyvsp[ 0 ].expr_ ) );
   }
#line 2870 "Parser.c"
   break;

   case 175: /* Expr2: _SYMB_17 Expr2  */
#line 659 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprNotDot( ( yyvsp[ 0 ].expr_ ) );
   }
#line 2876 "Parser.c"
   break;

   case 176: /* Expr2: Expr3  */
#line 660 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprNotBase( ( yyvsp[ 0 ].expr_ ) );
   }
#line 2882 "Parser.c"
   break;

   case 177: /* Expr3: Expr3 _EQ Expr4  */
#line 662 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprEq( ( yyvsp[ -2 ].expr_ ), ( yyvsp[ 0 ].expr_ ) );
   }
#line 2888 "Parser.c"
   break;

   case 178: /* Expr3: Expr3 _DEQ Expr4  */
#line 663 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprEq2( ( yyvsp[ -2 ].expr_ ), ( yyvsp[ 0 ].expr_ ) );
   }
#line 2894 "Parser.c"
   break;

   case 179: /* Expr3: Expr3 _BANGEQ Expr4  */
#line 664 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprNeq( ( yyvsp[ -2 ].expr_ ), ( yyvsp[ 0 ].expr_ ) );
   }
#line 2900 "Parser.c"
   break;

   case 180: /* Expr3: Expr3 _LTGT Expr4  */
#line 665 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprNeq2( ( yyvsp[ -2 ].expr_ ), ( yyvsp[ 0 ].expr_ ) );
   }
#line 2906 "Parser.c"
   break;

   case 181: /* Expr3: Expr3 _HASH Expr4  */
#line 666 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprNeq3( ( yyvsp[ -2 ].expr_ ), ( yyvsp[ 0 ].expr_ ) );
   }
#line 2912 "Parser.c"
   break;

   case 182: /* Expr3: Expr3 _LT Expr4  */
#line 667 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprLt( ( yyvsp[ -2 ].expr_ ), ( yyvsp[ 0 ].expr_ ) );
   }
#line 2918 "Parser.c"
   break;

   case 183: /* Expr3: Expr3 _LDARROW Expr4  */
#line 668 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprLe( ( yyvsp[ -2 ].expr_ ), ( yyvsp[ 0 ].expr_ ) );
   }
#line 2924 "Parser.c"
   break;

   case 184: /* Expr3: Expr3 _GT Expr4  */
#line 669 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprGt( ( yyvsp[ -2 ].expr_ ), ( yyvsp[ 0 ].expr_ ) );
   }
#line 2930 "Parser.c"
   break;

   case 185: /* Expr3: Expr3 _GTEQ Expr4  */
#line 670 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprGe( ( yyvsp[ -2 ].expr_ ), ( yyvsp[ 0 ].expr_ ) );
   }
#line 2936 "Parser.c"
   break;

   case 186: /* Expr3: Expr3 _DOLLAR Expr4  */
#line 671 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprSubStr( ( yyvsp[ -2 ].expr_ ), ( yyvsp[ 0 ].expr_ ) );
   }
#line 2942 "Parser.c"
   break;

   case 187: /* Expr3: Expr4  */
#line 672 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprRelBase( ( yyvsp[ 0 ].expr_ ) );
   }
#line 2948 "Parser.c"
   break;

   case 188: /* Expr4: Expr4 _PLUS Expr5  */
#line 674 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprAdd( ( yyvsp[ -2 ].expr_ ), ( yyvsp[ 0 ].expr_ ) );
   }
#line 2954 "Parser.c"
   break;

   case 189: /* Expr4: Expr4 _MINUS Expr5  */
#line 675 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprSub( ( yyvsp[ -2 ].expr_ ), ( yyvsp[ 0 ].expr_ ) );
   }
#line 2960 "Parser.c"
   break;

   case 190: /* Expr4: Expr5  */
#line 676 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprAddBase( ( yyvsp[ 0 ].expr_ ) );
   }
#line 2966 "Parser.c"
   break;

   case 191: /* Expr5: Expr5 _STAR Expr6  */
#line 678 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprMul( ( yyvsp[ -2 ].expr_ ), ( yyvsp[ 0 ].expr_ ) );
   }
#line 2972 "Parser.c"
   break;

   case 192: /* Expr5: Expr5 _SLASH Expr6  */
#line 679 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprDiv( ( yyvsp[ -2 ].expr_ ), ( yyvsp[ 0 ].expr_ ) );
   }
#line 2978 "Parser.c"
   break;

   case 193: /* Expr5: Expr5 _PERCENT Expr6  */
#line 680 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprMod( ( yyvsp[ -2 ].expr_ ), ( yyvsp[ 0 ].expr_ ) );
   }
#line 2984 "Parser.c"
   break;

   case 194: /* Expr5: Expr6  */
#line 681 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprMulBase( ( yyvsp[ 0 ].expr_ ) );
   }
#line 2990 "Parser.c"
   break;

   case 195: /* Expr6: Expr6 _DSTAR Expr7  */
#line 683 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprPow1( ( yyvsp[ -2 ].expr_ ), ( yyvsp[ 0 ].expr_ ) );
   }
#line 2996 "Parser.c"
   break;

   case 196: /* Expr6: Expr6 _CARET Expr7  */
#line 684 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprPow2( ( yyvsp[ -2 ].expr_ ), ( yyvsp[ 0 ].expr_ ) );
   }
#line 3002 "Parser.c"
   break;

   case 197: /* Expr6: Expr7  */
#line 685 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprPowBase( ( yyvsp[ 0 ].expr_ ) );
   }
#line 3008 "Parser.c"
   break;

   case 198: /* Expr7: _AMP LHS  */
#line 687 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprAddrOf( ( yyvsp[ 0 ].lhs_ ) );
   }
#line 3014 "Parser.c"
   break;

   case 199: /* Expr7: _STAR Expr7  */
#line 688 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprDeref( ( yyvsp[ 0 ].expr_ ) );
   }
#line 3020 "Parser.c"
   break;

   case 200: /* Expr7: _DMINUS Expr7  */
#line 689 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprDecPre( ( yyvsp[ 0 ].expr_ ) );
   }
#line 3026 "Parser.c"
   break;

   case 201: /* Expr7: _DPLUS Expr7  */
#line 690 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprIncPre( ( yyvsp[ 0 ].expr_ ) );
   }
#line 3032 "Parser.c"
   break;

   case 202: /* Expr7: _PLUS Expr7  */
#line 691 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprPlus( ( yyvsp[ 0 ].expr_ ) );
   }
#line 3038 "Parser.c"
   break;

   case 203: /* Expr7: _MINUS Expr7  */
#line 692 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprMinus( ( yyvsp[ 0 ].expr_ ) );
   }
#line 3044 "Parser.c"
   break;

   case 204: /* Expr7: Expr8  */
#line 693 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprSignBase( ( yyvsp[ 0 ].expr_ ) );
   }
#line 3050 "Parser.c"
   break;

   case 205: /* Expr8: Literal  */
#line 695 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprLit( ( yyvsp[ 0 ].literal_ ) );
   }
#line 3056 "Parser.c"
   break;

   case 206: /* Expr8: CallExpr  */
#line 696 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprCall( ( yyvsp[ 0 ].callexpr_ ) );
   }
#line 3062 "Parser.c"
   break;

   case 207: /* Expr8: QualifiedId  */
#line 697 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprQId( ( yyvsp[ 0 ].qualifiedid_ ) );
   }
#line 3068 "Parser.c"
   break;

   case 208: /* Expr8: _LPAREN Expr _RPAREN  */
#line 698 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprParen( ( yyvsp[ -1 ].expr_ ) );
   }
#line 3074 "Parser.c"
   break;

   case 209: /* Expr8: Expr8 _LBRACK SubscriptList _RBRACK  */
#line 699 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprIndex( ( yyvsp[ -3 ].expr_ ), ( yyvsp[ -1 ].subscriptlist_ ) );
   }
#line 3080 "Parser.c"
   break;

   case 210: /* Expr8: _LBRACE ExprListOpt _RBRACE  */
#line 700 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprArray( ( yyvsp[ -1 ].exprlistopt_ ) );
   }
#line 3086 "Parser.c"
   break;

   case 211: /* Expr8: _LBRACE _BAR ParamListOpt _BAR ExprListOpt _RBRACE  */
#line 701 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprBlock( ( yyvsp[ -3 ].paramlistopt_ ), ( yyvsp[ -1 ].exprlistopt_ ) );
   }
#line 3092 "Parser.c"
   break;

   case 212: /* Expr8: Expr8 _COLON _IDENT_ _LPAREN ArgListOpt _RPAREN  */
#line 702 "dory.y"
   {
      ( yyval.expr_ ) =
          make_ExprMethodCall( ( yyvsp[ -5 ].expr_ ), ( yyvsp[ -3 ]._string ), ( yyvsp[ -1 ].arglistopt_ ) );
   }
#line 3098 "Parser.c"
   break;

   case 213: /* Expr8: Expr8 _COLON _IDENT_  */
#line 703 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprFieldAccess( ( yyvsp[ -2 ].expr_ ), ( yyvsp[ 0 ]._string ) );
   }
#line 3104 "Parser.c"
   break;

   case 214: /* Expr8: QualifiedId _LBRACE FieldInitList _RBRACE  */
#line 704 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprStructInitNamed( ( yyvsp[ -3 ].qualifiedid_ ), ( yyvsp[ -1 ].fieldinitlist_ ) );
   }
#line 3110 "Parser.c"
   break;

   case 215: /* Expr8: _KW_struct _LBRACE FieldInitList _RBRACE  */
#line 705 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprStructAnon( ( yyvsp[ -1 ].fieldinitlist_ ) );
   }
#line 3116 "Parser.c"
   break;

   case 216: /* Expr8: Expr8 _DMINUS  */
#line 706 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprDecPost( ( yyvsp[ -1 ].expr_ ) );
   }
#line 3122 "Parser.c"
   break;

   case 217: /* Expr8: Expr8 _DPLUS  */
#line 707 "dory.y"
   {
      ( yyval.expr_ ) = make_ExprIncPost( ( yyvsp[ -1 ].expr_ ) );
   }
#line 3128 "Parser.c"
   break;

   case 218: /* CallExpr: QualifiedId _LPAREN ArgListOpt _RPAREN  */
#line 709 "dory.y"
   {
      ( yyval.callexpr_ ) = make_CallExprMain( ( yyvsp[ -3 ].qualifiedid_ ), ( yyvsp[ -1 ].arglistopt_ ) );
   }
#line 3134 "Parser.c"
   break;

   case 219: /* ArgListOpt: %empty  */
#line 711 "dory.y"
   {
      ( yyval.arglistopt_ ) = make_OptArgListEmpty();
   }
#line 3140 "Parser.c"
   break;

   case 220: /* ArgListOpt: Expr ArgListTail  */
#line 712 "dory.y"
   {
      ( yyval.arglistopt_ ) = make_OptArgListSome( ( yyvsp[ -1 ].expr_ ), ( yyvsp[ 0 ].arglisttail_ ) );
   }
#line 3146 "Parser.c"
   break;

   case 221: /* ArgListTail: %empty  */
#line 714 "dory.y"
   {
      ( yyval.arglisttail_ ) = make_ListArgTailEmpty();
   }
#line 3152 "Parser.c"
   break;

   case 222: /* ArgListTail: _COMMA Expr ArgListTail  */
#line 715 "dory.y"
   {
      ( yyval.arglisttail_ ) = make_ListArgTailCons( ( yyvsp[ -1 ].expr_ ), ( yyvsp[ 0 ].arglisttail_ ) );
   }
#line 3158 "Parser.c"
   break;

   case 223: /* ExprListOpt: %empty  */
#line 717 "dory.y"
   {
      ( yyval.exprlistopt_ ) = make_OptExprListEmpty();
   }
#line 3164 "Parser.c"
   break;

   case 224: /* ExprListOpt: Expr ExprListTail  */
#line 718 "dory.y"
   {
      ( yyval.exprlistopt_ ) = make_OptExprListSome( ( yyvsp[ -1 ].expr_ ), ( yyvsp[ 0 ].exprlisttail_ ) );
   }
#line 3170 "Parser.c"
   break;

   case 225: /* ExprListTail: %empty  */
#line 720 "dory.y"
   {
      ( yyval.exprlisttail_ ) = make_ListExprTailEmpty();
   }
#line 3176 "Parser.c"
   break;

   case 226: /* ExprListTail: _COMMA Expr ExprListTail  */
#line 721 "dory.y"
   {
      ( yyval.exprlisttail_ ) = make_ListExprTailCons( ( yyvsp[ -1 ].expr_ ), ( yyvsp[ 0 ].exprlisttail_ ) );
   }
#line 3182 "Parser.c"
   break;

   case 227: /* QualifiedId: _IDENT_  */
#line 723 "dory.y"
   {
      ( yyval.qualifiedid_ ) = make_QualifiedIdSingle( ( yyvsp[ 0 ]._string ) );
   }
#line 3188 "Parser.c"
   break;

   case 228: /* QualifiedId: _IDENT_ _DOT QualifiedId  */
#line 724 "dory.y"
   {
      ( yyval.qualifiedid_ ) = make_QualifiedIdMore( ( yyvsp[ -2 ]._string ), ( yyvsp[ 0 ].qualifiedid_ ) );
   }
#line 3194 "Parser.c"
   break;

   case 229: /* LHS: LValue  */
#line 726 "dory.y"
   {
      ( yyval.lhs_ ) = make_LHSVal( ( yyvsp[ 0 ].lvalue_ ) );
   }
#line 3200 "Parser.c"
   break;

   case 230: /* LHS: _STAR Expr7  */
#line 727 "dory.y"
   {
      ( yyval.lhs_ ) = make_LHSDeref( ( yyvsp[ 0 ].expr_ ) );
   }
#line 3206 "Parser.c"
   break;

   case 231: /* LValue: QualifiedId  */
#line 729 "dory.y"
   {
      ( yyval.lvalue_ ) = make_LValQId( ( yyvsp[ 0 ].qualifiedid_ ) );
   }
#line 3212 "Parser.c"
   break;

   case 232: /* LValue: LValue _COLON _IDENT_  */
#line 730 "dory.y"
   {
      ( yyval.lvalue_ ) = make_LValField( ( yyvsp[ -2 ].lvalue_ ), ( yyvsp[ 0 ]._string ) );
   }
#line 3218 "Parser.c"
   break;

   case 233: /* LValue: LValue _LBRACK SubscriptList _RBRACK  */
#line 731 "dory.y"
   {
      ( yyval.lvalue_ ) = make_LValIndex( ( yyvsp[ -3 ].lvalue_ ), ( yyvsp[ -1 ].subscriptlist_ ) );
   }
#line 3224 "Parser.c"
   break;

   case 234: /* SubscriptList: Expr SubscriptListTail  */
#line 733 "dory.y"
   {
      ( yyval.subscriptlist_ ) = make_SubscriptListSingle( ( yyvsp[ -1 ].expr_ ), ( yyvsp[ 0 ].subscriptlisttail_ ) );
   }
#line 3230 "Parser.c"
   break;

   case 235: /* SubscriptListTail: %empty  */
#line 735 "dory.y"
   {
      ( yyval.subscriptlisttail_ ) = make_ListSubscriptTailEmpty();
   }
#line 3236 "Parser.c"
   break;

   case 236: /* SubscriptListTail: _COMMA Expr SubscriptListTail  */
#line 736 "dory.y"
   {
      ( yyval.subscriptlisttail_ ) =
          make_ListSubscriptTailCons( ( yyvsp[ -1 ].expr_ ), ( yyvsp[ 0 ].subscriptlisttail_ ) );
   }
#line 3242 "Parser.c"
   break;

   case 237: /* Literal: _KW_nil  */
#line 738 "dory.y"
   {
      ( yyval.literal_ ) = make_LitNil();
   }
#line 3248 "Parser.c"
   break;

   case 238: /* Literal: _SYMB_45  */
#line 739 "dory.y"
   {
      ( yyval.literal_ ) = make_LitTrueDot();
   }
#line 3254 "Parser.c"
   break;

   case 239: /* Literal: _SYMB_46  */
#line 740 "dory.y"
   {
      ( yyval.literal_ ) = make_LitFalseDot();
   }
#line 3260 "Parser.c"
   break;

   case 240: /* Literal: T_IntLit  */
#line 741 "dory.y"
   {
      ( yyval.literal_ ) = make_LitInt( ( yyvsp[ 0 ]._string ) );
   }
#line 3266 "Parser.c"
   break;

   case 241: /* Literal: T_FloatLit  */
#line 742 "dory.y"
   {
      ( yyval.literal_ ) = make_LitFloat( ( yyvsp[ 0 ]._string ) );
   }
#line 3272 "Parser.c"
   break;

   case 242: /* Literal: Napis  */
#line 743 "dory.y"
   {
      ( yyval.literal_ ) = make_LitStr( ( yyvsp[ 0 ].napis_ ) );
   }
#line 3278 "Parser.c"
   break;

   case 243: /* Napis: T_StrLit  */
#line 745 "dory.y"
   {
      ( yyval.napis_ ) = make_NapisStr( ( yyvsp[ 0 ]._string ) );
   }
#line 3284 "Parser.c"
   break;

   case 244: /* Def: FunctionDef  */
#line 747 "dory.y"
   {
      ( yyval.def_ ) = make_DefFun( ( yyvsp[ 0 ].functiondef_ ) );
   }
#line 3290 "Parser.c"
   break;

   case 245: /* Def: ProcedureDef  */
#line 748 "dory.y"
   {
      ( yyval.def_ ) = make_DefProc( ( yyvsp[ 0 ].proceduredef_ ) );
   }
#line 3296 "Parser.c"
   break;

   case 246: /* Def: MethodDef  */
#line 749 "dory.y"
   {
      ( yyval.def_ ) = make_DefMeth( ( yyvsp[ 0 ].methoddef_ ) );
   }
#line 3302 "Parser.c"
   break;

   case 247: /* FunctionDef: _KW_function StaticOpt _IDENT_ _LPAREN ParamListOpt _RPAREN T_LineEol LocalDeclsOpt
                StaticDeclsOpt StmtListNonEmpty  */
#line 751 "dory.y"
   {
      ( yyval.functiondef_ ) = make_FunctionDefMain(
          ( yyvsp[ -8 ].staticopt_ ), ( yyvsp[ -7 ]._string ), ( yyvsp[ -5 ].paramlistopt_ ), ( yyvsp[ -3 ]._string ),
          ( yyvsp[ -2 ].localdeclsopt_ ), ( yyvsp[ -1 ].staticdeclsopt_ ), ( yyvsp[ 0 ].stmtlistnonempty_ ) );
   }
#line 3308 "Parser.c"
   break;

   case 248: /* ProcedureDef: _KW_procedure StaticOpt _IDENT_ _LPAREN ParamListOpt _RPAREN T_LineEol LocalDeclsOpt
                StaticDeclsOpt StmtListNonEmpty  */
#line 753 "dory.y"
   {
      ( yyval.proceduredef_ ) = make_ProcedureDefMain(
          ( yyvsp[ -8 ].staticopt_ ), ( yyvsp[ -7 ]._string ), ( yyvsp[ -5 ].paramlistopt_ ), ( yyvsp[ -3 ]._string ),
          ( yyvsp[ -2 ].localdeclsopt_ ), ( yyvsp[ -1 ].staticdeclsopt_ ), ( yyvsp[ 0 ].stmtlistnonempty_ ) );
   }
#line 3314 "Parser.c"
   break;

   case 249: /* StaticOpt: %empty  */
#line 755 "dory.y"
   {
      ( yyval.staticopt_ ) = make_OptStaticEmpty();
   }
#line 3320 "Parser.c"
   break;

   case 250: /* StaticOpt: _KW_static  */
#line 756 "dory.y"
   {
      ( yyval.staticopt_ ) = make_OptStaticSome();
   }
#line 3326 "Parser.c"
   break;

   case 251: /* LocalDeclsOpt: %empty  */
#line 758 "dory.y"
   {
      ( yyval.localdeclsopt_ ) = make_ListLocalDeclEmpty();
   }
#line 3332 "Parser.c"
   break;

   case 252: /* LocalDeclsOpt: T_LineEol LocalDeclsOpt  */
#line 759 "dory.y"
   {
      ( yyval.localdeclsopt_ ) = make_ListLocalDeclLine( ( yyvsp[ -1 ]._string ), ( yyvsp[ 0 ].localdeclsopt_ ) );
   }
#line 3338 "Parser.c"
   break;

   case 253: /* LocalDeclsOpt: LocalDecl LocalDeclsOpt  */
#line 760 "dory.y"
   {
      ( yyval.localdeclsopt_ ) = make_ListLocalDeclCons( ( yyvsp[ -1 ].localdecl_ ), ( yyvsp[ 0 ].localdeclsopt_ ) );
   }
#line 3344 "Parser.c"
   break;

   case 254: /* LocalDecl: _KW_local IdentInitList StmtEnd  */
#line 762 "dory.y"
   {
      ( yyval.localdecl_ ) = make_LocalDeclItem( ( yyvsp[ -1 ].identinitlist_ ), ( yyvsp[ 0 ].stmtend_ ) );
   }
#line 3350 "Parser.c"
   break;

   case 255: /* StaticDeclsOpt: %empty  */
#line 764 "dory.y"
   {
      ( yyval.staticdeclsopt_ ) = make_ListStaticDeclEmpty();
   }
#line 3356 "Parser.c"
   break;

   case 256: /* StaticDeclsOpt: StaticDecl StaticDeclsOpt  */
#line 765 "dory.y"
   {
      ( yyval.staticdeclsopt_ ) =
          make_ListStaticDeclCons( ( yyvsp[ -1 ].staticdecl_ ), ( yyvsp[ 0 ].staticdeclsopt_ ) );
   }
#line 3362 "Parser.c"
   break;

   case 257: /* StaticItemList: StaticItem StaticItemTail  */
#line 767 "dory.y"
   {
      ( yyval.staticitemlist_ ) =
          make_StaticItemListSingle( ( yyvsp[ -1 ].staticitem_ ), ( yyvsp[ 0 ].staticitemtail_ ) );
   }
#line 3368 "Parser.c"
   break;

   case 258: /* StaticItemTail: %empty  */
#line 769 "dory.y"
   {
      ( yyval.staticitemtail_ ) = make_StaticItemTailEmpty();
   }
#line 3374 "Parser.c"
   break;

   case 259: /* StaticItemTail: _COMMA StaticItem StaticItemTail  */
#line 770 "dory.y"
   {
      ( yyval.staticitemtail_ ) =
          make_StaticItemTailCons( ( yyvsp[ -1 ].staticitem_ ), ( yyvsp[ 0 ].staticitemtail_ ) );
   }
#line 3380 "Parser.c"
   break;

   case 260: /* StaticItem: _IDENT_ StaticArrayDimsOpt StaticInitOpt  */
#line 772 "dory.y"
   {
      ( yyval.staticitem_ ) = make_StaticItemMain( ( yyvsp[ -2 ]._string ), ( yyvsp[ -1 ].staticarraydimsopt_ ),
                                                   ( yyvsp[ 0 ].staticinitopt_ ) );
   }
#line 3386 "Parser.c"
   break;

   case 261: /* StaticArrayDimsOpt: %empty  */
#line 774 "dory.y"
   {
      ( yyval.staticarraydimsopt_ ) = make_StaticArrayDimsEmpty();
   }
#line 3392 "Parser.c"
   break;

   case 262: /* StaticArrayDimsOpt: _LBRACK SubscriptList _RBRACK StaticArrayDimsOpt  */
#line 775 "dory.y"
   {
      ( yyval.staticarraydimsopt_ ) =
          make_StaticArrayDimsCons( ( yyvsp[ -2 ].subscriptlist_ ), ( yyvsp[ 0 ].staticarraydimsopt_ ) );
   }
#line 3398 "Parser.c"
   break;

   case 263: /* StaticInitOpt: %empty  */
#line 777 "dory.y"
   {
      ( yyval.staticinitopt_ ) = make_StaticInitEmpty();
   }
#line 3404 "Parser.c"
   break;

   case 264: /* StaticInitOpt: _COLONEQ Expr  */
#line 778 "dory.y"
   {
      ( yyval.staticinitopt_ ) = make_StaticInitSome( ( yyvsp[ 0 ].expr_ ) );
   }
#line 3410 "Parser.c"
   break;

   case 265: /* IdentInitList: IdentInit IdentInitTail  */
#line 780 "dory.y"
   {
      ( yyval.identinitlist_ ) = make_ListIdentInitSingle( ( yyvsp[ -1 ].identinit_ ), ( yyvsp[ 0 ].identinittail_ ) );
   }
#line 3416 "Parser.c"
   break;

   case 266: /* IdentInitTail: %empty  */
#line 782 "dory.y"
   {
      ( yyval.identinittail_ ) = make_ListIdentInitTailEmpty();
   }
#line 3422 "Parser.c"
   break;

   case 267: /* IdentInitTail: _COMMA IdentInit IdentInitTail  */
#line 783 "dory.y"
   {
      ( yyval.identinittail_ ) =
          make_ListIdentInitTailCons( ( yyvsp[ -1 ].identinit_ ), ( yyvsp[ 0 ].identinittail_ ) );
   }
#line 3428 "Parser.c"
   break;

   case 268: /* IdentInit: _IDENT_ InitOpt  */
#line 785 "dory.y"
   {
      ( yyval.identinit_ ) = make_IdentInitItem( ( yyvsp[ -1 ]._string ), ( yyvsp[ 0 ].initopt_ ) );
   }
#line 3434 "Parser.c"
   break;

   case 269: /* InitOpt: %empty  */
#line 787 "dory.y"
   {
      ( yyval.initopt_ ) = make_OptInitEmpty();
   }
#line 3440 "Parser.c"
   break;

   case 270: /* InitOpt: _COLONEQ Expr  */
#line 788 "dory.y"
   {
      ( yyval.initopt_ ) = make_OptInitSome( ( yyvsp[ 0 ].expr_ ) );
   }
#line 3446 "Parser.c"
   break;

   case 271: /* MethodDef: _KW_method StaticOpt _IDENT_ _LPAREN ParamListOpt _RPAREN _KW_class ClassName T_LineEol
                LocalDeclsOpt StaticDeclsOpt StmtListNonEmpty  */
#line 790 "dory.y"
   {
      ( yyval.methoddef_ ) =
          make_MethodDefMain( ( yyvsp[ -10 ].staticopt_ ), ( yyvsp[ -9 ]._string ), ( yyvsp[ -7 ].paramlistopt_ ),
                              ( yyvsp[ -4 ].classname_ ), ( yyvsp[ -3 ]._string ), ( yyvsp[ -2 ].localdeclsopt_ ),
                              ( yyvsp[ -1 ].staticdeclsopt_ ), ( yyvsp[ 0 ].stmtlistnonempty_ ) );
   }
#line 3452 "Parser.c"
   break;

#line 3456 "Parser.c"

   default:
      break;
   }
   /* User semantic actions sometimes alter yychar, and that requires
      that yytoken be updated with the new translation.  We take the
      approach of translating immediately before every use of yytoken.
      One alternative is translating here after every semantic action,
      but that translation would be missed if the semantic action invokes
      YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
      if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
      incorrect destructor might then be invoked immediately.  In the
      case of YYERROR or YYBACKUP, subsequent parser actions might lead
      to an incorrect destructor call or verbose syntax error message
      before the lookahead is translated.  */
   YY_SYMBOL_PRINT( "-> $$ =", YY_CAST( yysymbol_kind_t, yyr1[ yyn ] ), &yyval, &yyloc );

   YYPOPSTACK( yylen );
   yylen = 0;

   *++yyvsp = yyval;
   *++yylsp = yyloc;

   /* Now 'shift' the result of the reduction.  Determine what state
      that goes to, based on the state we popped back to and the rule
      number reduced by.  */
   {
      const int yylhs = yyr1[ yyn ] - YYNTOKENS;
      const int yyi   = yypgoto[ yylhs ] + *yyssp;
      yystate         = ( 0 <= yyi && yyi <= YYLAST && yycheck[ yyi ] == *yyssp ? yytable[ yyi ] : yydefgoto[ yylhs ] );
   }

   goto yynewstate;

/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
   /* Make sure we have latest lookahead translation.  See comments at
      user semantic actions for why this is necessary.  */
   yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE( yychar );
   /* If not already recovering from an error, report this error.  */
   if( !yyerrstatus )
   {
      ++yynerrs;
      yyerror( &yylloc, scanner, result, YY_( "syntax error" ) );
   }

   yyerror_range[ 1 ] = yylloc;
   if( yyerrstatus == 3 )
   {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if( yychar <= YYEOF )
      {
         /* Return failure if at end of input.  */
         if( yychar == YYEOF )
            YYABORT;
      }
      else
      {
         yydestruct( "Error: discarding", yytoken, &yylval, &yylloc, scanner, result );
         yychar = YYEMPTY;
      }
   }

   /* Else will try to reuse lookahead token after shifting the error
      token.  */
   goto yyerrlab1;

/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
   /* Pacify compilers when the user code never invokes YYERROR and the
      label yyerrorlab therefore never appears in user code.  */
   if( 0 )
      YYERROR;
   ++yynerrs;

   /* Do not reclaim the symbols of the rule whose action triggered
      this YYERROR.  */
   YYPOPSTACK( yylen );
   yylen = 0;
   YY_STACK_PRINT( yyss, yyssp );
   yystate = *yyssp;
   goto yyerrlab1;

/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
   yyerrstatus = 3; /* Each real token shifted decrements this.  */

   /* Pop stack until we find a state that shifts the error token.  */
   for( ;; )
   {
      yyn = yypact[ yystate ];
      if( !yypact_value_is_default( yyn ) )
      {
         yyn += YYSYMBOL_YYerror;
         if( 0 <= yyn && yyn <= YYLAST && yycheck[ yyn ] == YYSYMBOL_YYerror )
         {
            yyn = yytable[ yyn ];
            if( 0 < yyn )
               break;
         }
      }

      /* Pop the current state because it cannot handle the error token.  */
      if( yyssp == yyss )
         YYABORT;

      yyerror_range[ 1 ] = *yylsp;
      yydestruct( "Error: popping", YY_ACCESSING_SYMBOL( yystate ), yyvsp, yylsp, scanner, result );
      YYPOPSTACK( 1 );
      yystate = *yyssp;
      YY_STACK_PRINT( yyss, yyssp );
   }

   YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
   *++yyvsp = yylval;
   YY_IGNORE_MAYBE_UNINITIALIZED_END

   yyerror_range[ 2 ] = yylloc;
   ++yylsp;
   YYLLOC_DEFAULT( *yylsp, yyerror_range, 2 );

   /* Shift the error token.  */
   YY_SYMBOL_PRINT( "Shifting", YY_ACCESSING_SYMBOL( yyn ), yyvsp, yylsp );

   yystate = yyn;
   goto yynewstate;

/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
   yyresult = 0;
   goto yyreturnlab;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
   yyresult = 1;
   goto yyreturnlab;

/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
   yyerror( &yylloc, scanner, result, YY_( "memory exhausted" ) );
   yyresult = 2;
   goto yyreturnlab;

/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
   if( yychar != YYEMPTY )
   {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE( yychar );
      yydestruct( "Cleanup: discarding lookahead", yytoken, &yylval, &yylloc, scanner, result );
   }
   /* Do not reclaim the symbols of the rule whose action triggered
      this YYABORT or YYACCEPT.  */
   YYPOPSTACK( yylen );
   YY_STACK_PRINT( yyss, yyssp );
   while( yyssp != yyss )
   {
      yydestruct( "Cleanup: popping", YY_ACCESSING_SYMBOL( +*yyssp ), yyvsp, yylsp, scanner, result );
      YYPOPSTACK( 1 );
   }
#ifndef yyoverflow
   if( yyss != yyssa )
      YYSTACK_FREE( yyss );
#endif

   return yyresult;
}

#line 793 "dory.y"

/* Entrypoint: parse Program from file. */
Program pProgram( FILE *inp )
{
   YYSTYPE  result;
   yyscan_t scanner = dory__initialize_lexer( inp );
   if( !scanner )
   {
      fprintf( stderr, "Failed to initialize lexer.\n" );
      return 0;
   }
   int error = yyparse( scanner, &result );
   dory_lex_destroy( scanner );
   if( error )
   { /* Failure */
      return 0;
   }
   else
   { /* Success */
      return result.program_;
   }
}

/* Entrypoint: parse Program from string. */
Program psProgram( const char *str )
{
   YYSTYPE  result;
   yyscan_t scanner = dory__initialize_lexer( 0 );
   if( !scanner )
   {
      fprintf( stderr, "Failed to initialize lexer.\n" );
      return 0;
   }
   YY_BUFFER_STATE buf   = dory__scan_string( str, scanner );
   int             error = yyparse( scanner, &result );
   dory__delete_buffer( buf, scanner );
   dory_lex_destroy( scanner );
   if( error )
   { /* Failure */
      return 0;
   }
   else
   { /* Success */
      return result.program_;
   }
}

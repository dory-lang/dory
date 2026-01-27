/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_DORY_BISON_H_INCLUDED
#define YY_DORY_BISON_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
#   define YYDEBUG 1
#endif
#if YYDEBUG
extern int dory_debug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
#   define YYTOKENTYPE
enum yytokentype
{
   YYEMPTY         = -2,
   YYEOF           = 0,   /* "end of file"  */
   YYerror         = 256, /* error  */
   YYUNDEF         = 257, /* "invalid token"  */
   _ERROR_         = 258, /* _ERROR_  */
   _BANGEQ         = 259, /* _BANGEQ  */
   _HASH           = 260, /* _HASH  */
   _DOLLAR         = 261, /* _DOLLAR  */
   _PERCENT        = 262, /* _PERCENT  */
   _PERCENTEQ      = 263, /* _PERCENTEQ  */
   _AMP            = 264, /* _AMP  */
   _LPAREN         = 265, /* _LPAREN  */
   _RPAREN         = 266, /* _RPAREN  */
   _STAR           = 267, /* _STAR  */
   _DSTAR          = 268, /* _DSTAR  */
   _SYMB_8         = 269, /* _SYMB_8  */
   _STAREQ         = 270, /* _STAREQ  */
   _PLUS           = 271, /* _PLUS  */
   _DPLUS          = 272, /* _DPLUS  */
   _PLUSEQ         = 273, /* _PLUSEQ  */
   _COMMA          = 274, /* _COMMA  */
   _MINUS          = 275, /* _MINUS  */
   _DMINUS         = 276, /* _DMINUS  */
   _MINUSEQ        = 277, /* _MINUSEQ  */
   _DOT            = 278, /* _DOT  */
   _SYMB_16        = 279, /* _SYMB_16  */
   _SYMB_46        = 280, /* _SYMB_46  */
   _SYMB_17        = 281, /* _SYMB_17  */
   _SYMB_15        = 282, /* _SYMB_15  */
   _SYMB_45        = 283, /* _SYMB_45  */
   _SLASH          = 284, /* _SLASH  */
   _SLASHEQ        = 285, /* _SLASHEQ  */
   _COLON          = 286, /* _COLON  */
   _COLONEQ        = 287, /* _COLONEQ  */
   _LT             = 288, /* _LT  */
   _LDARROW        = 289, /* _LDARROW  */
   _LTGT           = 290, /* _LTGT  */
   _EQ             = 291, /* _EQ  */
   _DEQ            = 292, /* _DEQ  */
   _GT             = 293, /* _GT  */
   _GTEQ           = 294, /* _GTEQ  */
   _LBRACK         = 295, /* _LBRACK  */
   _RBRACK         = 296, /* _RBRACK  */
   _CARET          = 297, /* _CARET  */
   _CARETEQ        = 298, /* _CARETEQ  */
   _KW_begin       = 299, /* _KW_begin  */
   _KW_break       = 300, /* _KW_break  */
   _KW_case        = 301, /* _KW_case  */
   _KW_class       = 302, /* _KW_class  */
   _KW_create      = 303, /* _KW_create  */
   _KW_descend     = 304, /* _KW_descend  */
   _KW_do          = 305, /* _KW_do  */
   _KW_each        = 306, /* _KW_each  */
   _KW_else        = 307, /* _KW_else  */
   _KW_elseif      = 308, /* _KW_elseif  */
   _KW_end         = 309, /* _KW_end  */
   _KW_endcase     = 310, /* _KW_endcase  */
   _KW_endclass    = 311, /* _KW_endclass  */
   _KW_enddo       = 312, /* _KW_enddo  */
   _KW_endenum     = 313, /* _KW_endenum  */
   _KW_endfor      = 314, /* _KW_endfor  */
   _KW_endif       = 315, /* _KW_endif  */
   _KW_endsequence = 316, /* _KW_endsequence  */
   _KW_endstruct   = 317, /* _KW_endstruct  */
   _KW_endswitch   = 318, /* _KW_endswitch  */
   _KW_endwhile    = 319, /* _KW_endwhile  */
   _KW_enum        = 320, /* _KW_enum  */
   _KW_exit        = 321, /* _KW_exit  */
   _SYMB_9         = 322, /* _SYMB_9  */
   _KW_for         = 323, /* _KW_for  */
   _KW_from        = 324, /* _KW_from  */
   _KW_function    = 325, /* _KW_function  */
   _SYMB_11        = 326, /* _SYMB_11  */
   _KW_if          = 327, /* _KW_if  */
   _KW_in          = 328, /* _KW_in  */
   _KW_inherit     = 329, /* _KW_inherit  */
   _KW_local       = 330, /* _KW_local  */
   _KW_loop        = 331, /* _KW_loop  */
   _KW_method      = 332, /* _KW_method  */
   _KW_nil         = 333, /* _KW_nil  */
   _KW_otherwise   = 334, /* _KW_otherwise  */
   _KW_procedure   = 335, /* _KW_procedure  */
   _SYMB_12        = 336, /* _SYMB_12  */
   _KW_recover     = 337, /* _KW_recover  */
   _KW_return      = 338, /* _KW_return  */
   _KW_sequence    = 339, /* _KW_sequence  */
   _KW_static      = 340, /* _KW_static  */
   _KW_step        = 341, /* _KW_step  */
   _KW_struct      = 342, /* _KW_struct  */
   _KW_switch      = 343, /* _KW_switch  */
   _KW_to          = 344, /* _KW_to  */
   _KW_using       = 345, /* _KW_using  */
   _KW_var         = 346, /* _KW_var  */
   _SYMB_10        = 347, /* _SYMB_10  */
   _KW_while       = 348, /* _KW_while  */
   _LBRACE         = 349, /* _LBRACE  */
   _BAR            = 350, /* _BAR  */
   _RBRACE         = 351, /* _RBRACE  */
   T_FloatLit      = 352, /* T_FloatLit  */
   T_IntLit        = 353, /* T_IntLit  */
   T_LineEol       = 354, /* T_LineEol  */
   T_PreprocLine   = 355, /* T_PreprocLine  */
   T_StrLit        = 356, /* T_StrLit  */
   _IDENT_         = 357  /* _IDENT_  */
};
typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if !defined YYSTYPE && !defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#   line 51 "dory.y"

   int                _int;
   char               _char;
   double             _double;
   char              *_string;
   Program            program_;
   LeadingEols        leadingeols_;
   TopDecls           topdecls_;
   TopDecl            topdecl_;
   Defs               defs_;
   PreprocDecl        preprocdecl_;
   ParamListOpt       paramlistopt_;
   ParamListTail      paramlisttail_;
   Stmt               stmt_;
   ReturnExprOpt      returnexpropt_;
   StmtList           stmtlist_;
   StmtListNonEmpty   stmtlistnonempty_;
   ElseIfList         elseiflist_;
   ElseIf             elseif_;
   ElseOpt            elseopt_;
   IfCond             ifcond_;
   EndIf              endif_;
   EndIfOpt           endifopt_;
   DoOpt              doopt_;
   WhileCond          whilecond_;
   EndDo              enddo_;
   EndDoOpt           enddoopt_;
   EndCase            endcase_;
   EndCaseOpt         endcaseopt_;
   StepOpt            stepopt_;
   EndFor             endfor_;
   EndForOpt          endforopt_;
   ForEachVars        foreachvars_;
   ForEachVarsTail    foreachvarstail_;
   ForEachExprs       foreachexprs_;
   ForEachExprsTail   foreachexprstail_;
   ForEachDescOpt     foreachdescopt_;
   CaseList           caselist_;
   CaseClause         caseclause_;
   CaseCondList       casecondlist_;
   CaseCondClause     casecondclause_;
   OtherwiseOpt       otherwiseopt_;
   BreakExprOpt       breakexpropt_;
   RecoverOpt         recoveropt_;
   RecoverUsingOpt    recoverusingopt_;
   EndSequence        endsequence_;
   EndSequenceOpt     endsequenceopt_;
   SwitchCond         switchcond_;
   EndSwitch          endswitch_;
   EndSwitchOpt       endswitchopt_;
   EndStruct          endstruct_;
   EndStructOpt       endstructopt_;
   EndClass           endclass_;
   EndClassOpt        endclassopt_;
   EndEnum            endenum_;
   EndEnumOpt         endenumopt_;
   CaseValue          casevalue_;
   StmtEnd            stmtend_;
   EnumDecl           enumdecl_;
   EnumListOpt        enumlistopt_;
   EnumListTail       enumlisttail_;
   EnumSep            enumsep_;
   EnumItem           enumitem_;
   EnumValueOpt       enumvalueopt_;
   LineEolOpt         lineeolopt_;
   StructDecl         structdecl_;
   StructBody         structbody_;
   StructMember       structmember_;
   FieldInitList      fieldinitlist_;
   FieldInit          fieldinit_;
   StaticDecl         staticdecl_;
   ClassDecl          classdecl_;
   CreateOpt          createopt_;
   ClassName          classname_;
   InheritOpt         inheritopt_;
   InheritKeyword     inheritkeyword_;
   StaticClassOpt     staticclassopt_;
   ClassBody          classbody_;
   ClassMember        classmember_;
   VisibilityScope    visibilityscope_;
   VarList            varlist_;
   VarDecl            vardecl_;
   VarInitOpt         varinitopt_;
   MethodParamsOpt    methodparamsopt_;
   Expr               expr_;
   ExprOr             expror_;
   CallExpr           callexpr_;
   ArgListOpt         arglistopt_;
   ArgListTail        arglisttail_;
   ExprListOpt        exprlistopt_;
   ExprListTail       exprlisttail_;
   QualifiedId        qualifiedid_;
   LHS                lhs_;
   LValue             lvalue_;
   SubscriptList      subscriptlist_;
   SubscriptListTail  subscriptlisttail_;
   Literal            literal_;
   Napis              napis_;
   Def                def_;
   FunctionDef        functiondef_;
   ProcedureDef       proceduredef_;
   StaticOpt          staticopt_;
   LocalDeclsOpt      localdeclsopt_;
   LocalDecl          localdecl_;
   StaticDeclsOpt     staticdeclsopt_;
   StaticItemList     staticitemlist_;
   StaticItemTail     staticitemtail_;
   StaticItem         staticitem_;
   StaticArrayDimsOpt staticarraydimsopt_;
   StaticInitOpt      staticinitopt_;
   IdentInitList      identinitlist_;
   IdentInitTail      identinittail_;
   IdentInit          identinit_;
   InitOpt            initopt_;
   MethodDef          methoddef_;

#   line 283 "Bison.h"
};
typedef union YYSTYPE YYSTYPE;
#   define YYSTYPE_IS_TRIVIAL  1
#   define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if !defined YYLTYPE && !defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
   int first_line;
   int first_column;
   int last_line;
   int last_column;
};
#   define YYLTYPE_IS_DECLARED 1
#   define YYLTYPE_IS_TRIVIAL  1
#endif

int dory_parse( yyscan_t scanner, YYSTYPE *result );

#endif /* !YY_DORY_BISON_H_INCLUDED  */

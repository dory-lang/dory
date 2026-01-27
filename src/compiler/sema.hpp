/*
 * sema.hpp - Minimal semantic analysis skeleton
 */

#ifndef SEMA_HPP_
#define SEMA_HPP_

#include "Absyn.h"

#include <stdbool.h>

typedef struct _SymbolTable SymbolTable;

SymbolTable *sema_CreateSymbolTable();
void         sema_DestroySymbolTable( SymbolTable *pTable );

bool sema_ValidateAst( Program pRoot );
void sema_ReportError( const char *szMessage, int nLine );

#endif /* SEMA_HPP_ */

/*
 * types.cpp - Minimal type stubs
 *
 * Creates/frees TypeInfo and sets default TYPE_VARIANT.
 * Skeleton for the future type system.
 */

#include "types.hpp"

#include <stdlib.h>

TypeInfo *type_New( const char *szName )
{
   ( void ) szName;
   TypeInfo *pType = ( TypeInfo * ) calloc( 1, sizeof( TypeInfo ) );
   if( !pType )
      return NULL;

   pType->nDataType = TYPE_VARIANT;
   return pType;
}

void type_Free( TypeInfo *pType )
{
   free( pType );
}

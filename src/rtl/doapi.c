/*
 * doapi.c - basic API implementation for the Dory virtual machine
 */

#include "doapi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ----------------------------------------------------------------------------
   ------------------------------------------------------------------------- */
// do_xalloc: Allocates memory and returns NULL on failure.
void *do_xalloc( size_t nSize )
{
   if( nSize == 0 )
      nSize = 1;
   return malloc( nSize );
}

// do_xfree: Frees allocated memory.
void do_xfree( void *pMem )
{
   free( pMem );
}

// do_xgrab: Allocates memory and aborts on failure.
void *do_xgrab( size_t nSize )
{
   void *pMem = do_xalloc( nSize );
   if( !pMem )
   {
      fprintf( stderr, "do_xgrab: out of memory (%zu bytes)\n", nSize );
      abort();
   }
   return pMem;
}

// do_xunlock: Unlocks virtual memory segments.
void do_xunlock( void *pMem )
{
   ( void ) pMem;
}

/* DO_UNUSED_FUNC temporary */
static char *DO_UNUSED_FUNC do_strdup( const char *szSrc )
{
   if( !szSrc )
      return NULL;
   size_t nLen  = strlen( szSrc );
   char  *szDst = ( char * ) malloc( nLen + 1 );
   if( szDst )
      memcpy( szDst, szSrc, nLen + 1 );
   return szDst;
}

/* ----------------------------------------------------------------------------
   Parameter values
   ------------------------------------------------------------------------- */
// do_param  : Retrieves a parameter from the VM with type checking.
// TODO:      No handling for out-of-range indices (currently returns NULL).
// Idea:      Add error logging for invalid indices.
// Priority:  Medium.
const DO_ITEM *do_param( DO_VM *pVm, int nIdx1Based, DO_TYPE nIt )
{
   int iIndex = nIdx1Based - 1;
   if( !pVm || iIndex < 0 || iIndex >= pVm->nArgc )
      return NULL;

   const DO_ITEM *pItem = &pVm->apArgv[ iIndex ];
   if( nIt == DO_IT_ANY )
      return pItem;

   DO_TYPE nType     = pItem->nType;
   DO_TYPE nReqFlags = nIt & ~DO_KIND_MASK;

   if( DO_KIND_OF( nType ) != DO_KIND_OF( nIt ) )
      return NULL;
   if( nReqFlags && ( ( nType & nReqFlags ) != nReqFlags ) )
      return NULL;
   return pItem;
}

// do_parc: Returns a parameter as a string (char*).
const char *do_parc( DO_VM *pVm, int nIdx1Based )
{
   const DO_ITEM *pItem = do_param( pVm, nIdx1Based, DO_IT_STRING );
   return pItem ? pItem->u.szValue : NULL;
}

// do_parclen: Returns the length of the string parameter.
size_t do_parclen( DO_VM *pVm, int nIdx1Based )
{
   const DO_ITEM *pItem = do_param( pVm, nIdx1Based, DO_IT_STRING );
   return pItem && pItem->u.szValue ? strlen( pItem->u.szValue ) : 0;
}

// do_parcsiz: Returns the allocated size for a string parameter.
size_t do_parcsiz( DO_VM *pVm, int nIdx1Based )
{
   const DO_ITEM *pItem = do_param( pVm, nIdx1Based, DO_IT_STRING );
   return pItem && pItem->u.szValue ? ( strlen( pItem->u.szValue ) + 1 ) : 0;
}

// do_parinfo: Returns parameter type or argument count.
DO_TYPE do_parinfo( DO_VM *pVm, int nIdx1Based )
{
   const DO_ITEM *pItem = do_param( pVm, nIdx1Based, DO_IT_ANY );
   return pItem ? pItem->nType : DO_IT_NIL;
}

// do_parl: Returns a logical parameter as int.
int do_parl( DO_VM *pVm, int nIdx1Based, int *pOkay )
{
   const DO_ITEM *pItem = do_param( pVm, nIdx1Based, DO_IT_LOGICAL );
   if( pItem )
   {
      if( pOkay )
         *pOkay = 1;
      return pItem->u.nValue ? 1 : 0;
   }
   // Try conversion from integer.
   int     lOkay = 0;
   int64_t n     = do_parni( pVm, nIdx1Based, &lOkay );
   if( lOkay )
   {
      if( pOkay )
         *pOkay = 1;
      return n != 0;
   }

   n = do_parnl( pVm, nIdx1Based, &lOkay );
   if( lOkay )
   {
      if( pOkay )
         *pOkay = 1;
      return n != 0;
   }

   if( pOkay )
      *pOkay = 0;
   return 0;
}

// do_parnd: Returns a numeric parameter as double.
double do_parnd( DO_VM *pVm, int nIdx1Based, int *pOkay )
{
   const DO_ITEM *pItem = do_param( pVm, nIdx1Based, DO_IT_DOUBLE );
   if( pItem )
   {
      if( pOkay )
         *pOkay = 1;
      return pItem->u.dValue;
   }
   // Try conversion from integer.
   int     lOkay = 0;
   int64_t n     = do_parni( pVm, nIdx1Based, &lOkay );
   if( lOkay )
   {
      if( pOkay )
         *pOkay = 1;
      return ( double ) n;
   }

   n = do_parnl( pVm, nIdx1Based, &lOkay );
   if( lOkay )
   {
      if( pOkay )
         *pOkay = 1;
      return ( double ) n;
   }
   // Try conversion from string.
   pItem = do_param( pVm, nIdx1Based, DO_IT_STRING );
   if( pItem && pItem->u.szValue )
   {
      char  *pEnd = NULL;
      double d    = strtod( pItem->u.szValue, &pEnd );
      if( pEnd && pEnd != pItem->u.szValue )
      {
         if( pOkay )
            *pOkay = 1;
         return d;
      }
   }

   if( pOkay )
      *pOkay = 0;
   return 0.0;
}

// do_parni: Returns a numeric parameter as int64_t.
int64_t do_parni( DO_VM *pVm, int nIdx1Based, int *pOkay )
{
   const DO_ITEM *pItem = do_param( pVm, nIdx1Based, DO_IT_ANY );
   if( !pItem || !do_is_numint( pItem->nType ) )
   {
      if( pOkay )
         *pOkay = 0;
      return 0;
   }
   if( pOkay )
      *pOkay = 1;
   return pItem->u.nValue;
}

// do_parnl: Returns a numeric parameter as long.
int64_t do_parnl( DO_VM *pVm, int nIdx1Based, int *pOkay )
{
   const DO_ITEM *pItem = do_param( pVm, nIdx1Based, DO_IT_LONG );
   if( !pItem )
   {
      if( pOkay )
         *pOkay = 0;
      return 0;
   }
   if( pOkay )
      *pOkay = 1;
   return pItem->u.nValue;
}

// do_parptr: Returns a pointer parameter.
void *do_parptr( DO_VM *pVm, int nIdx1Based, int *pOkay )
{
   const DO_ITEM *pItem = do_param( pVm, nIdx1Based, DO_IT_POINTER );
   if( !pItem )
   {
      if( pOkay )
         *pOkay = 0;
      return NULL;
   }
   if( pOkay )
      *pOkay = 1;
   return pItem->u.pValue;
}

/* ----------------------------------------------------------------------------
   ------------------------------------------------------------------------- */
// do_retnil: Returns NIL.
void do_retnil( DO_VM *pVm )
{
   if( !pVm )
      return;
   pVm->oRet.nType    = DO_IT_NIL;
   pVm->oRet.u.nValue = 0;
}

// do_retl: Returns a logical value.
void do_retl( DO_VM *pVm, int bValue )
{
   if( !pVm )
      return;
   pVm->oRet.nType    = DO_IT_LOGICAL;
   pVm->oRet.u.nValue = bValue ? 1 : 0;
}

// do_retni: Returns an integer value.
void do_retni( DO_VM *pVm, int64_t nValue )
{
   if( !pVm )
      return;
   pVm->oRet.nType    = DO_IT_LONG;
   pVm->oRet.u.nValue = nValue;
}

// do_retc: Returns a string.
void do_retc( DO_VM *pVm, const char *szValue )
{
   if( !pVm )
      return;
   pVm->oRet.nType     = DO_IT_STRING;
   pVm->oRet.u.szValue = szValue ? szValue : "";
}

// do_retptr: Returns a pointer value.
void do_retptr( DO_VM *pVm, void *pValue )
{
   if( !pVm )
      return;
   pVm->oRet.nType       = DO_IT_POINTER;
   pVm->oRet.nReserved   = 0;
   pVm->oRet.pStructType = NULL;
   pVm->oRet.u.pValue    = pValue;
}

/* ----------------------------------------------------------------------------
   ------------------------------------------------------------------------- */
// do_err_args: Reports invalid arguments.
void do_err_args( const char *szFuncName )
{
   fprintf( stderr, "%s: invalid arguments\n", szFuncName ? szFuncName : "<?>" );
}

// do_err_type: Reports an invalid type.
void do_err_type( const char *szFuncName, const char *szExpected, DO_TYPE nIt )
{
   fprintf( stderr, "Error: %s: expected %s, got type %u\n", szFuncName ? szFuncName : "<?>",
            szExpected ? szExpected : "<?>", DO_KIND( nIt ) );
}

// TODO:
/*

do_retclen:         Post a character return value with explicit length
do_retds:           Post a date return value using a date string
do_retl:            Post a logical return value
do_retnd:           Post a numeric return value using a double
do_retni:           Post a numeric return value using an integer
do_retnl:           Post a numeric return value using a long

do_storc:           Assign a null-terminated string to a referenced variable
do_storclen:        Assign string of explicit length to a referenced variable
do_stords:          Assign a date string to a referenced variable
do_storl:           Assign a logical value to a referenced variable
do_stornd:          Assign a numeric to a variable using double-precision type
do_storni:          Assign a numeric to a referenced variable using an integer
do_stornl:          Assign a numeric to a referenced variable using a long

*/

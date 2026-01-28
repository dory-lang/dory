/*
 * doapi.c - basic API implementation for the Dory virtual machine
 */

#include "doapi.h"
#include <inttypes.h>
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

// do_retarray: Returns an array value.
void do_retarray( DO_VM *pVm, DO_ARRAY *pArr )
{
   if( !pVm )
      return;
   pVm->oRet.nType       = DO_IT_ARRAY;
   pVm->oRet.nReserved   = 0;
   pVm->oRet.pStructType = NULL;
   pVm->oRet.u.pValue    = pArr;
}

/* ----------------------------------------------------------------------------
   Array helpers
   ------------------------------------------------------------------------- */
// do_array_new: Creates a new array container.
DO_ARRAY *do_array_new( int64_t nLen )
{
   if( nLen < 0 )
      nLen = 0;

   DO_ARRAY *pArr = ( DO_ARRAY * ) do_xgrab( sizeof( DO_ARRAY ) );
   pArr->nLen     = nLen;
   pArr->pItems   = NULL;

   if( nLen > 0 )
   {
      pArr->pItems = ( DO_ITEM * ) do_xgrab( sizeof( DO_ITEM ) * ( size_t ) nLen );
      memset( pArr->pItems, 0, sizeof( DO_ITEM ) * ( size_t ) nLen );
   }

   return pArr;
}

// do_array_at: Returns a 1-based element pointer or NULL if out of range.
DO_ITEM *do_array_at( DO_ARRAY *pArr, int64_t nIdx1Based )
{
   if( !pArr || nIdx1Based < 1 || nIdx1Based > pArr->nLen )
      return NULL;
   return &pArr->pItems[ nIdx1Based - 1 ];
}

// do_array_len: Returns array length.
int64_t do_array_len( DO_ARRAY *pArr )
{
   return pArr ? pArr->nLen : 0;
}

// do_array_acopy: Shallow copy from pSrc into pDst.
void do_array_acopy( DO_ARRAY *pDst, int64_t nDstPos, const DO_ARRAY *pSrc, int64_t nSrcPos, int64_t nCount )
{
   if( !pDst || !pSrc || nCount <= 0 )
      return;

   if( nDstPos < 1 || nSrcPos < 1 )
      return;

   int64_t nDstEnd = nDstPos + nCount - 1;
   int64_t nSrcEnd = nSrcPos + nCount - 1;
   if( nDstEnd > pDst->nLen || nSrcEnd > pSrc->nLen )
      return;

   for( int64_t i = 0; i < nCount; ++i )
   {
      pDst->pItems[ nDstPos - 1 + i ] = pSrc->pItems[ nSrcPos - 1 + i ];
   }
}

// do_array_aclone: Deep clone for nested arrays.
DO_ARRAY *do_array_aclone( const DO_ARRAY *pSrc )
{
   if( !pSrc )
      return NULL;

   DO_ARRAY *pDst = do_array_new( pSrc->nLen );
   for( int64_t i = 0; i < pSrc->nLen; ++i )
   {
      DO_ITEM oItem = pSrc->pItems[ i ];
      if( DO_KIND_OF( oItem.nType ) == DO_K_ARRAY && oItem.u.pValue )
      {
         DO_ARRAY *pChild = do_array_aclone( ( const DO_ARRAY * ) oItem.u.pValue );
         oItem.u.pValue   = pChild;
      }
      pDst->pItems[ i ] = oItem;
   }
   return pDst;
}

static int do_item_equal_deep( const DO_ITEM *pLeft, const DO_ITEM *pRight )
{
   if( !pLeft || !pRight )
      return 0;

   DO_KIND kLeft  = DO_KIND_OF( pLeft->nType );
   DO_KIND kRight = DO_KIND_OF( pRight->nType );
   if( kLeft != kRight )
      return 0;

   switch( kLeft )
   {
   case DO_K_NIL:
      return 1;
   case DO_K_I32:
   case DO_K_I64:
   case DO_K_LOGICAL:
      return pLeft->u.nValue == pRight->u.nValue;
   case DO_K_STRING:
      if( pLeft->u.szValue == NULL || pRight->u.szValue == NULL )
         return pLeft->u.szValue == pRight->u.szValue;
      return strcmp( pLeft->u.szValue, pRight->u.szValue ) == 0;
   case DO_K_ARRAY:
      return do_array_equal_deep( ( const DO_ARRAY * ) pLeft->u.pValue, ( const DO_ARRAY * ) pRight->u.pValue );
   default:
      return pLeft->u.pValue == pRight->u.pValue;
   }
}

// do_array_equal_deep: Deep equality for nested arrays.
int do_array_equal_deep( const DO_ARRAY *pLeft, const DO_ARRAY *pRight )
{
   if( pLeft == pRight )
      return 1;
   if( !pLeft || !pRight )
      return 0;
   if( pLeft->nLen != pRight->nLen )
      return 0;

   for( int64_t i = 0; i < pLeft->nLen; ++i )
   {
      if( !do_item_equal_deep( &pLeft->pItems[ i ], &pRight->pItems[ i ] ) )
         return 0;
   }
   return 1;
}

static DO_ARRAY *do_param_array( DO_VM *pVm, int nIdx1Based, int *pOkay )
{
   const DO_ITEM *pItem = do_param( pVm, nIdx1Based, DO_IT_ARRAY );
   if( !pItem )
   {
      if( pOkay )
         *pOkay = 0;
      return NULL;
   }
   if( pOkay )
      *pOkay = 1;
   return ( DO_ARRAY * ) pItem->u.pValue;
}

// ACopy: Shallow copy elements between arrays.
DO_FUNC( ACOPY )
{
   if( !pVm || pVm->nArgc < 2 )
   {
      do_err_args( "ACopy" );
      do_retnil( pVm );
      return;
   }

   int       lOkSrc = 0;
   int       lOkDst = 0;
   DO_ARRAY *pSrc   = do_param_array( pVm, 1, &lOkSrc );
   DO_ARRAY *pDst   = do_param_array( pVm, 2, &lOkDst );
   if( !lOkSrc || !lOkDst || !pSrc || !pDst )
   {
      do_err_type( "ACopy", "array", DO_IT_NIL );
      do_retnil( pVm );
      return;
   }

   int     lOkay   = 0;
   int64_t nSrcPos = 1;
   int64_t nCount  = 0;
   int64_t nDstPos = 1;

   if( pVm->nArgc >= 3 )
   {
      nSrcPos = do_parni( pVm, 3, &lOkay );
      if( !lOkay )
      {
         do_err_args( "ACopy" );
         do_retnil( pVm );
         return;
      }
   }
   if( pVm->nArgc >= 4 )
   {
      nCount = do_parni( pVm, 4, &lOkay );
      if( !lOkay )
      {
         do_err_args( "ACopy" );
         do_retnil( pVm );
         return;
      }
   }
   if( pVm->nArgc >= 5 )
   {
      nDstPos = do_parni( pVm, 5, &lOkay );
      if( !lOkay )
      {
         do_err_args( "ACopy" );
         do_retnil( pVm );
         return;
      }
   }

   if( nCount <= 0 )
   {
      int64_t nSrcAvail = pSrc->nLen - nSrcPos + 1;
      int64_t nDstAvail = pDst->nLen - nDstPos + 1;
      nCount            = nSrcAvail < nDstAvail ? nSrcAvail : nDstAvail;
   }

   do_array_acopy( pDst, nDstPos, pSrc, nSrcPos, nCount );
   do_retarray( pVm, pDst );
}

// AClone: Deep clone nested arrays.
DO_FUNC( ACLONE )
{
   int       lOk  = 0;
   DO_ARRAY *pSrc = do_param_array( pVm, 1, &lOk );
   if( !lOk || !pSrc )
   {
      do_err_type( "AClone", "array", DO_IT_NIL );
      do_retnil( pVm );
      return;
   }

   DO_ARRAY *pNew = do_array_aclone( pSrc );
   do_retarray( pVm, pNew );
}

// AEqual: Deep equality for arrays.
DO_FUNC( AEQUAL )
{
   int       lOk1   = 0;
   int       lOk2   = 0;
   DO_ARRAY *pLeft  = do_param_array( pVm, 1, &lOk1 );
   DO_ARRAY *pRight = do_param_array( pVm, 2, &lOk2 );
   if( !lOk1 || !lOk2 )
   {
      do_err_type( "AEqual", "array", DO_IT_NIL );
      do_retl( pVm, 0 );
      return;
   }

   do_retl( pVm, do_array_equal_deep( pLeft, pRight ) ? 1 : 0 );
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

// do_err_bounds: Reports out-of-range index for arrays.
void do_err_bounds( const char *szFuncName, int64_t nIndex, int64_t nLen )
{
   fprintf( stderr, "Error: %s: index %" PRId64 " out of range (1..%" PRId64 ")\n", szFuncName ? szFuncName : "<?>",
            nIndex, nLen );
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

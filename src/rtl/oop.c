/*
 * oop.c
 */

#include "oop.h"

DO_ENUM *do_enum_new_array( DO_ARRAY *pArray, int bDescend )
{
   DO_ENUM *pEnum  = ( DO_ENUM * ) do_xgrab( sizeof( DO_ENUM ) );
   pEnum->pArray   = pArray;
   pEnum->nLen     = pArray ? pArray->nLen : 0;
   pEnum->bDescend = bDescend ? 1 : 0;
   pEnum->nIndex   = pEnum->bDescend ? pEnum->nLen + 1 : 0;
   return pEnum;
}

void do_enum_free( DO_ENUM *pEnum )
{
   if( !pEnum )
      return;
   do_xfree( pEnum );
}

int do_enum_step( DO_ENUM *pEnum )
{
   if( !pEnum || pEnum->nLen <= 0 )
      return 0;

   if( pEnum->bDescend )
      --pEnum->nIndex;
   else
      ++pEnum->nIndex;

   return ( pEnum->nIndex >= 1 && pEnum->nIndex <= pEnum->nLen ) ? 1 : 0;
}

void do_enum_set_index( DO_ENUM *pEnum, int64_t nIndex )
{
   if( !pEnum )
      return;
   pEnum->nIndex = nIndex;
}

int64_t do_enum_index( const DO_ENUM *pEnum )
{
   return pEnum ? pEnum->nIndex : 0;
}

int do_enum_isfirst( const DO_ENUM *pEnum )
{
   if( !pEnum || pEnum->nLen <= 0 )
      return 0;
   return pEnum->nIndex == ( pEnum->bDescend ? pEnum->nLen : 1 );
}

int do_enum_islast( const DO_ENUM *pEnum )
{
   if( !pEnum || pEnum->nLen <= 0 )
      return 0;
   return pEnum->nIndex == ( pEnum->bDescend ? 1 : pEnum->nLen );
}

DO_ITEM *do_enum_value_ptr( DO_ENUM *pEnum )
{
   if( !pEnum || !pEnum->pArray )
      return NULL;
   return do_array_at( pEnum->pArray, pEnum->nIndex );
}

DO_ARRAY *do_enum_base( DO_ENUM *pEnum )
{
   return pEnum ? pEnum->pArray : NULL;
}

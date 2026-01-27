/*
 * ptr.c
 */

#include "doapi.h"
#include <stdlib.h>

DO_FUNC( PEEK )
{
   const DO_ITEM *pArg = do_param( pVm, 1, DO_IT_POINTER );
   if( !pArg || !DO_HASFLAG( pArg->nType, DO_F_BYREF ) )
   {
      do_err_type( "peek", "pointer byref", pArg ? pArg->nType : DO_IT_NIL );
      do_retnil( pVm );
      return;
   }
   DO_ITEM *pItem = ( DO_ITEM * ) pArg->u.pValue;
   if( !pItem )
   {
      do_err_args( "peek" );
      do_retnil( pVm );
      return;
   }
   pVm->oRet = *pItem;
}

DO_FUNC( POKE )
{
   const DO_ITEM *pArg = do_param( pVm, 1, DO_IT_POINTER );
   if( !pArg || !DO_HASFLAG( pArg->nType, DO_F_BYREF ) )
   {
      do_err_type( "poke", "pointer byref", pArg ? pArg->nType : DO_IT_NIL );
      do_retnil( pVm );
      return;
   }
   DO_ITEM *pItem = ( DO_ITEM * ) pArg->u.pValue;
   if( !pItem )
   {
      do_err_args( "poke" );
      do_retnil( pVm );
      return;
   }

   const DO_ITEM *pVal = do_param( pVm, 2, DO_IT_ANY );
   if( !pVal )
   {
      do_err_args( "poke" );
      do_retnil( pVm );
      return;
   }
   *pItem = *pVal;
   do_retnil( pVm );
}

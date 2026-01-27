/*
 * string.c
 */

#include "doapi.h"
#include <string.h>

DO_FUNC( MEMSET )
{
   int   lOkay = 0;
   void *pMem  = do_parptr( pVm, 1, &lOkay );
   if( !lOkay || !pMem )
   {
      do_err_args( "memset" );
      do_retnil( pVm );
      return;
   }

   int64_t nByte = do_parni( pVm, 2, &lOkay );
   if( !lOkay )
   {
      do_err_args( "memset" );
      do_retnil( pVm );
      return;
   }

   int64_t nLen = do_parni( pVm, 3, &lOkay );
   if( !lOkay || nLen < 0 )
   {
      do_err_args( "memset" );
      do_retnil( pVm );
      return;
   }

   memset( pMem, ( int ) nByte, ( size_t ) nLen );
   do_retnil( pVm );
}

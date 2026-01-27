/*
 * stdlib.c
 */

#include "doapi.h"
#include <stdlib.h>

DO_FUNC( MALLOC )
{
   int     lOkay  = 0;
   int64_t nValue = do_parni( pVm, 1, &lOkay );
   if( !lOkay || nValue < 0 )
   {
      do_err_args( "malloc" );
      do_retptr( pVm, NULL );
      return;
   }
   void *pMem = malloc( ( size_t ) nValue );
   do_retptr( pVm, pMem );
}

DO_FUNC( FREE )
{
   int   lOkay = 0;
   void *pMem  = do_parptr( pVm, 1, &lOkay );
   if( !lOkay )
   {
      do_err_args( "free" );
      do_retnil( pVm );
      return;
   }
   free( pMem );
   do_retnil( pVm );
}

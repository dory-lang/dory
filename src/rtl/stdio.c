/*
 * stdio.c
 */

#include "doapi.h"
#include <inttypes.h>
#include <stdio.h>

// printf()  : Writes a formatted string to the console
// TODO:       Handles only one argument, no formatting.
// Idea:       Implement full printf-style formatting.
// Priority:   Low.
DO_FUNC( PRINTF )
{
   const char *szText = do_parc( pVm, 1 );
   if( szText )
   {
      fputs( szText, stdout );
      fputc( '\n', stdout );
   }
   else
   {
      int     lOkay  = 0;
      int64_t nValue = do_parni( pVm, 1, &lOkay );
      if( lOkay )
      {
         fprintf( stdout, "%" PRId64 "\n", nValue );
      }
      else
      {
         do_err_args( "printf" );
      }
   }
   do_retnil( pVm );
}

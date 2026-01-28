/*
 * stdio.c
 */

#include "doapi.h"
#include <inttypes.h>
#include <stdio.h>

// printf()  : Writes a formatted string to the console
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
      do_err_args( "printf" );
   }

   do_retnil( pVm );
}

/*
 * main.cpp - Minimal entry point
 *
 * Parses a .prg file and optionally prints AST or its dump.
 * Runs semantic checks, codegen, and (optionally) linking via clang.
 */

extern "C"
{
#include "Absyn.h"
#include "Parser.h"
#include "Printer.h"
}

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "ast.hpp"
#include "codegen.hpp"
#include "platform.h"
#include "sema.hpp"

static bool read_file_into_string( FILE *pIn, std::string &oOut )
{
   if( !pIn )
      return false;
   if( fseek( pIn, 0, SEEK_END ) != 0 )
      return false;
   long nSize = ftell( pIn );
   if( nSize < 0 )
      return false;
   if( fseek( pIn, 0, SEEK_SET ) != 0 )
      return false;

   oOut.clear();
   if( nSize == 0 )
      return true;
   oOut.resize( ( size_t ) nSize );
   size_t nRead = fread( &oOut[ 0 ], 1, ( size_t ) nSize, pIn );
   return nRead == ( size_t ) nSize;
}

static bool has_extension( const std::string &oPath )
{
   size_t nSep = oPath.find_last_of( "/\\" );
   size_t nDot = oPath.find_last_of( '.' );
   return nDot != std::string::npos && ( nSep == std::string::npos || nDot > nSep );
}

static std::string strip_extension( const std::string &oPath )
{
   if( !has_extension( oPath ) )
      return oPath;

   size_t nSep = oPath.find_last_of( "/\\" );
   size_t nDot = oPath.find_last_of( '.' );
   if( nSep != std::string::npos && nDot <= nSep )
      return oPath;
   return oPath.substr( 0, nDot );
}

int main( int nArgc, char **apszArgv )
{
   const char              *szInput    = NULL;
   const char              *szOutput   = NULL;
   const char              *szIrOut    = NULL;
   const char              *szObjOut   = NULL;
   const char              *szBinOut   = NULL;
   bool                     lAstStdout = false;
   bool                     lDumpAst   = false;
   bool                     lIrStdout  = false;
   std::vector<std::string> aLibDirs;
   std::vector<std::string> aLibs;
   std::vector<std::string> aLinks;

   for( int i = 1; i < nArgc; ++i )
   {
      const char *szArg = apszArgv[ i ];
      if( szArg[ 0 ] == '-' )
      {
         if( strcmp( szArg, "--dump" ) == 0 )
         {
            lDumpAst = true;
            continue;
         }
         if( strcmp( szArg, "--ast" ) == 0 )
         {
            lAstStdout = true;
            continue;
         }
         if( strcmp( szArg, "--ir" ) == 0 )
         {
            lIrStdout = true;
            continue;
         }
         if( strcmp( szArg, "--ir-out" ) == 0 )
         {
            if( i + 1 >= nArgc )
            {
               fprintf( stderr, "error: --ir-out requires a path\n" );
               return 2;
            }
            szIrOut = apszArgv[ ++i ];
            continue;
         }
         if( strcmp( szArg, "--obj" ) == 0 )
         {
            if( i + 1 >= nArgc )
            {
               fprintf( stderr, "error: --obj requires a path\n" );
               return 2;
            }
            szObjOut = apszArgv[ ++i ];
            continue;
         }
         if( strcmp( szArg, "--bin" ) == 0 )
         {
            if( i + 1 >= nArgc )
            {
               fprintf( stderr, "error: --bin requires a path\n" );
               return 2;
            }
            szBinOut = apszArgv[ ++i ];
            continue;
         }
         if( strcmp( szArg, "--link" ) == 0 )
         {
            if( i + 1 >= nArgc )
            {
               fprintf( stderr, "error: --link requires a path\n" );
               return 2;
            }
            aLinks.push_back( apszArgv[ ++i ] );
            continue;
         }
         if( strcmp( szArg, "-L" ) == 0 )
         {
            if( i + 1 >= nArgc )
            {
               fprintf( stderr, "error: -L requires a path\n" );
               return 2;
            }
            aLibDirs.push_back( apszArgv[ ++i ] );
            continue;
         }
         if( strncmp( szArg, "-L", 2 ) == 0 )
         {
            if( szArg[ 2 ] == '\0' )
            {
               fprintf( stderr, "error: -L requires a path\n" );
               return 2;
            }
            aLibDirs.push_back( szArg + 2 );
            continue;
         }
         if( strcmp( szArg, "-l" ) == 0 )
         {
            if( i + 1 >= nArgc )
            {
               fprintf( stderr, "error: -l requires a library name\n" );
               return 2;
            }
            aLibs.push_back( apszArgv[ ++i ] );
            continue;
         }
         if( strncmp( szArg, "-l", 2 ) == 0 )
         {
            if( szArg[ 2 ] == '\0' )
            {
               fprintf( stderr, "error: -l requires a library name\n" );
               return 2;
            }
            aLibs.push_back( szArg + 2 );
            continue;
         }
         if( strcmp( szArg, "-o" ) == 0 )
         {
            if( i + 1 >= nArgc )
            {
               fprintf( stderr, "error: -o requires a path\n" );
               return 2;
            }
            szOutput = apszArgv[ ++i ];
            continue;
         }
         if( strcmp( szArg, "-h" ) == 0 || strcmp( szArg, "--help" ) == 0 )
         {
            fprintf( stderr, "Usage:\n"
                             "  dory <file.dor> [options]\n"
                             "\n"
                             "Input/AST:\n"
                             "  -o <path>       Write AST to file\n"
                             "  --ast           Print AST to stdout\n"
                             "  --dump          Dump AST (debug)\n"
                             "\n"
                             "IR/Object/Binary:\n"
                             "  --ir            Print LLVM IR to stdout\n"
                             "  --ir-out <path> Write LLVM IR to file\n"
                             "  --obj <path>    Write object file\n"
                             "  --bin <path>    Link and write executable\n"
                             "\n"
                             "Linker options:\n"
                             "  --link <path>   Add object or library file (can repeat)\n"
                             "  -L <dir>        Add library search path (can repeat)\n"
                             "  -l <name>       Link library by name (can repeat)\n"
                             "\n"
                             "Examples:\n"
                             "  dory main.dor --dump\n"
                             "  dory main.dor --ir-out out.ll\n"
                             "  dory main.dor --bin out\n" );
            return 0;
         }
         fprintf( stderr, "error: unknown option '%s'\n", szArg );
         return 2;
      }

      if( szInput )
      {
         fprintf( stderr, "error: multiple input files\n" );
         return 2;
      }
      szInput = szArg;
   }

   if( !szInput )
   {
      fprintf( stderr, "usage: dory <file.dor> [options]\n"
                       "try: dory --help\n" );
      return 2;
   }

   FILE *pIn = fopen( szInput, "rb" );
   if( !pIn )
   {
      perror( szInput );
      return 2;
   }

   std::string oSrc;
   if( !read_file_into_string( pIn, oSrc ) )
   {
      fclose( pIn );
      fprintf( stderr, "error: failed to read input\n" );
      return 2;
   }

   fclose( pIn );

   if( oSrc.empty() || ( oSrc.back() != '\n' && oSrc.back() != '\r' ) )
      oSrc.push_back( '\n' );

   Program pAst = psProgram( oSrc.c_str() );

   if( !pAst )
      return 1;

   if( szOutput )
   {
      FILE *pOut = fopen( szOutput, "wb" );
      if( !pOut )
      {
         perror( szOutput );
         return 2;
      }
      char *szAst = printProgram( pAst );
      if( szAst )
      {
         fputs( szAst, pOut );
         fputc( '\n', pOut );
         free( szAst );
      }
      fclose( pOut );
   }

   if( lAstStdout )
   {
      char *szAst = printProgram( pAst );
      if( szAst )
      {
         fputs( szAst, stdout );
         fputc( '\n', stdout );
         free( szAst );
      }
   }

   if( lDumpAst )
      ast_DumpProgram( pAst );

   if( lIrStdout || szIrOut || szObjOut || szBinOut )
   {
      if( !sema_ValidateAst( pAst ) )
         return 1;

      CodeGenerator oGen( "dory" );
      if( !oGen.generate( pAst ) )
      {
         fprintf( stderr, "error: codegen failed\n" );
         return 1;
      }
      if( lIrStdout )
      {
         std::string oIr = oGen.getIRString();
         fputs( oIr.c_str(), stdout );
      }
      if( szIrOut )
      {
         if( !oGen.writeIRToFile( szIrOut ) )
         {
            fprintf( stderr, "error: failed to write IR to %s\n", szIrOut );
            return 2;
         }
      }

      std::string oObjPath;
      bool        lAutoObj = ( szBinOut && !szObjOut );
      std::string oBinPath;
      std::string oBinBase;
      if( szBinOut )
      {
         oBinPath = szBinOut;
         oBinBase = strip_extension( oBinPath );
#if DO_OS_WINDOWS
         if( !has_extension( oBinPath ) )
            oBinPath += ".exe";
#endif
      }
      if( szObjOut )
         oObjPath = szObjOut;
      else if( !oBinBase.empty() )
         oObjPath = oBinBase + ".o";

      if( !oObjPath.empty() )
      {
         if( !oGen.writeObjectToFile( oObjPath ) )
         {
            fprintf( stderr, "error: failed to write object to %s\n", oObjPath.c_str() );
            return 2;
         }
      }

      if( szBinOut )
      {
         const char *szDefaultLibDir = "../lib";
         const char *szDefaultLib    = "dory";
         bool        lHaveLibDir     = false;
         bool        lHaveLib        = false;

         for( const std::string &oDir : aLibDirs )
         {
            if( oDir == szDefaultLibDir )
            {
               lHaveLibDir = true;
               break;
            }
         }
         if( !lHaveLibDir )
            aLibDirs.push_back( szDefaultLibDir );

         for( const std::string &oLib : aLibs )
         {
            if( oLib == szDefaultLib )
            {
               lHaveLib = true;
               break;
            }
         }
         if( !lHaveLib )
            aLibs.push_back( szDefaultLib );

         std::string oCmd = "clang \"" + oObjPath + "\"";
         for( const std::string &oLink : aLinks )
            oCmd += " \"" + oLink + "\"";
         for( const std::string &oDir : aLibDirs )
            oCmd += " -L\"" + oDir + "\"";
         for( const std::string &oLib : aLibs )
            oCmd += " -l" + oLib;
         oCmd += " -o \"" + oBinPath + "\"";
         int nRc = system( oCmd.c_str() );
         if( nRc != 0 )
         {
            fprintf( stderr, "error: clang link failed\n" );
            return 2;
         }
         if( lAutoObj && !oObjPath.empty() )
            std::remove( oObjPath.c_str() );
      }
   }

   return 0;
}

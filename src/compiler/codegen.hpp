/*
 * codegen.hpp - Minimal codegen skeleton
 */

#ifndef CODEGEN_HPP_
#define CODEGEN_HPP_

#include "Absyn.h"

#include <string>

class CodeGenerator
{
 public:
   explicit CodeGenerator( const std::string &oModuleName );
   ~CodeGenerator();
   bool        generate( Program pRoot );
   bool        writeIRToFile( const std::string &oPath ) const;
   bool        writeObjectToFile( const std::string &oPath ) const;
   std::string getIRString() const;

 private:
   struct Impl;
   Impl *m_Impl;
};

#endif /* The End CODEGEN_HPP_ */

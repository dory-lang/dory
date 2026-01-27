/*
 * types.hpp - Minimal types
 */

#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <stdbool.h>

typedef enum
{
   TYPE_VARIANT
} DataType;

typedef struct _TypeInfo TypeInfo;

struct _TypeInfo
{
   DataType nDataType;
};

TypeInfo *type_New( const char *szName );
void      type_Free( TypeInfo *pType );

#endif /* TYPES_HPP_ */

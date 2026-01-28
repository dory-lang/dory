/*
 * oop.h
 */

#ifndef DO_OOP_H_
#define DO_OOP_H_

#include "doapi.h"

typedef struct _DO_ENUM DO_ENUM;

struct _DO_ENUM
{
   DO_ARRAY *pArray;
   int64_t   nIndex;
   int64_t   nLen;
   int       bDescend;
};

DO_ENUM *do_enum_new_array( DO_ARRAY *pArray, int bDescend );
void     do_enum_free( DO_ENUM *pEnum );

int      do_enum_step( DO_ENUM *pEnum );
void     do_enum_set_index( DO_ENUM *pEnum, int64_t nIndex );
int64_t  do_enum_index( const DO_ENUM *pEnum );
int      do_enum_isfirst( const DO_ENUM *pEnum );
int      do_enum_islast( const DO_ENUM *pEnum );
DO_ITEM *do_enum_value_ptr( DO_ENUM *pEnum );
DO_ARRAY *do_enum_base( DO_ENUM *pEnum );

#endif

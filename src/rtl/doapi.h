/*
 * doapi.h - API header for the Dory virtual machine
 */

#ifndef DOAPI_H_
#define DOAPI_H_

#include "platform.h"
#include <stddef.h>
#include <stdint.h>

typedef struct _DO_VM DO_VM;

/* Macro for defining Dory functions */
#define DO_FUNC( name ) void DO_##name( DO_VM *pVm )

/* --- KIND + FLAGS --- */
typedef uint32_t DO_TYPE;

#define DO_KIND_MASK 0x000000FFu /* 0..255 base kinds */

/* KIND: exactly one of the base kinds */
typedef enum
{
   DO_K_NIL = 0,
   DO_K_POINTER,
   DO_K_I32,
   DO_K_I64,
   DO_K_F64,
   DO_K_LOGICAL,
   DO_K_DATE,
   DO_K_TIMESTAMP,
   DO_K_SYMBOL,
   DO_K_STRING,
   DO_K_BLOCK,
   DO_K_ARRAY,
   DO_K_HASH,
   DO_K_ENUM,
   DO_K_STRUCT,
   DO_K_OBJECT
} DO_KIND;

/* FLAGS: qualifiers */
enum
{
   DO_F_BYREF    = 0x00000100u,
   DO_F_MEMVAR   = 0x00000200u,
   DO_F_ALIAS    = 0x00000400u,
   DO_F_MEMOFLAG = 0x00000800u,
   DO_F_EXTREF   = 0x00001000u,
   DO_F_DEFAULT  = 0x00002000u,
   DO_F_RECOVER  = 0x00004000u,
   /* Additional flags in the future: CONST, READONLY, GC, TEMP, etc. */
};

/* Helper macros */
#define DO_T( kind )            ( ( DO_TYPE ) ( kind ) )
#define DO_KIND_OF( nIt )       ( ( DO_KIND ) ( ( nIt ) & DO_KIND_MASK ) )
#define DO_KIND( nIt )          DO_KIND_OF( nIt )
#define DO_HASFLAG( nIt, flag ) ( ( ( nIt ) & ( flag ) ) != 0 )
#define DO_ADDFLAG( nIt, flag ) ( ( nIt ) | ( flag ) )

/* Macros for common combinations of types with flags */
#define DO_MAKE_BYREF( nIt )   DO_ADDFLAG( ( nIt ), DO_F_BYREF )
#define DO_MAKE_MEMVAR( nIt )  DO_ADDFLAG( ( nIt ), DO_F_MEMVAR )
#define DO_MAKE_MEMO( nIt )    DO_ADDFLAG( ( nIt ), DO_F_MEMOFLAG )
#define DO_MAKE_DEFAULT( nIt ) DO_ADDFLAG( ( nIt ), DO_F_DEFAULT )

/* Legacy name mapping for backward compatibility */
enum
{
   DO_IT_NIL       = DO_T( DO_K_NIL ),
   DO_IT_POINTER   = DO_T( DO_K_POINTER ),
   DO_IT_INTEGER   = DO_T( DO_K_I32 ),
   DO_IT_LONG      = DO_T( DO_K_I64 ),
   DO_IT_DOUBLE    = DO_T( DO_K_F64 ),
   DO_IT_DATE      = DO_T( DO_K_DATE ),
   DO_IT_TIMESTAMP = DO_T( DO_K_TIMESTAMP ),
   DO_IT_LOGICAL   = DO_T( DO_K_LOGICAL ),
   DO_IT_SYMBOL    = DO_T( DO_K_SYMBOL ),
   DO_IT_STRING    = DO_T( DO_K_STRING ),
   DO_IT_MEMOFLAG  = DO_F_MEMOFLAG,
   DO_IT_BLOCK     = DO_T( DO_K_BLOCK ),
   DO_IT_ARRAY     = DO_T( DO_K_ARRAY ),
   DO_IT_HASH      = DO_T( DO_K_HASH ),
   DO_IT_ENUM      = DO_T( DO_K_ENUM ),
   DO_IT_STRUCT    = DO_T( DO_K_STRUCT ),
   DO_IT_OBJECT    = DO_T( DO_K_OBJECT ),
   DO_IT_BYREF     = DO_F_BYREF,
   DO_IT_MEMVAR    = DO_F_MEMVAR,
   DO_IT_ALIAS     = DO_F_ALIAS,
   DO_IT_EXTREF    = DO_F_EXTREF,
   DO_IT_DEFAULT   = DO_F_DEFAULT,
   DO_IT_RECOVER   = DO_F_RECOVER,
   DO_IT_MEMO      = ( DO_F_MEMOFLAG | DO_IT_STRING ),
   DO_IT_ANY       = 0xFFFFFFFFu
};

/* Type groups - as static inline functions */
static inline int do_is_numint( DO_TYPE nIt )
{
   DO_KIND k = DO_KIND_OF( nIt );
   return ( k == DO_K_I32 ) || ( k == DO_K_I64 );
}

static inline int do_is_numeric( DO_TYPE nIt )
{
   DO_KIND k = DO_KIND_OF( nIt );
   return ( k == DO_K_I32 ) || ( k == DO_K_I64 ) || ( k == DO_K_F64 );
}

static inline int do_is_datetime( DO_TYPE nIt )
{
   DO_KIND k = DO_KIND_OF( nIt );
   return ( k == DO_K_DATE ) || ( k == DO_K_TIMESTAMP );
}

static inline int do_is_complex( DO_TYPE nIt )
{
   DO_KIND k = DO_KIND_OF( nIt );
   return ( k == DO_K_BLOCK ) || ( k == DO_K_ARRAY ) || ( k == DO_K_HASH ) || ( k == DO_K_POINTER ) ||
          ( k == DO_K_STRING );
}

static inline int do_is_scalar( DO_TYPE nIt )
{
   DO_KIND k = DO_KIND_OF( nIt );
   return ( k == DO_K_NIL ) || ( k == DO_K_I32 ) || ( k == DO_K_I64 ) || ( k == DO_K_F64 ) || ( k == DO_K_LOGICAL ) ||
          ( k == DO_K_DATE ) || ( k == DO_K_TIMESTAMP ) || ( k == DO_K_STRING ) || ( k == DO_K_SYMBOL );
}

static inline int do_is_integer( DO_TYPE nIt )
{
   return DO_KIND_OF( nIt ) == DO_K_I32;
}

static inline int do_is_long( DO_TYPE nIt )
{
   return DO_KIND_OF( nIt ) == DO_K_I64;
}

static inline int do_is_double( DO_TYPE nIt )
{
   return DO_KIND_OF( nIt ) == DO_K_F64;
}

static inline int do_is_string( DO_TYPE nIt )
{
   return DO_KIND_OF( nIt ) == DO_K_STRING;
}

static inline int do_is_logical( DO_TYPE nIt )
{
   return DO_KIND_OF( nIt ) == DO_K_LOGICAL;
}

static inline int do_is_array( DO_TYPE nIt )
{
   return DO_KIND_OF( nIt ) == DO_K_ARRAY;
}

static inline int do_is_hash( DO_TYPE nIt )
{
   return DO_KIND_OF( nIt ) == DO_K_HASH;
}

static inline int do_is_struct( DO_TYPE nIt )
{
   return DO_KIND_OF( nIt ) == DO_K_STRUCT;
}

static inline int do_is_object( DO_TYPE nIt )
{
   return DO_KIND_OF( nIt ) == DO_K_OBJECT;
}

static inline int do_is_pointer( DO_TYPE nIt )
{
   return DO_KIND_OF( nIt ) == DO_K_POINTER;
}

static inline int do_is_block( DO_TYPE nIt )
{
   return DO_KIND_OF( nIt ) == DO_K_BLOCK;
}

static inline int do_is_symbol( DO_TYPE nIt )
{
   return DO_KIND_OF( nIt ) == DO_K_SYMBOL;
}

static inline int do_is_enum( DO_TYPE nIt )
{
   return DO_KIND_OF( nIt ) == DO_K_ENUM;
}

static inline int do_is_nil( DO_TYPE nIt )
{
   return DO_KIND_OF( nIt ) == DO_K_NIL;
}

/* Flag checks */
static inline int do_is_byref( DO_TYPE nIt )
{
   return DO_HASFLAG( nIt, DO_F_BYREF );
}

static inline int do_is_memvar( DO_TYPE nIt )
{
   return DO_HASFLAG( nIt, DO_F_MEMVAR );
}

static inline int do_is_alias( DO_TYPE nIt )
{
   return DO_HASFLAG( nIt, DO_F_ALIAS );
}

static inline int do_is_memo( DO_TYPE nIt )
{
   return ( DO_KIND_OF( nIt ) == DO_K_STRING ) && DO_HASFLAG( nIt, DO_F_MEMOFLAG );
}

static inline int do_is_extref( DO_TYPE nIt )
{
   return DO_HASFLAG( nIt, DO_F_EXTREF );
}

static inline int do_is_default( DO_TYPE nIt )
{
   return DO_HASFLAG( nIt, DO_F_DEFAULT );
}

static inline int do_is_recover( DO_TYPE nIt )
{
   return DO_HASFLAG( nIt, DO_F_RECOVER );
}

/* Helpers to create types with flags */
static inline DO_TYPE do_make_type( DO_KIND kind )
{
   return DO_T( kind );
}

static inline DO_TYPE do_make_byref( DO_TYPE nIt )
{
   return DO_ADDFLAG( nIt, DO_F_BYREF );
}

static inline DO_TYPE do_make_memvar( DO_TYPE nIt )
{
   return DO_ADDFLAG( nIt, DO_F_MEMVAR );
}

static inline DO_TYPE do_make_memo( DO_TYPE nIt )
{
   if( DO_KIND_OF( nIt ) == DO_K_STRING )
   {
      return DO_ADDFLAG( nIt, DO_F_MEMOFLAG );
   }
   return nIt;
}

typedef struct _DO_ITEM        DO_ITEM;
typedef struct _DO_STRUCT_TYPE DO_STRUCT_TYPE;

struct _DO_STRUCT_TYPE
{
   uint32_t    nFieldCount;
   const char *szName;
   /* You can add field descriptions, offsets, types, etc. here. */
};

struct _DO_ITEM
{
   DO_TYPE         nType;
   uint32_t        nReserved;   /* Can be used as typeId for STRUCT/OBJECT */
   DO_STRUCT_TYPE *pStructType; /* Type description for STRUCT/OBJECT */
   union
   {
      int64_t     nValue;
      double      dValue;
      const char *szValue;
      void       *pValue;
   } u;
};

struct _DO_VM
{
   int32_t  nArgc;
   int32_t  nPad;
   DO_ITEM *apArgv;
   DO_ITEM  oRet;
};

/* do_x* stubs */
void *do_xalloc( size_t nSize );
void  do_xfree( void *pMem );
void *do_xgrab( size_t nSize );
void  do_xunlock( void *pMem );

/* do_param style (nIt: kind + flags; DO_IT_ANY = no check) */
const DO_ITEM *do_param( DO_VM *pVm, int nIdx1Based, DO_TYPE nIt );
const char    *do_parc( DO_VM *pVm, int nIdx1Based );
size_t         do_parclen( DO_VM *pVm, int nIdx1Based );
size_t         do_parcsiz( DO_VM *pVm, int nIdx1Based );
DO_TYPE        do_parinfo( DO_VM *pVm, int nIdx1Based );
void          *do_parptr( DO_VM *pVm, int nIdx1Based, int *pOkay );
int            do_parl( DO_VM *pVm, int nIdx1Based, int *pOkay );
double         do_parnd( DO_VM *pVm, int nIdx1Based, int *pOkay );
int64_t        do_parni( DO_VM *pVm, int nIdx1Based, int *pOkay );
int64_t        do_parnl( DO_VM *pVm, int nIdx1Based, int *pOkay );

/* do_ret* style */
void do_retnil( DO_VM *pVm );
void do_retl( DO_VM *pVm, int bValue );
void do_retni( DO_VM *pVm, int64_t nValue );
void do_retc( DO_VM *pVm, const char *szValue );
void do_retptr( DO_VM *pVm, void *pValue );

void do_err_args( const char *szFuncName );
void do_err_type( const char *szFuncName, const char *szExpected, DO_TYPE nIt );

#endif /* The End DOAPI_H_ */

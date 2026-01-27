/*
 * platform.h - Platform detection for GT library
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

/* ----------------------------------------------------------------------------
   Operating System Detection
   ------------------------------------------------------------------------- */

/* UNIX/Linux family */
#if defined( __unix__ ) || defined( __unix ) || defined( unix )
#   define DO_OS_UNIX 1
#else
#   define DO_OS_UNIX 0
#endif

/* Linux */
#if defined( __linux__ ) || defined( __linux )
#   define DO_OS_LINUX 1
#else
#   define DO_OS_LINUX 0
#endif

/* Apple macOS (Darwin) */
#if defined( __APPLE__ ) && defined( __MACH__ )
#   define DO_OS_DARWIN 1
#   define DO_OS_MACOS  1
#else
#   define DO_OS_DARWIN 0
#   define DO_OS_MACOS  0
#endif

/* BSD family */
#if defined( __FreeBSD__ ) || defined( __OpenBSD__ ) || defined( __NetBSD__ ) || defined( __DragonFly__ )
#   define DO_OS_BSD 1
#else
#   define DO_OS_BSD 0
#endif

/* Windows */
#if defined( _WIN32 ) || defined( _WIN64 ) || defined( __WIN32__ ) || defined( __TOS_WIN__ ) || defined( __WINDOWS__ )
#   define DO_OS_WINDOWS 1
#   ifndef WIN32_LEAN_AND_MEAN
#      define WIN32_LEAN_AND_MEAN
#   endif
#else
#   define DO_OS_WINDOWS 0
#endif

/* Windows z Virtual Terminal support (Windows 10 1809+) */
#if DO_OS_WINDOWS
#   if defined( _WIN32_WINNT ) && _WIN32_WINNT >= 0x0A00
#      define DO_WINDOWS_HAS_VT 1
#   else
#      define DO_WINDOWS_HAS_VT 0
#   endif
#else
#   define DO_WINDOWS_HAS_VT 0
#endif

/* Cygwin (Windows with POSIX layer) */
#if defined( __CYGWIN__ )
#   define DO_OS_CYGWIN 1
#else
#   define DO_OS_CYGWIN 0
#endif

/* Haiku */
#ifdef __HAIKU__
#   define DO_OS_HAIKU 1
#else
#   define DO_OS_HAIKU 0
#endif

/* Solaris/Illumos */
#ifdef __sun
#   define DO_OS_SOLARIS 1
#else
#   define DO_OS_SOLARIS 0
#endif

/* IBM AIX */
#ifdef _AIX
#   define DO_OS_AIX 1
#else
#   define DO_OS_AIX 0
#endif

/* ----------------------------------------------------------------------------
   Composite definitions (convenience)
   ------------------------------------------------------------------------- */

/* Any POSIX/UNIX-like system */
#if DO_OS_UNIX || DO_OS_LINUX || DO_OS_DARWIN || DO_OS_BSD || DO_OS_CYGWIN || DO_OS_HAIKU || DO_OS_SOLARIS || DO_OS_AIX
#   define DO_OS_POSIX 1
#else
#   define DO_OS_POSIX 0
#endif

/* Any BSD system (including Darwin) */
#if DO_OS_BSD || DO_OS_DARWIN
#   define DO_OS_BSD_FAMILY 1
#else
#   define DO_OS_BSD_FAMILY 0
#endif

/* X11 availability (for the xwc backend) */
#if DO_OS_POSIX && !DO_OS_DARWIN /* macOS has XQuartz, but not by default */
#   define DO_HAS_X11_POSSIBLE 1
#else
#   define DO_HAS_X11_POSSIBLE 0
#endif

#ifndef DO_HAS_X11
#   define DO_HAS_X11 0 /* Disabled by default; set by the build */
#endif

/* ----------------------------------------------------------------------------
   Architecture Detection (optional but useful)
   ------------------------------------------------------------------------- */

/* 64-bit detection */
#if defined( __x86_64__ ) || defined( _M_X64 ) || defined( __aarch64__ ) || defined( __64BIT__ ) || defined( __LP64__ )
#   define DO_ARCH_64BIT 1
#else
#   define DO_ARCH_64BIT 0
#endif

/* Endianness */
#if ( defined( __BYTE_ORDER__ ) && ( __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ ) ) || defined( __LITTLE_ENDIAN__ ) ||  \
    defined( _WIN32 ) || defined( __i386__ ) || defined( __x86_64__ )
#   define DO_LITTLE_ENDIAN 1
#   define DO_BIG_ENDIAN    0
#elif ( defined( __BYTE_ORDER__ ) && ( __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ ) ) || defined( __BIG_ENDIAN__ ) ||      \
    defined( __sparc__ )
#   define DO_LITTLE_ENDIAN 0
#   define DO_BIG_ENDIAN    1
#else
#   define DO_LITTLE_ENDIAN 0
#   define DO_BIG_ENDIAN    0
#endif

/* ----------------------------------------------------------------------------
   Compiler Detection
   ------------------------------------------------------------------------- */

/* GCC or compatible */
#if defined( __GNUC__ ) && !defined( __clang__ )
#   define DO_COMPILER_GCC     1
#   define DO_COMPILER_VERSION ( __GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__ )
#elif defined( __clang__ )
#   define DO_COMPILER_CLANG   1
#   define DO_COMPILER_VERSION ( __clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__ )
#elif defined( _MSC_VER )
#   define DO_COMPILER_MSVC    1
#   define DO_COMPILER_VERSION _MSC_VER
#else
#   define DO_COMPILER_UNKNOWN 1
#   define DO_COMPILER_VERSION 0
#endif

/* MinGW (GCC on Windows) */
#if defined( __MINGW32__ ) || defined( __MINGW64__ )
#   define DO_COMPILER_MINGW 1
#else
#   define DO_COMPILER_MINGW 0
#endif

/* ----------------------------------------------------------------------------
   Debug/Release detection
   ------------------------------------------------------------------------- */

#if !defined( NDEBUG ) || defined( _DEBUG ) || defined( DEBUG )
#   define DO_DEBUG 1
#else
#   define DO_DEBUG 0
#endif

/* ----------------------------------------------------------------------------
   Language Detection (C or C++)
   ------------------------------------------------------------------------- */

#ifdef __cplusplus
#   define DO_LANG_CPP 1
#   define DO_EXTERN_C_BEGIN                                                                                           \
      extern "C"                                                                                                       \
      {
#   define DO_EXTERN_C_END }
#   define DO_CPP          1
#else
#   define DO_LANG_CPP 0
#   define DO_EXTERN_C_BEGIN
#   define DO_EXTERN_C_END
#   define DO_CPP 0
#endif

/* ----------------------------------------------------------------------------
   Convenience macros for common checks
   ------------------------------------------------------------------------- */

#define DO_ON_WINDOWS() ( DO_OS_WINDOWS )
#define DO_ON_UNIX()    ( DO_OS_POSIX )
#define DO_ON_LINUX()   ( DO_OS_LINUX )
#define DO_ON_MACOS()   ( DO_OS_MACOS )
#define DO_ON_BSD()     ( DO_OS_BSD_FAMILY )

#define DO_IS_DEBUG() ( DO_DEBUG )
#define DO_IS_64BIT() ( DO_ARCH_64BIT )

/* ----------------------------------------------------------------------------
   Deprecation warnings
   ------------------------------------------------------------------------- */

#if DO_COMPILER_GCC || DO_COMPILER_CLANG
#   define DO_DEPRECATED( msg ) __attribute__( ( deprecated( msg ) ) )
#elif defined( _MSC_VER )
#   define DO_DEPRECATED( msg ) __declspec( deprecated( msg ) )
#else
#   define DO_DEPRECATED( msg )
#endif

/* ----------------------------------------------------------------------------
   Inline hints
   ------------------------------------------------------------------------- */

#if DO_COMPILER_GCC || DO_COMPILER_CLANG
#   define DO_INLINE   inline __attribute__( ( always_inline ) )
#   define DO_NOINLINE __attribute__( ( noinline ) )
#elif defined( _MSC_VER )
#   define DO_INLINE   __forceinline
#   define DO_NOINLINE __declspec( noinline )
#else
#   define DO_INLINE inline
#   define DO_NOINLINE
#endif

/* ----------------------------------------------------------------------------
   Inline assembly macros
   ------------------------------------------------------------------------- */

#if DO_COMPILER_GCC || DO_COMPILER_CLANG
#   define DO_ASM( ... ) __asm__ __volatile__( __VA_ARGS__ )
#elif defined( _MSC_VER )
#   define DO_ASM( ... ) __asm { __VA_ARGS__ }
#else
#   define DO_ASM( ... )
#endif

/* ----------------------------------------------------------------------------
   API export/import for shared libraries (DLL)
   ------------------------------------------------------------------------- */

#if DO_ON_WINDOWS()
/* Export from DLL / import when using */
#   ifdef DO_BUILDING_DLL
#      define DO_API __declspec( dllexport )
#   elif defined( DO_USING_DLL )
#      define DO_API __declspec( dllimport )
#   else
#      define DO_API
#   endif
#else
/* On UNIX-like systems use visibility where available */
#   if DO_COMPILER_GCC || DO_COMPILER_CLANG
#      define DO_API __attribute__( ( visibility( "default" ) ) )
#   else
#      define DO_API
#   endif
#endif

/* ----------------------------------------------------------------------------
   Likely/unlikely hints (for optimization)
   ------------------------------------------------------------------------- */

#if DO_COMPILER_GCC || DO_COMPILER_CLANG
#   define DO_LIKELY( x )   __builtin_expect( !!( x ), 1 )
#   define DO_UNLIKELY( x ) __builtin_expect( !!( x ), 0 )
#else
#   define DO_LIKELY( x )   ( x )
#   define DO_UNLIKELY( x ) ( x )
#endif

/* ----------------------------------------------------------------------------
   Function format checking (for printf-like functions)
   ------------------------------------------------------------------------- */

#if DO_COMPILER_GCC || DO_COMPILER_CLANG
#   define DO_PRINTF_ATTR( fmt_index, args_index ) __attribute__( ( format( printf, fmt_index, args_index ) ) )
#   define DO_SCANF_ATTR( fmt_index, args_index )  __attribute__( ( format( scanf, fmt_index, args_index ) ) )
#else
#   define DO_PRINTF_ATTR( fmt_index, args_index )
#   define DO_SCANF_ATTR( fmt_index, args_index )
#endif

/* ----------------------------------------------------------------------------
   Fallthrough for switch statements
   ------------------------------------------------------------------------- */

#if defined( __GNUC__ ) && !defined( __clang__ ) && ( DO_COMPILER_VERSION >= 70000 )
#   define DO_FALLTHROUGH __attribute__( ( fallthrough ) )
#elif defined( __clang__ )
#   define DO_FALLTHROUGH __attribute__( ( fallthrough ) )
#elif defined( _MSC_VER ) && ( DO_COMPILER_VERSION >= 1910 )
#   define DO_FALLTHROUGH [[fallthrough]]
#else
#   define DO_FALLTHROUGH ( ( void ) 0 )
#endif

/* ----------------------------------------------------------------------------
   Unused parameter/variable warning suppression
   ------------------------------------------------------------------------- */

#if DO_COMPILER_GCC || DO_COMPILER_CLANG
#   define DO_UNUSED_FUNC __attribute__( ( unused ) )
#else
/* MSVC: use (void)x locally, pragma macro here would be misleading */
#   define DO_UNUSED_FUNC
#endif

#endif /* PLATFORM_H_ */

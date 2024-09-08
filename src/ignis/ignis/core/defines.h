#ifndef IGNIS_CORE_DEFINES_H
#define IGNIS_CORE_DEFINES_H

#include <stddef.h>
#include <stdint.h>

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef size_t  usize;
typedef _Bool   b8;
typedef int32_t b32;
typedef float   f32;
typedef double  f64;
typedef char    byte;

#define true  1
#define false 0

#if defined(__clang__) || defined(__GNUC__)
#  define GENERATE_TRAP() __builtin_trap()
#  define THREADLOCAL     __thread
#  define STDCALL         __attribute__((stdcall))
#  define CDECL           __attribute__((cdecl))
#  define RESTRICT        __restrict__
#  define FORCE_INLINE    __attribute__((always_inline)) inline
#  define FORCE_NOINLINE  __attribute__((noinline))
#  define NO_RETURN       __attribute__((noreturn))
#  define DEPRECATED      [[deprecated]]
#  define STATIC_ASSERT   _Static_assert

#elif defined(_MSC_VER)
#  if _MSC_VER < 1900
#    error "Required Visual Studio 2015 or newer."
#  endif
#  define GENERATE_TRAP() __debugbreak()
#  define THREADLOCAL     __declspec(thread)
#  define STDCALL         __stdcall
#  define CDECL           __cdecl
#  define RESTRICT        __restrict
#  define FORCE_INLINE    __forceinline
#  define FORCE_NOINLINE  __declspec(noinline)
#  define NO_RETURN       __declspec(noreturn)
#  define DEPRECATED      __declspec(deprecated)
#  define STATIC_ASSERT   static_assert

#  undef __PRETTY_FUNCTION__
#  define __PRETTY_FUNCTION__ __FUNCS_
#else
#  error "Unknown compiler"
#endif

#define _INTL_COMBINE_MACRO_VAR(a, b) a##b
#define _INTL_MACRO_VAR(a, b)         _INTL_COMBINE_MACRO_VAR(a, b)

#define _INTL_128TH_ARG(                                                       \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16,     \
    _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, \
    _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, \
    _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, \
    _62, _63, _64, _65, _66, _67, _68, _69, _70, _71, _72, _73, _74, _75, _76, \
    _77, _78, _79, _80, _81, _82, _83, _84, _85, _86, _87, _88, _89, _90, _91, \
    _92, _93, _94, _95, _96, _97, _98, _99, _100, _101, _102, _103, _104,      \
    _105, _106, _107, _108, _109, _110, _111, _112, _113, _114, _115, _116,    \
    _117, _118, _119, _120, _121, _122, _123, _124, _125, _126, _127, N, ...)  \
  N
#define _INTL_RSEQ_N()                                                         \
  127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113,   \
      112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, \
      97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80,  \
      79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62,  \
      61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44,  \
      43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26,  \
      25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, \
      6, 5, 4, 3, 2, 1, 0
#define _INTL_NARG_(...) _INTL_128TH_ARG(__VA_ARGS__)

#define NARG(...)        _INTL_NARG_(__VA_ARGS__, _INTL_RSEQ_N())
#define MACRO_VAR(_name) _INTL_MACRO_VAR(_name, __LINE__)

#define defer(...)                                                             \
  for (i32 MACRO_VAR(i) = 0; MACRO_VAR(i) != 1; MACRO_VAR(i)++, __VA_ARGS__)

#define IMIN(x, y) (x < y ? x : y)
#define IMAX(x, y) (x > y ? x : y)

#define MEM_ALLOC(_byte_size)         malloc(_byte_size)
#define MEM_REALLOC(_ptr, _byte_size) realloc(_ptr, _byte_size)
#define MEM_DESTROY(_ptr)             free(_ptr)

#endif

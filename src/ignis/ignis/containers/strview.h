#ifndef IGNIS_CORE_STRVIEW_H
#define IGNIS_CORE_STRVIEW_H

#include "ignis/containers/dyarr.h"
#include "ignis/containers/strbuf.h"
#include "ignis/core/defines.h"

typedef struct strview {
  u64         len;
  const char* cstr;
} strview_t;

#define EMPTY_STRVIEW                                                          \
  (strview_t) {                                                                \
    .len = 0, .cstr = NULL,                                                    \
  }

strview_t _str_slice_strbuf(const strbuf_t* str, u64 beg, u64 end);
strview_t _str_slice_strview(const strview_t* view, u64 beg, u64 end);
strview_t _str_slice_cstr(const char* cstr, u64 beg, u64 end);

// if end is == NO_POS then the length of the string will go to the end of the
// original string from the beg pos
#define str_slice(str, beg, end)                                               \
  _Generic((str),                                                              \
      strbuf_t *: _str_slice_strbuf,                                           \
      strview_t *: _str_slice_strview,                                         \
      char*: _str_slice_cstr,                                                  \
      const strbuf_t*: _str_slice_strbuf,                                      \
      const strview_t*: _str_slice_strview,                                    \
      const char*: _str_slice_cstr)(str, beg, end)

strview_t _strview_ref(const strbuf_t* str);
strview_t _strview_ref_view(const strview_t* view);
strview_t _strview_ref_cstr(const char* cstr);

#define strview(str)                                                           \
  _Generic((str),                                                              \
      strbuf_t *: _strview_ref,                                                \
      strview_t *: _strview_ref_view,                                          \
      char*: _strview_ref_cstr,                                                \
      const strbuf_t*: _strview_ref,                                           \
      const strview_t*: _strview_ref_view,                                     \
      const char*: _strview_ref_cstr)(str)

b8 _strview_cmp(const strview_t* str1, const strbuf_t* str2);
b8 _strview_cmp_view(const strview_t* view1, const strview_t* view2);
b8 _strview_cmp_cstr(const strview_t* view, const char* cstr);

#define strview_cmp(str1, str2)                                                \
  _Generic((str2),                                                             \
      strbuf_t *: _strview_cmp,                                                \
      strview_t *: _strview_cmp_view,                                          \
      char*: _strview_cmp_cstr,                                                \
      const strbuf_t*: _strview_cmp,                                           \
      const strview_t*: _strview_cmp_view,                                     \
      const char*: _strview_cmp_cstr)(str1, str2)

u64 _strview_find(const strview_t* src, const strbuf_t* search);
u64 _strview_find_view(const strview_t* src, const strview_t* search);
u64 _strview_find_cstr(const strview_t* src, const char* search);

#define strview_find(src, find)                                                \
  _Generic((find),                                                             \
      strbuf_t *: _strview_find,                                               \
      strview_t *: _strview_find_view,                                         \
      char*: _strview_find_cstr,                                               \
      const strbuf_t*: _strview_find,                                          \
      const strview_t*: _strview_find_view,                                    \
      const char*: _strview_find_cstr)(src, find)

u64 _strview_rfind(const strview_t* src, const strbuf_t* search);
u64 _strview_rfind_view(const strview_t* src, const strview_t* search);
u64 _strview_rfind_cstr(const strview_t* src, const char* search);

#define strview_rfind(src, find)                                               \
  _Generic((find),                                                             \
      strbuf_t *: _strview_rfind,                                              \
      strview_t *: _strview_rfind_view,                                        \
      char*: _strview_rfind_cstr,                                              \
      const strbuf_t*: _strview_rfind,                                         \
      const strview_t*: _strview_rfind_view,                                   \
      const char*: _strview_rfind_cstr)(src, find)

DYARR(u64) _strview_find_range(const strview_t* src, const strbuf_t* search);
DYARR(u64)
_strview_find_range_view(const strview_t* src, const strview_t* search);
DYARR(u64) _strview_find_range_cstr(const strview_t* src, const char* search);

// Returns dynamic array and will need to be destroyed
#define strview_find_range(src, find)                                          \
  _Generic((find),                                                             \
      strbuf_t *: _strview_find_range,                                         \
      strview_t *: _strview_find_range_view,                                   \
      char*: _strview_find_range_cstr,                                         \
      const strbuf_t*: _strview_find_range,                                    \
      const strview_t*: _strview_find_range_view,                              \
      const char*: _strview_find_range_cstr)(src, find)

#endif

#ifndef IGNIS_CORE_STRVIEW_H
#define IGNIS_CORE_STRVIEW_H

#include "ignis/containers/strbuf.h"
#include "ignis/core/defines.h"

typedef struct strview {
  usize       len;
  const char* cstr;
} strview_t;

#define EMPTY_STRVIEW                                                          \
  (strview_t) {                                                                \
    .len = 0, .cstr = NULL,                                                    \
  }

strview_t _strview_ref(const strbuf_t* str);
strview_t _strview_ref_view(const strview_t* str);
strview_t _strview_ref_cstr(const char* str);

#define strview(str)                                                           \
  _Generic((str),                                                              \
      strbuf_t *: _strview_ref,                                                \
      strview_t *: _strview_ref_view,                                          \
      char*: _strview_ref_cstr,                                                \
      const strbuf_t*: _strview_ref,                                           \
      const strview_t*: _strview_ref_view,                                     \
      const char*: _strview_ref_cstr)(str)

b8 _strview_cmp_strview(const strview_t* str1, const strbuf_t* str2);
b8 _strview_cmp_view(const strview_t* view1, const strview_t* view2);
b8 _strview_cmp_cstr(const strview_t* view, const char* cstr);

#define strview_cmp(str1, str2)                                                \
  _Generic((str2),                                                             \
      strbuf_t *: _strview_append,                                             \
      strview_t *: _strview_append_view,                                       \
      char*: _strview_append_cstr,                                             \
      const strbuf_t*: _strview_append,                                        \
      const strview_t*: _strview_append_view,                                  \
      const char*: _strview_append_cstr)(str1, str2)

usize _strview_find(const strview_t* src, const strbuf_t* search);
usize _strview_find_view(const strview_t* src, const strview_t* search);
usize _strview_find_cstr(const strview_t* src, const char* search);

#define strview_find(src, find)                                                \
  _Generic((src),                                                              \
      strbuf_t *: _strview_find,                                               \
      strview_t *: _strview_find_view,                                         \
      char*: _strview_find_cstr,                                               \
      const strbuf_t*: _strview_find,                                          \
      const strview_t*: _strview_find_view,                                    \
      const char*: _strview_find_cstr)(src, find)

usize _strview_rfind(const strview_t* src, const strbuf_t* search);
usize _strview_rfind_view(const strview_t* src, const strview_t* search);
usize _strview_rfind_cstr(const strview_t* src, const char* search);

#define strview_rfind(src, find)                                               \
  _Generic((src),                                                              \
      strbuf_t *: _strview_rfind,                                              \
      strview_t *: _strview_rfind_view,                                        \
      char*: _strview_rfind_cstr,                                              \
      const strbuf_t*: _strview_rfind,                                         \
      const strview_t*: _strview_rfind_view,                                   \
      const char*: _strview_rfind_cstr)(src, find)

i32* _strview_find_range(const strview_t* src, const strbuf_t* search);
i32* _strview_find_range_view(const strview_t* src, const strview_t* search);
i32* _strview_find_range_cstr(const strview_t* src, const char* search);

#define strview_find_range(dest, src)                                          \
  _Generic((src),                                                              \
      strbuf_t *: _strview_find_range,                                         \
      strview_t *: _strview_find_range_view,                                   \
      char*: _strview_find_range_cstr,                                         \
      const strbuf_t*: _strview_find_range,                                    \
      const strview_t*: _strview_find_range_view,                              \
      const char*: _strview_find_range_cstr)(dest, src)

#endif

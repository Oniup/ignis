#ifndef IGNIS_CORE_STRBUF_H
#define IGNIS_CORE_STRBUF_H

#include "ignis/core/defines.h"
#include <stdio.h>
#include <string.h>

typedef struct strview strview_t;

typedef struct strbuf {
  usize len;
  usize cap;
  char* cstr;
} strbuf_t;

#define EMPTY_STRBUF                                                           \
  (strbuf_t) {                                                                 \
    .len = 0, .cap = 0, .cstr = NULL,                                          \
  }

void strbuf_destroy(strbuf_t* str);

// TODO:
strview_t strbuf_slice(const strbuf_t* str, usize beg, usize end);
strbuf_t  strbuf_read_file(FILE* file);

// TODO:
void strbuf_to_upper(strbuf_t* dest);
void strbuf_to_lower(strbuf_t* dest);
void strbuf_to_pascal(strbuf_t* dest);
void strbuf_to_camel(strbuf_t* dest);
void strbuf_to_normal_spaces(strbuf_t* dest);

strbuf_t _strbuf_create(const strbuf_t* view);
strbuf_t _strbuf_create_view(const strview_t* view);
strbuf_t _strbuf_create_cstr(const char* cstr);
strbuf_t _strbuf_create_cap(i32 len);

#define strbuf_create(str)                                                     \
  _Generic((str),                                                              \
      strbuf_t *: _strbuf_create,                                              \
      strview_t *: _strbuf_create_view,                                        \
      char*: _strbuf_create_cstr,                                              \
      const strbuf_t*: _strbuf_create,                                         \
      const strview_t*: _strbuf_create_view,                                   \
      const char*: _strbuf_create_cstr,                                        \
      int: _strbuf_create_cap)(str)

// TODO:
strbuf_t _strbuf_move(const strbuf_t* view);
strbuf_t _strbuf_move_view(const strview_t* view);
strbuf_t _strbuf_move_cstr(const char* cstr);

#define strbuf_move(str)                                                       \
  _Generic((str),                                                              \
      strbuf_t *: _strbuf_move,                                                \
      strview_t *: _strbuf_move_view,                                          \
      char*: _strbuf_move_cstr)(str)

// TODO:
b8 _strbuf_cpy(const strbuf_t* dest, const strbuf_t* src);
b8 _strbuf_cpy_view(const strbuf_t* dest, const strview_t* src);
b8 _strbuf_cpy_cstr(const strbuf_t* dest, const char* src);

#define strbuf_cpy(dest, src)                                                  \
  _Generic((src),                                                              \
      strbuf_t *: _strbuf_cpy,                                                 \
      strview_t *: _strbuf_cpy_view,                                           \
      char*: _strbuf_cpy_cstr,                                                 \
      const strbuf_t*: _strbuf_cpy,                                            \
      const strview_t*: _strbuf_cpy_view,                                      \
      const char*: _strbuf_cpy_cstr)(dest, src)

// TODO:
b8 _strbuf_append(const strbuf_t* dest, const strbuf_t* src);
b8 _strbuf_append_view(const strbuf_t* dest, const strview_t* src);
b8 _strbuf_append_cstr(const strbuf_t* dest, const char* src);

#define strbuf_append(dest, src)                                               \
  _Generic((src),                                                              \
      strbuf_t *: _strbuf_append,                                              \
      strview_t *: _strbuf_append_view,                                        \
      char*: _strbuf_append_cstr,                                              \
      const strbuf_t*: _strbuf_append,                                         \
      const strview_t*: _strbuf_append_view,                                   \
      const char*: _strbuf_append_cstr)(dest, src)

b8 _strbuf_cmp_strbuf(const strbuf_t* str1, const strbuf_t* str2);
b8 _strbuf_cmp_view(const strbuf_t* str, const strview_t* view);
b8 _strbuf_cmp_cstr(const strbuf_t* str, const char* cstr);

#define strbuf_cmp(str1, str2)                                                 \
  _Generic((str2),                                                             \
      strbuf_t *: _strbuf_append,                                              \
      strview_t *: _strbuf_append_view,                                        \
      char*: _strbuf_append_cstr,                                              \
      const strbuf_t*: _strbuf_append,                                         \
      const strview_t*: _strbuf_append_view,                                   \
      const char*: _strbuf_append_cstr)(str1, str2)

// TODO:
usize _strbuf_find(const strbuf_t* dest, const strbuf_t* search);
usize _strbuf_find_view(const strbuf_t* dest, const strview_t* search);
usize _strbuf_find_cstr(const strbuf_t* dest, const char* search);

#define strbuf_find(src, find)                                                 \
  _Generic((src),                                                              \
      strbuf_t *: _strbuf_find,                                                \
      strview_t *: _strbuf_find_view,                                          \
      char*: _strbuf_find_cstr,                                                \
      const strbuf_t*: _strbuf_find,                                           \
      const strview_t*: _strbuf_find_view,                                     \
      const char*: _strbuf_find_cstr)(src, find)

// TODO:
usize _strbuf_rfind(const strbuf_t* dest, const strbuf_t* search);
usize _strbuf_rfind_view(const strbuf_t* dest, const strview_t* search);
usize _strbuf_rfind_cstr(const strbuf_t* dest, const char* search);

#define strbuf_rfind(src, find)                                                \
  _Generic((src),                                                              \
      strbuf_t *: _strbuf_rfind,                                               \
      strview_t *: _strbuf_rfind_view,                                         \
      char*: _strbuf_rfind_cstr,                                               \
      const strbuf_t*: _strbuf_rfind,                                          \
      const strview_t*: _strbuf_rfind_view,                                    \
      const char*: _strbuf_rfind_cstr)(src, find)

// TODO:
i32* _strbuf_find_range(const strbuf_t* src, const strbuf_t* search);
i32* _strbuf_find_range_view(const strbuf_t* src, const strview_t* search);
i32* _strbuf_find_range_cstr(const strbuf_t* src, const char* search);

#define strbuf_find_range(dest, src)                                           \
  _Generic((src),                                                              \
      strbuf_t *: _strbuf_find_range,                                          \
      strview_t *: _strbuf_find_range_view,                                    \
      char*: _strbuf_find_range_cstr,                                          \
      const strbuf_t*: _strbuf_find_range,                                     \
      const strview_t*: _strbuf_find_range_view,                               \
      const char*: _strbuf_find_range_cstr)(dest, src)

#define strbuf_cat(str, ...)                                                   \
  ({                                                                           \
    strview_t append[] = {__VA_ARGS__};                                        \
    for (i32 i = 0; i < NARG(__VA_ARGS__); i++) {                              \
      strbuf_append(str, append[i]);                                           \
    }                                                                          \
  })

#endif

#ifndef IGNIS_CORE_STRBUF_H
#define IGNIS_CORE_STRBUF_H

#include "ignis/containers/dyarr.h"
#include "ignis/core/defines.h"
#include <stdio.h>
#include <string.h>

typedef struct strview strview_t;

typedef struct strbuf {
    char* cstr;
    u64   len;
    u64   cap;
} strbuf_t;

#define EMPTY_STRBUF                                                           \
    (strbuf_t) {                                                               \
        .len = 0, .cap = 0, .cstr = NULL,                                      \
    }

void strbuf_destroy(strbuf_t* str);
void strbuf_move(strbuf_t* dest, strbuf_t* str);
void strbuf_resize(strbuf_t* dest, u64 len);
void strbuf_realloc(strbuf_t* dest, u64 cap);
void strbuf_clear(strbuf_t* dest);

strbuf_t strbuf_read_file(FILE* file);

void strbuf_to_upper(strbuf_t* dest);
void strbuf_to_lower(strbuf_t* dest);

void strbuf_to_pascal(strbuf_t* dest, b8 remove_spaces);
void strbuf_to_camel(strbuf_t* dest, b8 remove_spaces);

// @param fmt_caps: To format capitalized characters after the first or after a
// period.
void strbuf_to_normal_text(strbuf_t* dest, b8 fmt_caps);

b8 strbuf_vfmt(strbuf_t* dest, const strview_t* fmt, va_list args);
b8 strbuf_fmt(strbuf_t* dest, const strview_t* fmt, ...);

strbuf_t _strbuf_create(const strbuf_t* view);
strbuf_t _strbuf_create_view(const strview_t* view);
strbuf_t _strbuf_create_cstr(const char* cstr);
strbuf_t _strbuf_create_cap(u64 cap);

#define strbuf_create(str)                                                     \
    _Generic((str),                                                            \
        strbuf_t *: _strbuf_create,                                            \
        strview_t *: _strbuf_create_view,                                      \
        char*: _strbuf_create_cstr,                                            \
        const strbuf_t*: _strbuf_create,                                       \
        const strview_t*: _strbuf_create_view,                                 \
        const char*: _strbuf_create_cstr,                                      \
        i32: _strbuf_create_cap,                                               \
        u32: _strbuf_create_cap,                                               \
        u64: _strbuf_create_cap)(str)

void _strbuf_cpy(strbuf_t* dest, const strbuf_t* src);
void _strbuf_cpy_view(strbuf_t* dest, const strview_t* src);
void _strbuf_cpy_cstr(strbuf_t* dest, const char* src);

#define strbuf_cpy(dest, src)                                                  \
    _Generic((src),                                                            \
        strbuf_t *: _strbuf_cpy,                                               \
        strview_t *: _strbuf_cpy_view,                                         \
        char*: _strbuf_cpy_cstr,                                               \
        const strbuf_t*: _strbuf_cpy,                                          \
        const strview_t*: _strbuf_cpy_view,                                    \
        const char*: _strbuf_cpy_cstr)(dest, src)

void _strbuf_append(strbuf_t* dest, const strbuf_t* src);
void _strbuf_append_view(strbuf_t* dest, const strview_t* src);
void _strbuf_append_cstr(strbuf_t* dest, const char* src);

#define strbuf_append(dest, src)                                               \
    _Generic((src),                                                            \
        strbuf_t *: _strbuf_append,                                            \
        strview_t *: _strbuf_append_view,                                      \
        char*: _strbuf_append_cstr,                                            \
        const strbuf_t*: _strbuf_append,                                       \
        const strview_t*: _strbuf_append_view,                                 \
        const char*: _strbuf_append_cstr)(dest, src)

b8 _strbuf_cmp(const strbuf_t* str1, const strbuf_t* str2);
b8 _strbuf_cmp_view(const strbuf_t* str, const strview_t* view);
b8 _strbuf_cmp_cstr(const strbuf_t* str, const char* cstr);

#define strbuf_cmp(str1, str2)                                                 \
    _Generic((str2),                                                           \
        strbuf_t *: _strbuf_cmp,                                               \
        strview_t *: _strbuf_cmp_view,                                         \
        char*: _strbuf_cmp_cstr,                                               \
        const strbuf_t*: _strbuf_cmp,                                          \
        const strview_t*: _strbuf_cmp_view,                                    \
        const char*: _strbuf_cmp_cstr)(str1, str2)

u64 _strbuf_find(const strbuf_t* src, const strbuf_t* search);
u64 _strbuf_find_view(const strbuf_t* src, const strview_t* search);
u64 _strbuf_find_cstr(const strbuf_t* src, const char* search);

#define strbuf_find(src, search)                                               \
    _Generic((search),                                                         \
        strbuf_t *: _strbuf_find,                                              \
        strview_t *: _strbuf_find_view,                                        \
        char*: _strbuf_find_cstr,                                              \
        const strbuf_t*: _strbuf_find,                                         \
        const strview_t*: _strbuf_find_view,                                   \
        const char*: _strbuf_find_cstr)(src, search)

u64 _strbuf_rfind(const strbuf_t* src, const strbuf_t* search);
u64 _strbuf_rfind_view(const strbuf_t* src, const strview_t* search);
u64 _strbuf_rfind_cstr(const strbuf_t* src, const char* search);

#define strbuf_rfind(src, search)                                              \
    _Generic((search),                                                         \
        strbuf_t *: _strbuf_rfind,                                             \
        strview_t *: _strbuf_rfind_view,                                       \
        char*: _strbuf_rfind_cstr,                                             \
        const strbuf_t*: _strbuf_rfind,                                        \
        const strview_t*: _strbuf_rfind_view,                                  \
        const char*: _strbuf_rfind_cstr)(src, search)

DYARR(u64) _strbuf_find_range(const strbuf_t* src, const strbuf_t* search);
DYARR(u64)
_strbuf_find_range_view(const strbuf_t* src, const strview_t* search);
DYARR(u64) _strbuf_find_range_cstr(const strbuf_t* src, const char* search);

// Returns dynamic array and will need to be destroyed
#define strbuf_find_range(src, search)                                         \
    _Generic((search),                                                         \
        strbuf_t *: _strbuf_find_range,                                        \
        strview_t *: _strbuf_find_range_view,                                  \
        char*: _strbuf_find_range_cstr,                                        \
        const strbuf_t*: _strbuf_find_range,                                   \
        const strview_t*: _strbuf_find_range_view,                             \
        const char*: _strbuf_find_range_cstr)(src, search)

#define strbuf_join(...)                                                       \
    ({                                                                         \
        strbuf_t buf = EMPTY_STRBUF;                                           \
        strbuf_cat(&buf, __VA_ARGS__);                                         \
        buf;                                                                   \
    })

#define strbuf_cat(str, ...)                                                   \
    ({                                                                         \
        strview_t append[] = {__VA_ARGS__};                                    \
        for (u64 i = 0; i < NARG(__VA_ARGS__); i++) {                          \
            strbuf_append(str, &append[i]);                                    \
        }                                                                      \
    })

#endif

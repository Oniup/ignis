#include "ignis/containers/strbuf.h"
#include "ignis/containers/strview.h"
#include "ignis/core/log.h"
#include "ignis/core/memory.h"
#include <ctype.h>
#include <string.h>

static const char cnvt_chars[]   = {' ', '-', '_', '\n', '\t'};
static const u64  cnvt_chars_len = 5;

void strbuf_destroy(strbuf_t* str) {
    if (str->cstr) {
        mem_destroy(str->cstr, str->cap, MEM_TAG_STRBUF);
        *str = EMPTY_STRBUF;
    }
}

void strbuf_move(strbuf_t* dest, strbuf_t* str) {
    ASSERT(dest, "Cannot move str to dest: dest is null");
    ASSERT(str, "Cannot move str to dest: str is null");

    if (dest->cstr) {
        strbuf_destroy(dest);
    }

    dest->cstr = str->cstr;
    dest->len  = str->len;
    dest->cap  = str->cap;
    *str       = EMPTY_STRBUF;
}

void strbuf_resize(strbuf_t* dest, u64 len) {
    ASSERT(dest, "Cannot resize strbuf: dest is null");
    if (len > dest->cap) {
        strbuf_realloc(dest, len + 1);
    }
    dest->len = len;
}

void strbuf_realloc(strbuf_t* dest, u64 cap) {
    ASSERT(dest, "Cannot realloc strbuf: dest is null");
    dest->cstr = mem_realloc(dest->cstr, dest->cap, cap, MEM_TAG_STRBUF);
    dest->cap  = cap;
}

void strbuf_clear(strbuf_t* dest) {
    ASSERT(dest, "Cannot clear strbuf: dest is null");
    if (dest->cstr) {
        dest->cstr[0] = '\0';
        dest->len     = 0;
    }
}

void strbuf_to_upper(strbuf_t* dest) {
    ASSERT(dest, "Cannot convert to upper case: dest is null");
    for (u64 i = 0; i < dest->len; i++) {
        dest->cstr[i] = toupper(dest->cstr[i]);
    }
}

void strbuf_to_lower(strbuf_t* dest) {
    ASSERT(dest, "Cannot convert to lower case: dest is null");
    for (u64 i = 0; i < dest->len; i++) {
        dest->cstr[i] = tolower(dest->cstr[i]);
    }
}

void strbuf_to_pascal(strbuf_t* dest, b8 remove_spaces) {
    ASSERT(dest, "Cannot convert to Pascal case: dest is null");
    if (dest->len == 0) {
        return;
    }

    dest->cstr[0] = toupper(dest->cstr[0]);
    for (u64 i = 1; i < dest->len; i++) {
        b8 skip_to_lower = false;
        for (u64 j = 0; j < cnvt_chars_len; j++) {
            if (dest->cstr[i - 1] == cnvt_chars[j]) {
                dest->cstr[i] = toupper(dest->cstr[i]);
                i++;
                break;
            }
        }
        if (!skip_to_lower) {
            dest->cstr[i] = tolower(dest->cstr[i]);
        }
    }

    if (remove_spaces) {
        u64 k = 0;
        for (u64 i = 0; i < dest->len; i++) {
            b8 add_to_final = true;
            for (u64 j = 0; j < cnvt_chars_len; j++) {
                if (dest->cstr[i] == cnvt_chars[j]) {
                    add_to_final = false;
                    break;
                }
            }
            if (add_to_final) {
                dest->cstr[k++] = dest->cstr[i];
            }
        }
        dest->len     = k;
        dest->cstr[k] = '\0';
    }
}

void strbuf_to_camel(strbuf_t* dest, b8 remove_spaces) {
    ASSERT(dest, "Cannot convert to camel case: dest is null");
    if (dest->len == 0) {
        return;
    }

    dest->cstr[0] = tolower(dest->cstr[0]);
    for (u64 i = 1; i < dest->len; i++) {
        b8 skip_to_lower = false;
        for (u64 j = 0; j < cnvt_chars_len; j++) {
            if (dest->cstr[i - 1] == cnvt_chars[j]) {
                dest->cstr[i] = toupper(dest->cstr[i]);
                skip_to_lower = true;
                break;
            }
        }
        if (!skip_to_lower) {
            dest->cstr[i] = tolower(dest->cstr[i]);
        }
    }

    if (remove_spaces) {
        u64 k = 0;
        for (u64 i = 0; i < dest->len; i++) {
            b8 add_to_final = true;
            for (u64 j = 0; j < cnvt_chars_len; j++) {
                if (dest->cstr[i] == cnvt_chars[j]) {
                    add_to_final = false;
                    break;
                }
            }
            if (add_to_final) {
                dest->cstr[k++] = dest->cstr[i];
            }
        }
        dest->len     = k;
        dest->cstr[k] = '\0';
    }
}

void strbuf_to_normal_text(strbuf_t* dest, b8 fmt_caps) {
    ASSERT(dest, "Cannot convert to normal text: dest is null");
    if (dest->len == 0) {
        return;
    }

    strbuf_t normal_text = strbuf_create(dest->len);

    u64 j         = 0;
    b8  keep_caps = false;
    for (u64 i = 0; i < dest->len; i++) {
        if (dest->cstr[i] == '_') {
            strbuf_resize(&normal_text, j + 1);
            normal_text.cstr[j++] = ' ';
        } else if (i > 0 && isupper(dest->cstr[i])) {
            strbuf_resize(&normal_text, j + 2);
            normal_text.cstr[j++] = ' ';
            if (!keep_caps && fmt_caps) {
                normal_text.cstr[j++] = tolower(dest->cstr[i]);
            } else {
                normal_text.cstr[j++] = dest->cstr[i];
                keep_caps             = false;
            }
        } else {
            strbuf_resize(&normal_text, j + 1);
            normal_text.cstr[j++] = dest->cstr[i];
        }
    }

    strbuf_move(dest, &normal_text);
}

strbuf_t _strbuf_create(const strbuf_t* str) {
    ASSERT(str, "Cannot create strbuf: strbuf passed to copy is null");
    if (!str->cstr) {
        return EMPTY_STRBUF;
    }
    u64      cap = str->len + 1;
    strbuf_t buf = {
        .len  = str->len,
        .cap  = cap,
        .cstr = mem_alloc(cap, MEM_TAG_STRBUF),
    };
    buf.cstr[buf.len] = '\0';
    for (u64 i = 0; i < cap; i++) {
        buf.cstr[i] = str->cstr[i];
    }
    return buf;
}

strbuf_t _strbuf_create_view(const strview_t* view) {
    ASSERT(view, "Cannot create view: view passed to copy is null");
    if (!view->cstr) {
        return EMPTY_STRBUF;
    }
    u64      cap = view->len + 1;
    strbuf_t buf = {
        .len  = view->len,
        .cap  = cap,
        .cstr = mem_alloc(cap, MEM_TAG_STRBUF),
    };
    for (u64 i = 0; i < cap; i++) {
        buf.cstr[i] = view->cstr[i];
    }
    buf.cstr[buf.len] = '\0';
    return buf;
}

strbuf_t _strbuf_create_cstr(const char* cstr) {
    if (!cstr) {
        return EMPTY_STRBUF;
    }
    return _strbuf_create_view(
        &(strview_t) {.cstr = cstr, .len = strlen(cstr)});
}

strbuf_t _strbuf_create_cap(u64 cap) {
    strbuf_t buf = {
        .len  = 0,
        .cap  = cap,
        .cstr = mem_alloc(cap, MEM_TAG_STRBUF),
    };
    return buf;
}

void _strbuf_cpy(strbuf_t* dest, const strbuf_t* src) {
    ASSERT(dest, "Cannot copy string to string buffer: dest strbuf is Null");
    if (!src) {
        strbuf_clear(dest);
        return;
    }
    strbuf_resize(dest, src->len);
    for (u64 i = 0; i < dest->len; i++) {
        dest->cstr[i] = src->cstr[i];
    }
    dest->cstr[dest->len] = '\0';
}

void _strbuf_cpy_view(strbuf_t* dest, const strview_t* src) {
    ASSERT(dest, "Cannot copy string to view buffer: dest str is null");
    if (!src) {
        strbuf_clear(dest);
    }
    strbuf_resize(dest, src->len);
    for (u64 i = 0; i < dest->len; i++) {
        dest->cstr[i] = src->cstr[i];
    }
    dest->cstr[dest->len] = '\0';
}

void _strbuf_cpy_cstr(strbuf_t* dest, const char* src) {
    strview_t view = strview(src);
    _strbuf_cpy_view(dest, &view);
}

void _strbuf_append(strbuf_t* dest, const strbuf_t* src) {
    ASSERT(dest, "Cannot append string to view buffer: dest str is null");
    if (src->len == 0 || !src->cstr || src->cstr[0] == '\0') {
        return;
    }
    u64 old_len = dest->len;
    strbuf_resize(dest, dest->len + src->len);
    char* offset = dest->cstr + old_len;
    for (u64 i = 0; i < src->len; i++) {
        offset[i] = src->cstr[i];
    }
    dest->cstr[dest->len] = '\0';
}

void _strbuf_append_view(strbuf_t* dest, const strview_t* src) {
    ASSERT(dest, "Cannot append string to view buffer: dest str is null");
    if (src->len == 0 || !src->cstr || src->cstr[0] == '\0') {
        return;
    }
    u64 old_len = dest->len;
    strbuf_resize(dest, dest->len + src->len);
    char* offset = dest->cstr + old_len;
    for (u64 i = 0; i < src->len; i++) {
        offset[i] = src->cstr[i];
    }
    dest->cstr[dest->len] = '\0';
}

void _strbuf_append_cstr(strbuf_t* dest, const char* src) {
    strview_t view = strview(src);
    _strbuf_append_view(dest, &view);
}

b8 _strbuf_cmp(const strbuf_t* str1, const strbuf_t* str2) {
    if ((!str1 || !str2) || str1->len != str2->len) {
        return false;
    }
    if (str1 == str2) {
        return true;
    }
    for (u64 i = 0; i < str1->len; i++) {
        if (str1->cstr[i] != str2->cstr[i]) {
            return false;
        }
    }
    return true;
}

b8 _strbuf_cmp_view(const strbuf_t* str, const strview_t* view) {
    return strview_cmp(view, str);
}

b8 _strbuf_cmp_cstr(const strbuf_t* str, const char* cstr) {
    strview_t view = strview(cstr);
    const i32 val  = 123;
    return strview_cmp(&view, str);
}

u64 _strbuf_find(const strbuf_t* src, const strbuf_t* search) {
    strview_t view = strview(src);
    return strview_find(&view, search);
}

u64 _strbuf_find_view(const strbuf_t* src, const strview_t* search) {
    strview_t view = strview(src);
    return strview_find(&view, search);
}

u64 _strbuf_find_cstr(const strbuf_t* src, const char* search) {
    strview_t view = strview(src);
    return strview_find(&view, search);
}

u64 _strbuf_rfind(const strbuf_t* src, const strbuf_t* search) {
    strview_t view = strview(src);
    return strview_rfind(&view, search);
}

u64 _strbuf_rfind_view(const strbuf_t* src, const strview_t* search) {
    strview_t view = strview(src);
    return strview_rfind(&view, search);
}

u64 _strbuf_rfind_cstr(const strbuf_t* src, const char* search) {
    strview_t view = strview(src);
    return strview_rfind(&view, search);
}

DYARR(u64) _strbuf_find_range(const strbuf_t* src, const strbuf_t* search) {
    strview_t view = strview(src);
    return strview_find_range(&view, search);
}

DYARR(u64)
_strbuf_find_range_view(const strbuf_t* src, const strview_t* search) {
    strview_t view = strview(src);
    return strview_find_range(&view, search);
}

DYARR(u64)
_strbuf_find_range_cstr(const strbuf_t* src, const char* search) {
    strview_t view = strview(src);
    return strview_find_range(&view, search);
}

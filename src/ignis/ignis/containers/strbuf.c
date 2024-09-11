#include "ignis/containers/strbuf.h"
#include "ignis/containers/strview.h"
#include "ignis/core/memory.h"
#include <string.h>

void strbuf_destroy(strbuf_t* str) {
  if (str->cstr) {
    mem_destroy(str->cstr, str->cap, MEM_TAG_STRBUF);
    *str = EMPTY_STRBUF;
  }
}

strbuf_t _strbuf_create(const strbuf_t* str) {
}

strbuf_t _strbuf_create_view(const strview_t* view) {
  usize    cap = view->len + 1;
  strbuf_t buf = {
      .cstr = mem_alloc(cap, MEM_TAG_STRBUF),
      .len  = view->len,
      .cap  = cap,
  };
  buf.cstr[buf.len] = '\0';
  return buf;
}

strbuf_t _strbuf_create_cstr(const char* cstr) {
  return _strbuf_create_view(&(strview_t) {.cstr = cstr, .len = strlen(cstr)});
}

strbuf_t _strbuf_create_cap(i32 len) {
}

b8 _strbuf_cmp_strbuf(const strbuf_t* str1, const strbuf_t* str2) {
  if (str1 == str2) {
    return true;
  }
  if ((!str1 || !str2) && str1->len != str2->len) {
    return false;
  }
  for (usize i = 0; i < str1->len; i++) {
    if (str1->cstr[i] != str2->cstr[i]) {
      return false;
    }
  }
  return true;
}

b8 _strbuf_cmp_view(const strbuf_t* str, const strview_t* view) {
  return _strview_cmp_strview(view, str);
}

b8 _strbuf_cmp_cstr(const strbuf_t* str, const char* cstr) {
}

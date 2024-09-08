#include "ignis/containers/strbuf.h"
#include <stdlib.h>

void strbuf_destroy(strbuf_t* str) {
  if (str->cstr) {
    free(str->cstr);
    *str = EMPTY_STRBUF;
  }
}

strbuf_t _strbuf_create(const strbuf_t* view) {
}

strbuf_t _strbuf_create_view(const strview_t* view) {
}

strbuf_t _strbuf_create_cstr(const char* cstr) {
}

strbuf_t _strbuf_create_cap(i32 len) {
}

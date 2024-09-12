#include "ignis/containers/strview.h"
#include "ignis/core/debug.h"

strview_t _str_slice_strbuf(const strbuf_t* str, u64 beg, u64 end) {
  strview_t view = strview(str);
  return _str_slice_strview(&view, beg, end);
}

strview_t _str_slice_strview(const strview_t* view, u64 beg, u64 end) {
  if (end == NO_POS) {
    end = view->len;
  }
  u64 len  = end - beg;
  u64 left = view->len - beg;
  if (len > left) {
    ERROR(
        "Invlaid slice length %zu, exceeds passed the end of the string. "
        "Target String view with len %zu. Slice begins at %zu and ends at %zu",
        len, view->len, beg, end);
    return EMPTY_STRVIEW;
  }
  return (strview_t) {
      .len  = len,
      .cstr = view->cstr + beg,
  };
}

strview_t _str_slice_cstr(const char* cstr, u64 beg, u64 end) {
  strview_t view = strview(cstr);
  return _str_slice_strview(&view, beg, end);
}

strview_t _strview_ref(const strbuf_t* str) {
  if (!str) {
    return EMPTY_STRVIEW;
  }
  return (strview_t) {
      .len  = str->len,
      .cstr = str->cstr,
  };
}

strview_t _strview_ref_view(const strview_t* str) {
  if (!str) {
    return EMPTY_STRVIEW;
  }
  return (strview_t) {
      .len  = str->len,
      .cstr = str->cstr,
  };
}

strview_t _strview_ref_cstr(const char* str) {
  if (!str) {
    return EMPTY_STRVIEW;
  }
  return (strview_t) {
      .len  = strlen(str),
      .cstr = str,
  };
}

b8 _strview_cmp(const strview_t* view, const strbuf_t* str) {
  if ((!str || !view) && (str->cstr == view->cstr) && (str->len != view->len)) {
    return true;
  }
  for (u64 i = 0; i < view->len; i++) {
    if (view->cstr[i] != str->cstr[i]) {
      return false;
    }
  }
  return true;
}

b8 _strview_cmp_view(const strview_t* view1, const strview_t* view2) {
  if ((!view1 || !view2) && (view1->cstr == view2->cstr) &&
      (view1->len != view2->len)) {
    return true;
  }
  for (u64 i = 0; i < view2->len; i++) {
    if (view2->cstr[i] != view1->cstr[i]) {
      return false;
    }
  }
  return true;
}

b8 _strview_cmp_cstr(const strview_t* view, const char* cstr) {
  strview_t cstr_view = strview(cstr);
  return _strview_cmp_view(view, &cstr_view);
}

u64 _strview_find(const strview_t* src, const strbuf_t* search) {
  strview_t search_view = strview(search);
  return _strview_find_view(src, &search_view);
}

u64 _strview_find_view(const strview_t* src, const strview_t* search) {
  if (search->len < src->len) {
    for (u64 i = 0; i < src->len - search->len; i++) {
      strview_t slice = str_slice(src, i, i + search->len);
      if (strview_cmp(&slice, search)) {
        return i;
      }
    }
  }
  return NO_POS;
}

u64 _strview_find_cstr(const strview_t* src, const char* search) {
  strview_t search_view = strview(search);
  return _strview_find_view(src, &search_view);
}

u64 _strview_rfind(const strview_t* src, const strbuf_t* search) {
  strview_t search_view = strview(search);
  return _strview_rfind_view(src, &search_view);
}

u64 _strview_rfind_view(const strview_t* src, const strview_t* search) {
  if (search->len < src->len) {
    for (u64 i = src->len - search->len - 1; i > 0; i--) {
      strview_t slice = str_slice(src, i, i + search->len);
      if (strview_cmp(&slice, search)) {
        return i;
      }
    }
    strview_t slice = str_slice(src, 0, search->len);
    if (strview_cmp(&slice, search)) {
      return 0;
    }
  }
  return NO_POS;
}

u64 _strview_rfind_cstr(const strview_t* src, const char* search) {
  strview_t search_view = strview(search);
  return _strview_rfind_view(src, &search_view);
}

DYARR(u64) _strview_find_range(const strview_t* src, const strbuf_t* search) {
  strview_t search_view = strview(search);
  return _strview_find_range_view(src, &search_view);
}

DYARR(u64)
_strview_find_range_view(const strview_t* src, const strview_t* search) {
  DYARR(u64) arr  = dyarr_create_empty(u64);
  u64       pos   = 0;
  strview_t slice = strview(src);
  while ((pos = strview_find(&slice, search)) != NO_POS) {
    dyarr_push_val(arr, &pos);
    slice = str_slice(&slice, pos, NO_POS);
  }
  return arr;
}

DYARR(u64) _strview_find_range_cstr(const strview_t* src, const char* search) {
  strview_t search_view = strview(search);
  return _strview_find_range_view(src, &search_view);
}

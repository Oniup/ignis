#include "ignis/containers/dyarr.h"
#include "ignis/core/debug.h"
#include <stdlib.h>
#include <string.h>

void* _dyarr_create(usize stride, usize size, usize capacity) {
  dyarr_header_t* header = malloc(sizeof(dyarr_header_t) + (stride * capacity));
  if (header == NULL) {
    FATAL("Failed to allocate memory todyarr list");
  }
  header->len    = size;
  header->cap    = capacity;
  header->stride = stride;
  return DYARR_DATA(header);
}

void dyarr_destroy(void* dyarr) {
  if (dyarr != NULL) {
    dyarr_header_t* header = DYARR_HEADER(dyarr);
    free(header);
  }
}

void* _dyarr_resize(void* dyarr, usize length) {
  if (length > dyarr_cap(dyarr)) {
    dyarr = _dyarr_realloc(dyarr, length);
  }
  dyarr_len(dyarr) = length;
  return dyarr;
}

void* _dyarr_realloc(void* dyarr, usize capacity) {
  dyarr_header_t* hdr = DYARR_HEADER(dyarr);
  hdr = realloc(hdr, sizeof(dyarr_header_t) + (hdr->stride * capacity));
  if (hdr == NULL) {
    FATAL("Failed to reallocate memory to dyarr list. Attepted to resize "
          "capacity to %zu",
          capacity);
  }
  hdr->cap = capacity;
  return DYARR_DATA(hdr);
}

void* _dyarr_push_back_val(void* dyarr, void* data) {
  ASSERT(dyarr != NULL, "Cannot push data to the back, dynamic array is NULL");
  usize len    = dyarr_len(dyarr);
  usize stride = dyarr_stride(dyarr);
  dyarr        = _dyarr_resize(dyarr, len + 1);
  memcpy(dyarr + len * stride, data, stride);
  return dyarr;
}

void* _dyarr_push_front_val(void* dyarr, void* data) {
  ASSERT(dyarr != NULL, "Cannot push data to the front, dynamic array is NULL");
  usize len    = dyarr_len(dyarr);
  usize stride = dyarr_stride(dyarr);
  dyarr        = _dyarr_resize(dyarr, len + 1);
  memmove(dyarr + stride, dyarr, stride * len);
  memcpy(dyarr, data, stride);
  return dyarr;
}

void* _dyarr_insert_val(void* dyarr, usize pos, void* data) {
  ASSERT(dyarr != NULL,
         "Cannot insert data at index %zu, dynamic array is NULL", pos);

  usize len    = dyarr_len(dyarr);
  usize stride = dyarr_stride(dyarr);
  ASSERT(
      pos <= len,
      "Cannot insert data into dynamic array at index %zu when length is %zu",
      pos, len);

  dyarr = _dyarr_resize(dyarr, len + 1);
  memmove(dyarr + (stride * (pos + 1)), dyarr + (stride * pos),
          stride * (len - pos));
  memcpy(dyarr + (stride * pos), data, stride);
  return dyarr;
}

void dyarr_pop(void* dyarr, void* popped) {
  ASSERT(dyarr != NULL, "Cannot pop back, dyarr is NULL");
  dyarr_header_t* hdr = DYARR_HEADER(dyarr);
  usize           pos = hdr->len - 1;

  if (popped != NULL) {
    memcpy(popped, dyarr + (hdr->stride * pos), hdr->stride);
  }
  dyarr_len(dyarr) = pos;
}

void dyarr_pop_at(void* dyarr, usize pos, void* popped) {
  ASSERT(dyarr != NULL, "Cannot erase at %zu, dyarr is NULL", pos);

  dyarr_header_t* hdr = DYARR_HEADER(dyarr);
  ASSERT(pos <= hdr->len,
         "Cannot erase at %zu, when dynamic array length is %zu", pos,
         hdr->len);

  if (popped != NULL) {
    memcpy(popped, dyarr + (hdr->stride * pos), hdr->stride);
  }
  memmove(dyarr + (hdr->stride * pos), dyarr + (hdr->stride * (pos + 1)),
          hdr->stride * (hdr->len - pos - 1));
  dyarr_len(dyarr) = hdr->len - 1;
}

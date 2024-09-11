#include "ignis/containers/list.h"
#include <memory.h>
#include <stdlib.h>

list_t list_create(usize elmt_size) {
  list_t list = {
      .elmt_size = elmt_size,
      .beg       = NULL,
      .end       = NULL,
  };
  return list;
}

void list_destroy(list_t* list) {
  list_elmt_hdr_t* next = list->beg;
  while (next != NULL) {
    list_elmt_hdr_t* curr = next;
    next                  = next->next;
    free(curr);
  }
  list->beg = NULL;
  list->end = NULL;
}

b8 list_push(list_t* list, void* val) {
  if (list->end != NULL) {
    list->end->next       = malloc(sizeof(list_elmt_hdr_t) + list->elmt_size);
    list->end->next->prev = list->end;
    list->end             = list->end->next;
  } else {
    list->beg       = malloc(sizeof(list_elmt_hdr_t) + list->elmt_size);
    list->beg->prev = NULL;
    list->end       = list->beg;
  }
  list->end->next = NULL;
  memcpy(list->end + 1, val, list->elmt_size);
}

b8 list_push_front(list_t* list) {
}

b8 list_pop(list_t* list) {
}

b8 list_pop_at(list_t* list) {
}

#ifndef IGNIS_CONTAINERS_LIST_H
#define IGNIS_CONTAINERS_LIST_H

#include "ignis/core/defines.h"

typedef struct list_elmt_hdr {
  struct list_elmt_hdr* prev;
  struct list_elmt_hdr* next;
} list_elmt_hdr_t;

typedef struct {
  usize            elmt_size;
  list_elmt_hdr_t* beg;
  list_elmt_hdr_t* end;
} list_t;

list_t list_create(usize elmt_size);
void   list_destroy(list_t* list);
b8     list_push(list_t* list, void* val);
b8     list_push_front(list_t* list);
b8     list_pop(list_t* list);
b8     list_pop_at(list_t* list);

#endif

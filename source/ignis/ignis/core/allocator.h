#ifndef IGNIS_CORE_ALLOCATOR_H
#define IGNIS_CORE_ALLOCATOR_H

#include "ignis/core/defines.h"

typedef struct mem_generic_heap_allocator mem_generic_heap_allocator_t;
typedef struct mem_frame_allocator        mem_frame_allocator_t;
typedef struct mem_static_allocator       mem_static_allocator_t;

struct mem_generic_heap_allocator {
  void* (*allocate_fn)(void* allocator, void* ptr, usize size);
  void* (*resize_fn)(void* allocator, void* ptr, usize old_size, usize size);
  void (*destroy_fn)(void* allocator, void* ptr, usize size);
};

struct mem_frame_allocator {
  void* (*allocate_fn)(void* allocator, usize size);
  void* (*resize_fn)(void* allocator, usize old_size, usize size);
  void (*destroy_fn)(void* allocator);
  void* ptr;
  b8    owned;
  usize size;
};

struct mem_static_allocator {
  void* (*allocate_fn)(void* allocator, usize size);
  void* (*resize_fn)(void* allocator, usize old_size, usize size);
  void (*destroy_fn)(void* allocator);
  void* ptr;
  usize size;
  usize capacity;
};

mem_generic_heap_allocator_t mem_generic_heap_allocator();
mem_frame_allocator_t        mem_frame_allocator(void* ptr);
mem_static_allocator_t       mem_static_allocator(void* ptr, usize capacity);

#endif

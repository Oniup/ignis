#ifndef IGNIS_CORE_MEMORY_MEMORY_H
#define IGNIS_CORE_MEMORY_MEMORY_H

#include "ignis/core/defines.h"

typedef enum {
  MEM_TAG_UNKNOWN,

  // Containers
  MEM_TAG_DYARR,
  MEM_TAG_LIST,
  MEM_TAG_STRBUF,

  // Engine
  MEM_TAG_ASSET_RESOURCE,
  MEM_TAG_SCENE,
  MEM_TAG_ENTITY,
  MEM_TAG_ENTITY_NODE,

  // Graphics
  MEM_TAG_VULKAN,
  MEM_TAG_OPENGL,

  MEM_TAG_MAX_TAGS,
} mem_tag_t;

void mem_system_init();
void mem_system_destroy();

void* mem_alloc(usize size, mem_tag_t tag);
void* mem_realloc(void* ptr, usize old_size, usize size, mem_tag_t tag);
void  mem_destroy(void* ptr, usize size, mem_tag_t tag);

#ifdef IGNIS_TRACE_MEMORY_STATS
usize _mem_alloc_count(mem_tag_t tag);
usize _mem_alloc_size(mem_tag_t tag);
usize _mem_total_alloc_count();
usize _mem_total_alloc_size();

#  define mem_alloc_count(_tag)   _mem_alloc_count(_tag)
#  define mem_alloc_size(_tag)    _mem_alloc_size(_tag)
#  define mem_total_alloc_count() _mem_total_alloc_count()
#  define mem_total_alloc_size()  _mem_total_alloc_size()
#else
#  define mem_alloc_count(_tag)   ((void)0)
#  define mem_alloc_size(_tag)    ((void)0)
#  define mem_total_alloc_count() ((void)0)
#  define mem_total_alloc_size()  ((void)0)
#endif

#endif

#include "ignis/core/memory.h"
#include "ignis/core/log.h"
#include "ignis/core/defines.h"
#include <CThreads/cthreads.h>
#include <stdlib.h>

#ifdef IGNIS_TRACE_MEMORY_STATS

typedef struct {
    usize allocs_size[MEM_TAG_MAX_TAGS];
    usize allocs_count[MEM_TAG_MAX_TAGS];
} mem_stats_t;

#endif

static const char* cstr_tags[MEM_TAG_MAX_TAGS] = {
    "UNKNOWN",

    // Containers
    "DYARR",
    "LIST",
    "STRBUF",

    // Engine
    "ASSET_RESOURCE",
    "SCENE",
    "ENTITY",
    "ENTITY_NODE",

    // Graphics
    "VULKAN",
    "OPENGL",
};

typedef struct {
#ifdef IGNIS_TRACE_MEMORY_STATS
    mem_stats_t stats;
#endif
    struct cthreads_mutex mtx;
} mem_state_t;

mem_state_t* s_state = NULL;

void mem_system_init() {
    s_state = malloc(sizeof(mem_state_t));
    if (!s_state) {
        FATAL("Failed to allocate memory state: Insufficient resources");
    }

#ifdef IGNIS_TRACE_MEMORY_STATS
    for (usize i = 0; i < MEM_TAG_MAX_TAGS; i++) {
        s_state->stats.allocs_size[i]  = 0;
        s_state->stats.allocs_count[i] = 0;
    }
#endif
    if (cthreads_mutex_init(&s_state->mtx, NULL) != 0) {
        FATAL("Memory system failed");
    }
}

void mem_system_destroy() {
    if (s_state) {
#ifdef IGNIS_TRACE_MEMORY_STATS
        for (usize i = 0; i < MEM_TAG_MAX_TAGS; i++) {
            if (s_state->stats.allocs_size[i] > 0) {
                ERROR("Leeking %zu [%s] memory allocations, (%zu bytes) ",
                      s_state->stats.allocs_count[i], cstr_tags[i],
                      s_state->stats.allocs_size[i]);
            }
        }
#endif
        cthreads_mutex_destroy(&s_state->mtx);
        free(s_state);
    }
}

void* mem_alloc(usize size, mem_tag_t tag) {
    if (size == 0) {
        return NULL;
    }

    cthreads_mutex_lock(&s_state->mtx);
    void* mem = malloc(size);
    if (mem == NULL) {
        FATAL("Failed to allocate memory: Insufficient resources");
    }
    cthreads_mutex_unlock(&s_state->mtx);

#ifdef IGNIS_TRACE_MEMORY_STATS
    s_state->stats.allocs_size[tag] += size;
    s_state->stats.allocs_count[tag]++;
#endif

    return mem;
}

void* mem_realloc(void* ptr, usize old_size, usize size, mem_tag_t tag) {
    if (!ptr) {
        return mem_alloc(size, tag);
    } else {
        ptr = realloc(ptr, size);
        if (ptr == NULL) {
            FATAL("Failed to re-allocate memory: Insufficient resources");
        }
    }

#ifdef IGNIS_TRACE_MEMORY_STATS
    s_state->stats.allocs_size[tag] += size - old_size;
#endif

    return ptr;
}

void mem_destroy(void* ptr, usize size, mem_tag_t tag) {
    free(ptr);
#ifdef IGNIS_TRACE_MEMORY_STATS
    s_state->stats.allocs_count[tag]--;
    s_state->stats.allocs_size[tag] -= size;
#endif
}

#ifdef IGNIS_TRACE_MEMORY_STATS

usize _mem_alloc_count(mem_tag_t tag) {
    return s_state->stats.allocs_count[tag];
}

usize _mem_alloc_size(mem_tag_t tag) {
    return s_state->stats.allocs_size[tag];
}

usize _mem_total_alloc_count() {
    usize total = 0;
    for (usize i = 0; i < MEM_TAG_MAX_TAGS; i++) {
        total += s_state->stats.allocs_count[i];
    }
    return total;
}

usize _mem_total_alloc_size() {
    usize total = 0;
    for (usize i = 0; i < MEM_TAG_MAX_TAGS; i++) {
        total += s_state->stats.allocs_count[i];
    }
    return total;
}

#endif

#ifndef IGNIS_CORE_THREADS_THREAD_H
#define IGNIS_CORE_THREADS_THREAD_H

#include "ignis/core/defines.h"

typedef struct thread thread_t;

typedef void(pfn_thread_start)(thread_t* thread, void* args);

typedef struct thread {
  void* intl_data;
  u64   id;
} thread_t;

b8   thread_init(pfn_thread_start start_fn, void* params, b8 auto_detach,
                 thread_t* thread);
void thread_destroy(thread_t* thread);

void thread_detach(thread_t* thread);
void thread_cancel(thread_t* thread);

b8 thread_wait(thread_t* thread);
b8 thread_wait_timeout(thread_t* thread, usize wait_ms);

b8   thread_is_active(thread_t* thread);
void thread_sleep(thread_t* thread, usize ms);

usize platform_current_thread_id(void);

#endif

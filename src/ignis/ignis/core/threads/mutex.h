#ifndef IGNIS_CORE_THREADS_MUTEX_H
#define IGNIS_CORE_THREADS_MUTEX_H

#include "ignis/core/defines.h"

typedef struct {
  void* intl_data;
} mutex_t;

b8   mutex_init(mutex_t* out_mutex);
void mutex_destroy(mutex_t* mutex);
b8   mutex_lock(mutex_t* mutex);
b8   mutex_unlock(mutex_t* mutex);

#endif

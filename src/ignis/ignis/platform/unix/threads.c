#include "ignis/platform/defines.h"

#ifdef PLATFORM_UNIX

#  include "ignis/core/debug.h"
#  include "ignis/core/threads/mutex.h"
#  include "ignis/core/threads/thread.h"
#  include <errno.h>
#  include <pthread.h>
#  include <stdlib.h>

b8 thread_init(pfn_thread_start start_fn, void* args, b8 auto_detach,
               thread_t* thread) {
  if (start_fn == NULL) {
    return false;
  }
  i32 res = pthread_create((pthread_t*)&thread->id, 0,
                           (void* (*)(void*))start_fn, args);
  switch (res) {
  case 0:
    break;
  case EAGAIN:
    ERROR("Failed to create thread: Not enough resources");
    return false;
  case EINVAL:
    ERROR("Failed to create thread: Invalid attributes");
    return false;
  default:
    ERROR("Failed to create thread: An unhandled error has occurred. errno=%i",
          res);
    return false;
  }
  return true;
}

void thread_destroy(thread_t* thread) {
}

void thread_detach(thread_t* thread) {
}

void thread_cancel(thread_t* thread) {
}

b8 thread_wait(thread_t* thread) {
  return true;
}

b8 thread_wait_timeout(thread_t* thread, usize wait_ms) {
  return true;
}

b8 thread_is_active(thread_t* thread) {
  return true;
}

void thread_sleep(thread_t* thread, usize ms) {
}

usize platform_current_thread_id(void) {
  return 0;
}

b8 mutex_init(mutex_t* out_mutex) {
  if (out_mutex == NULL) {
    ERROR("Mutex is null, cannot initialize");
    return false;
  }

  pthread_mutexattr_t mtx_attr;
  pthread_mutexattr_init(&mtx_attr);
  pthread_mutexattr_settype(&mtx_attr, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_t mtx;
  i32             res = pthread_mutex_init(&mtx, &mtx_attr);
  if (res != 0) {
    ERROR("PThreads failed to initialize mutex");
    return false;
  }

  out_mutex->intl_data                    = malloc(sizeof(pthread_mutex_t));
  *(pthread_mutex_t*)out_mutex->intl_data = mtx;
  return true;
}

void mutex_destroy(mutex_t* mutex) {
  if (mutex != NULL) {
    i32 res = pthread_mutex_destroy((pthread_mutex_t*)mutex->intl_data);
    switch (res) {
    case 0:
      break;
    case EBUSY:
      ERROR("Unable to destroy mutex: Is locked or referenced");
      break;
    case EINVAL:
      ERROR("Unable to destroy mutex: Invalid value specified by mutex");
      break;
    default:
      ERROR("An handled error has occurred while destroy a mutex: errno=%i",
            res);
      break;
    }

    free(mutex->intl_data);
    mutex->intl_data = NULL;
  }
}

b8 mutex_lock(mutex_t* mutex) {
  if (!mutex) {
    return false;
  }
  // Lock
  i32 result = pthread_mutex_lock((pthread_mutex_t*)mutex->intl_data);
  switch (result) {
  case 0:
    return true;
  case EOWNERDEAD:
    FATAL("Owning thread terminated while mutex still active");
    return false;
  case EAGAIN:
    ERROR("Unable to obtain mutex lock: the maximum number of recursive mutex "
          "locks has been reached");
    return false;
  case EBUSY:
    ERROR("Unable to obtain mutex lock: a mutex lock already exists");
    return false;
  case EDEADLK:
    ERROR("Unable to obtain mutex lock: a mutex deadlock was detected");
    return false;
  default:
    ERROR(
        "An handled error has occurred while obtaining a mutex lock: errno=%i",
        result);
    return false;
  }
}

b8 mutex_unlock(mutex_t* mutex) {
  if (!mutex) {
    return false;
  }
  if (mutex->intl_data) {
    i32 result = pthread_mutex_unlock((pthread_mutex_t*)mutex->intl_data);
    switch (result) {
    case 0:
      return true;
    case EOWNERDEAD:
      ERROR("Unable to unlock mutex: owning thread terminated while mutex "
            "still active");
      return false;
    case EPERM:
      ERROR("Unable to unlock mutex: mutex not owned by current thread");
      return false;
    default:
      ERROR("An handled error has occurred while unlocking a mutex lock: "
            "errno=%i",
            result);
      return false;
    }
  }

  return false;
}

#endif

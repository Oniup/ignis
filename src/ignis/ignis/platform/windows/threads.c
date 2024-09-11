#include "ignis/platform/defines.h"

#ifdef PLATFORM_WINDOWS

#  include "ignis/core/debug.h"
#  include "ignis/core/threads/thread.h"
#  include <errno.h>
#  include <stdlib.h>
#  include <windows.h>

b8 mutex_create(mutex_t* out_mutex) {
  if (!out_mutex) {
    return false;
  }

  out_mutex->intl_data = CreateMutex(0, 0, 0);
  if (out_mutex->intl_data == NULL) {
    ERROR("PThreads failed to initialize mutex");
    return false;
  }
  return true;
}

void mutex_destroy(mutex_t* mutex) {
  if (mutex && mutex->intl_data) {
    CloseHandle(mutex->intl_data);
    mutex->intl_data = 0;
  }
}

b8 mutex_lock(mutex_t* mutex) {
  if (!mutex) {
    return false;
  }
  DWORD res = WaitForSingleObject(mutex->intl_data, INFINITE);
  switch (res) {
  case WAIT_OBJECT_0:
    return true;
  case WAIT_ABANDONED:
    ERROR("unable to lock mutex: Thread got ownership of an abandoned mutex");
    return false;
  }
  return true;
}

b8 mutex_unlock(mutex_t* mutex) {
  if (!mutex || !mutex->intl_data) {
    return false;
  }
  i32 res = ReleaseMutex(mutex->intl_data);
  return res != 0;
}

#endif

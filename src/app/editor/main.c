#include <ignis/containers/dyarr.h>
#include <ignis/core/debug.h>
#include <stdio.h>

int main(int argc, char** argv) {
  int* test = NULL;
  log_context_init();
  log_context_add(log_terminal_output(LOG_DEFAULT_SEVERITY_FILTER |
                                      LOG_SEVERITY_INFO | LOG_SEVERITY_TRACE |
                                      LOG_SEVERITY_VERBOSE));
  log_context_add(log_file_output(LOG_DEFAULT_SEVERITY_FILTER));

  for (usize j = 0; j < 10; j++) {
    DYARR(int) arr = dyarr_create_empty(int);
    defer(dyarr_destroy(arr)) {
      dyarr_realloc(arr, 5);
      TRACE("len: %zu, cap: %zu", dyarr_len(arr), dyarr_cap(arr));

      dyarr_push(arr, 0);
      dyarr_push(arr, 1);
      dyarr_push(arr, 2);
      dyarr_push(arr, 3);
      dyarr_push(arr, 4);
      dyarr_push(arr, 5);
      dyarr_push(arr, 6);
      dyarr_push(arr, 7);
      dyarr_push(arr, 8);
      dyarr_push(arr, 9);

      dyarr_insert(arr, 5, 10);
      dyarr_insert(arr, 11, 11);
      dyarr_pop(arr, NULL);
      int popped;
      dyarr_pop_at(arr, 2, &popped);
      ASSERT(popped == 2, "Popped should be 2 not %d", popped);

      TRACE("len: %zu, cap: %zu, arr:", dyarr_len(arr), dyarr_cap(arr));
      for (usize i = 0; i < dyarr_len(arr); i++) {
        printf("[%zu]: %d\n", i, arr[i]);
      }
    }
  }

  defer(log_context_destroy()) {
  }

  return 0;
}

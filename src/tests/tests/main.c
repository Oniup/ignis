#include "tests/containers/test_containers.h"
#include "tests/framework.h"
#include <stdio.h>
#include <string.h>

typedef struct {
  const char* name;
  void (*func)();
} registered_test_t;

#define REGISTER_TEST(_test)                                                   \
  (registered_test_t) {                                                        \
    .name = #_test, .func = _test                                              \
  }

static registered_test_t tests[] = {
    // Containers
    REGISTER_TEST(test_dyarr),
    REGISTER_TEST(test_strbuf),
};

int main(i32 argc, char** argv) {
  usize test_count = sizeof(tests) / sizeof(tests[0]);

  for (usize i = 0; i < test_count; i++) {
    registered_test_t* test   = &tests[i];
    b8                 filter = false;

    for (usize j = 0; j < argc; j++) {
      if (strncmp(test->name, argv[j], strlen(test->name)) == 0) {
        filter = true;
      }
    }
    if (!filter) {
      printf("%s:\n", test->name);
      test->func();
    }
  }
  return 0;
}

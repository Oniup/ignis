#include "tests/containers/test_containers.h"
#include <ignis/core/log.h>
#include <ignis/core/memory.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    const char* name;
    void (*func)();
} registered_test_t;

#define REGISTER_TEST(_test)                                                   \
    (registered_test_t) {                                                      \
        .name = #_test, .func = _test                                          \
    }

static registered_test_t tests[] = {
    // Containers
    REGISTER_TEST(DynamicArray),
    REGISTER_TEST(StringView),
    REGISTER_TEST(StringBuffer),
};

void run_tests(i32 argc, char** argv) {
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
}

int main(i32 argc, char** argv) {
    log_context_init();
    log_context_add(log_terminal_output(LOG_DEFAULT_SEVERITY_FILTER |
                                        LOG_SEVERITY_INFO | LOG_SEVERITY_TRACE |
                                        LOG_SEVERITY_VERBOSE));
    log_context_add(log_file_output(LOG_DEFAULT_SEVERITY_FILTER));
    mem_system_init();

    run_tests(argc, argv);

    mem_system_destroy();
    log_context_destroy();
    return 0;
}

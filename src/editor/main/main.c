#include <cglm/cglm.h>
#include <ignis/containers/dyarr.h>
#include <ignis/core/log.h>
#include <ignis/core/memory.h>

int main(int argc, char** argv) {
    log_context_init();
    log_context_add(log_terminal_output(LOG_DEFAULT_SEVERITY_FILTER |
                                        LOG_SEVERITY_INFO | LOG_SEVERITY_TRACE |
                                        LOG_SEVERITY_VERBOSE));
    log_context_add(log_file_output(LOG_DEFAULT_SEVERITY_FILTER));
    mem_system_init();

    // Runtime ...

    mem_system_destroy();
    log_context_destroy();
    return 0;
}

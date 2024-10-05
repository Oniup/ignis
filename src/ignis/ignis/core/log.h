#ifndef IGNIS_CORE_LOG_H
#define IGNIS_CORE_LOG_H

#include "ignis/core/defines.h"

#define LOG_OUTPUT_MAX_COUNT 3
#define LOG_MAX_MSG_SIZE     512
#define LOG_DEFAULT_SEVERITY_FILTER                                            \
    LOG_SEVERITY_ERROR | LOG_SEVERITY_WARNING | LOG_SEVERITY_FATAL

#ifndef LOG_FILE_OUTPUT_PATH
#    define LOG_FILE_OUTPUT_PATH "./ignis.logs"
#endif

typedef enum {
    LOG_SEVERITY_VERBOSE = 1 << 0,
    LOG_SEVERITY_TRACE   = 1 << 1,
    LOG_SEVERITY_INFO    = 1 << 2,
    LOG_SEVERITY_WARNING = 1 << 3,
    LOG_SEVERITY_ERROR   = 1 << 4,
    LOG_SEVERITY_FATAL   = 1 << 5,
} log_severity_t;

const char* severity_to_cstr(log_severity_t lv);

typedef struct log_output log_output_t;
typedef void (*log_output_print_fn)(log_output_t* output, log_severity_t lv,
                                    const char* msg);
typedef void (*log_output_destroy_data_fn)(log_output_t* output);

struct log_output {
    i32                        severity_filter;
    void*                      data;
    log_output_destroy_data_fn destroy_data_fn;
    log_output_print_fn        print_fn;
};

log_output_t log_terminal_output(int severity_filter);
log_output_t log_file_output(int severity_filter);

typedef struct log_context {
    log_output_t outputs[LOG_OUTPUT_MAX_COUNT];
    size_t       output_count;
} log_context_t;

void log_context_init();
void log_context_destroy();
void log_context_add(log_output_t output);

void _intl_print_log(log_severity_t lv, int32_t line, const char* fp,
                     const char* fn, const char* fmt, ...);

#define _INTL_LOG(_lv, ...)                                                    \
    _intl_print_log(LOG_SEVERITY_##_lv, __LINE__, __FILE__, __FUNCTION__,      \
                    __VA_ARGS__)

#define VERBOSE(...) _INTL_LOG(VERBOSE, __VA_ARGS__)
#define TRACE(...)   _INTL_LOG(TRACE, __VA_ARGS__)
#define INFO(...)    _INTL_LOG(INFO, __VA_ARGS__)
#define ERROR(...)   _INTL_LOG(ERROR, __VA_ARGS__)
#define WARNING(...) _INTL_LOG(WARNING, __VA_ARGS__)
#define FATAL(...)                                                             \
    _INTL_LOG(FATAL, __VA_ARGS__);                                             \
    GENERATE_TRAP()

#ifndef NDEBUG
#    define DEBUG_VERBOSE(...) VERBOSE(__VA_ARGS__)
#    define DEBUG_TRACE(...)   TRACE(__VA_ARGS__)
#    define DEBUG_INFO(...)    INFO(__VA_ARGS__)
#    define DEBUG_ERROR(...)   ERROR(__VA_ARGS__)
#    define DEBUG_WARNING(...) WARNING(__VA_ARGS__)
#    define DEBUG_FATAL(...)   FATAL(__VA_ARGS__)

#    define ASSERT(_expr, ...)                                                 \
        if (!(_expr)) {                                                        \
            FATAL("expr: [" #_expr "] " __VA_ARGS__);                          \
        } else                                                                 \
            ((void)0)
#else
#    define DEBUG_VERBOSE(...)
#    define DEBUG_TRACE(...)
#    define DEBUG_INFO(...)
#    define DEBUG_ERROR(...)
#    define DEBUG_WARNING(...)
#    define DEBUG_FATAL(...)
#    define ASSERT(_expr, ...)
#endif

#endif

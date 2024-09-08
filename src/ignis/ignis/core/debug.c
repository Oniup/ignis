#include "ignis/core/debug.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define ASCII_RESET "\e[0m"

static log_context_t s_context;

static const char* severity_to_color(log_severity_t lv) {
  switch (lv) {
  case LOG_SEVERITY_VERBOSE:
    return "\e[0;30m";
  case LOG_SEVERITY_TRACE:
    return "\e[0;34m";
  case LOG_SEVERITY_INFO:
    return "\e[0;37m";
  case LOG_SEVERITY_ERROR:
    return "\e[0;31m";
  case LOG_SEVERITY_WARNING:
    return "\e[0;33m";
  case LOG_SEVERITY_FATAL:
    return "\e[2;31m";
  }
}

static void log_terminal_print_pfn(log_output_t* output, log_severity_t lv,
                                   const char* msg) {
  FILE* fp = NULL;
  if (lv > LOG_SEVERITY_INFO) {
    fp = stderr;
  } else {
    fp = stdout;
  }
  fprintf(fp, "%s%s%s", severity_to_color(lv), msg, ASCII_RESET);
  fflush(fp);
}

static void log_file_print_pfn(log_output_t* output, log_severity_t lv,
                               const char* msg) {
  FILE* fp = fopen(LOG_FILE_OUTPUT_PATH, "a");
  defer(fclose(fp)) {
    fprintf(fp, "%s", msg);
  }
}

const char* severity_to_cstr(log_severity_t lv) {
  switch (lv) {
  case LOG_SEVERITY_VERBOSE:
    return "Verbose";
  case LOG_SEVERITY_TRACE:
    return "Trace";
  case LOG_SEVERITY_INFO:
    return "Info";
  case LOG_SEVERITY_ERROR:
    return "Error";
  case LOG_SEVERITY_WARNING:
    return "Warning";
  case LOG_SEVERITY_FATAL:
    return "Fatal";
  }
}

log_output_t log_terminal_output(int severity_filter) {
  return (log_output_t) {
      .severity_filter = severity_filter,
      .data            = NULL,
      .destroy_data_fn = NULL,
      .print_fn        = log_terminal_print_pfn,
  };
}

log_output_t log_file_output(int severity_filter) {
  return (log_output_t) {
      .severity_filter = severity_filter,
      .data            = NULL,
      .destroy_data_fn = NULL,
      .print_fn        = log_file_print_pfn,
  };
}

void log_context_init() {
  s_context = (log_context_t) {
      .outputs      = {},
      .output_count = 0,
  };
}

void log_context_destroy() {
  for (size_t i = 0; i < s_context.output_count; i++) {
    log_output_t* output = &s_context.outputs[i];
    if (output->destroy_data_fn != NULL) {
      output->destroy_data_fn(output);
    }
  }
  s_context.output_count = 0;
}

void log_context_add(log_output_t output) {
  s_context.outputs[s_context.output_count++] = output;
}

void _intl_print_log(log_severity_t lv, int32_t line, const char* fp,
                     const char* fn, const char* fmt, ...) {
  char   msg[LOG_MAX_MSG_SIZE];
  size_t offset = 0;

  va_list args;
  va_start(args, fmt);
  defer(va_end(args)) {
    offset = snprintf(msg, LOG_MAX_MSG_SIZE, "[%s]: %s:%d '%s'\n",
                      severity_to_cstr(lv), fn, line, fp);
    offset += vsnprintf(msg + offset, LOG_MAX_MSG_SIZE - offset, fmt, args);
    offset += snprintf(msg + offset, LOG_MAX_MSG_SIZE - offset, "\n");
  }

  for (size_t i = 0; i < s_context.output_count; i++) {
    log_output_t* output = &s_context.outputs[i];
    if (output->severity_filter & lv) {
      output->print_fn(output, lv, msg);
    }
  }
}

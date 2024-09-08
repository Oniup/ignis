#ifndef IGNIS_TESTS_FRAGMEWORK_H
#define IGNIS_TESTS_FRAGMEWORK_H

#define RED   "\e[0;31m"
#define GREEN "\e[0;32m"
#define RESET "\e[0m"

#include <ignis/core/defines.h>
#include <stdio.h>

#define TEST(_name)                                                            \
  static b8 _name() {                                                          \
    const char* _intl_test_name = #_name;

#define END_TEST                                                               \
  return true;                                                                 \
  }

#define START_TEST_SUITE(_suite_name)                                          \
  void _suite_name() {                                                         \
    usize _intl_tests_ran    = 0;                                              \
    usize _intl_tests_passed = 0;

#define END_TEST_SUITE                                                         \
  printf("\ttotal: %zu ", _intl_tests_ran);                                    \
  if (_intl_tests_passed < _intl_tests_ran) {                                  \
    printf(GREEN "passed: %zu" RED " failed: %zu" RESET "\n",                  \
           _intl_tests_passed, _intl_tests_ran - _intl_tests_passed);          \
  } else {                                                                     \
    printf(GREEN "passed: %zu\n" RESET, _intl_tests_passed);                   \
  }                                                                            \
  }

#define RUN(_test)                                                             \
  ({                                                                           \
    _intl_tests_ran++;                                                         \
    if (_test()) {                                                             \
      printf("\t" GREEN "PASS\t" RESET "%s\n", #_test);                        \
      _intl_tests_passed++;                                                    \
    }                                                                          \
  })

#define _INTL_FAILED_PREFIX "\t" RED "FAILED\t" RESET "%s:%d " RED

#define EQUALS(_x, _y)                                                         \
  if ((_x) != (_y)) {                                                          \
    printf(_INTL_FAILED_PREFIX "[" #_x " != " #_y "]\n" RESET,                 \
           _intl_test_name, __LINE__);                                         \
    return false;                                                              \
  } else                                                                       \
    ((void)0)

#define IS_TRUE(_expr)                                                         \
  if (!(_expr)) {                                                              \
    printf(_INTL_FAILED_PREFIX RED "[" #_expr "] is false\n" RESET,            \
           _intl_test_name, __LINE__);                                         \
    return false;                                                              \
  } else                                                                       \
    ((void)0)

#endif

#include "tests/containers/test_containers.h"
#include "tests/framework.h"
#include <ignis/containers/strview.h>
#include <ignis/core/cstr.h>

TEST(CStringUtilities) {
  const char* str_a = "This is a test";
  EQUALS(CSTR_LEN(str_a), strlen(str_a));
  EQUALS(CSTR_CMP(str_a, str_a), strncmp(str_a, str_a, strlen(str_a)) == 0);
}
END_TEST

TEST(GetRef) {
  const char* str_a     = "C String with a const attribute";
  char*       str_b     = "C String with no const";
  usize       str_a_len = strlen(str_a);
  usize       str_b_len = strlen(str_b);

  strview_t strview = strview(str_a);
  EQUALS(strview.len, str_a_len);
  for (usize i = 0; i < str_a_len; i++) {
    EQUALS(strview.cstr[i], str_a[i]);
  }

  strview = strview(str_b);
  EQUALS(strview.len, str_b_len);
  for (usize i = 0; i < str_b_len; i++) {
    EQUALS(strview.cstr[i], str_b[i]);
  }

  strview_t strview_b = strview(&strview);
  EQUALS(strview.len, strview_b.len);
  for (usize i = 0; i < str_b_len; i++) {
    EQUALS(strview.cstr[i], strview_b.cstr[i]);
  }
}
END_TEST

TEST(Compare) {
  const char*     str_a = "C String with a const attribute";
  const strview_t str_b = strview("This is a test");
}
END_TEST

START_TEST_SUITE(TestStringView) {
  RUN(CStringUtilities);
  RUN(GetRef);
}
END_TEST_SUITE

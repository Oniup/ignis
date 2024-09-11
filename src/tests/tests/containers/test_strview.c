#include "tests/containers/test_containers.h"
#include "tests/framework.h"
#include <ignis/containers/strview.h>

static char*       s_cstr_no_const   = "C String with no const";
static const char* s_cstr_with_const = "C String with a const attribute";
static strview_t   s_strview         = {};

TEST(get_ref) {
  char*     val     = "Another text";
  strview_t strview = strview("This is a test");
}
END_TEST

START_TEST_SUITE(test_strview) {
  s_strview = strview("String View");

  RUN(get_ref);
}
END_TEST_SUITE

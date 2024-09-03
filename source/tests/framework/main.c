#include "framework/lcut.h"
#include <stdio.h>

int main(int argc, char** argv) {
  LCUT_TEST_BEGIN("Dynamic Array", NULL, NULL);

  LCUT_TEST_RUN();
  LCUT_TEST_REPORT();
  LCUT_TEST_END();

  LCUT_TEST_RESULT();
}

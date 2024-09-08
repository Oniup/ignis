#include "tests/containers/test_containers.h"
#include "tests/framework.h"
#include <ignis/containers/dyarr.h>

TEST(create_empty) {
  DYARR(i32) arr = dyarr_create_empty(i32);
  EQUALS(dyarr_len(arr), 0);
  EQUALS(dyarr_cap(arr), 0);
  EQUALS(dyarr_stride(arr), sizeof(i32));
  dyarr_destroy(arr);
}
END_TEST

TEST(create_size) {
  DYARR(i32) arr = dyarr_create_size(i32, 10, 20);
  defer(dyarr_destroy(arr)) {
    EQUALS(dyarr_len(arr), 10);
    EQUALS(dyarr_cap(arr), 20);
    EQUALS(dyarr_stride(arr), sizeof(i32));
    for (usize i = 0; i < dyarr_len(arr); i++) {
      EQUALS(arr[i], 0);
    }
  }
}
END_TEST

TEST(create_with_capacity) {
  DYARR(i32) arr = dyarr_create(i32, 20);
  EQUALS(dyarr_len(arr), 0);
  EQUALS(dyarr_cap(arr), 20);
  EQUALS(dyarr_stride(arr), sizeof(i32));
  dyarr_destroy(arr);
}
END_TEST

TEST(push_without_increasing_capacity) {
  DYARR(usize) arr = dyarr_create(usize, 20);
  defer(dyarr_destroy(arr)) {
    usize cap = dyarr_cap(arr);

    EQUALS(dyarr_len(arr), 0);
    EQUALS(dyarr_stride(arr), sizeof(usize));
    EQUALS(cap, 20);

    // Push elements up to the capacity increasing the length of the array
    for (usize i = 0; i < cap; i++) {
      dyarr_push(arr, i);
      EQUALS(dyarr_len(arr), i + 1);
      EQUALS(cap, 20);
    }

    // Compare correct length and each element at each position is correct
    EQUALS(dyarr_len(arr), dyarr_cap(arr));
    for (usize i = 0; i < dyarr_len(arr); i++) {
      EQUALS(arr[i], i);
    }
  }
}
END_TEST

TEST(push_and_increase_capacity) {
  DYARR(i32) arr = dyarr_create(i32, 5);
  defer(dyarr_destroy(arr)) {
    EQUALS(dyarr_len(arr), 0);
    EQUALS(dyarr_cap(arr), 5);

    // Push elements
    for (i32 i = 0; i < 10; i++) {
      dyarr_push(arr, i);
      EQUALS(dyarr_len(arr), i + 1);

      if (i + 1 < 5) {
        IS_TRUE(dyarr_len(arr) < dyarr_cap(arr));
      } else {
        EQUALS(dyarr_cap(arr), dyarr_len(arr));
      }
    }

    // Compare correct length and each element at each position is correct
    EQUALS(dyarr_len(arr), 10);
    for (usize i = 0; i < dyarr_len(arr); i++) {
      EQUALS(arr[i], i);
    }
  }
}
END_TEST

TEST(resize) {
  DYARR(i32) arr = dyarr_create_empty(i32);
  defer(dyarr_destroy(arr)) {
    dyarr_resize(arr, 10);
    EQUALS(dyarr_len(arr), 10);
    EQUALS(dyarr_cap(arr), 10);
    EQUALS(dyarr_stride(arr), sizeof(i32));
    for (usize i = 0; i < dyarr_len(arr); i++) {
      EQUALS(arr[i], 0);
    }
  }
}
END_TEST

TEST(resize_capacity) {
  DYARR(i32) arr = dyarr_create_empty(i32);
  defer(dyarr_destroy(arr)) {
    dyarr_realloc(arr, 10);
    EQUALS(dyarr_len(arr), 0);
    EQUALS(dyarr_cap(arr), 10);
    EQUALS(dyarr_stride(arr), sizeof(i32));
  }
}
END_TEST

TEST(push_front) {
  DYARR(i32) arr = dyarr_create_empty(i32);
  defer(dyarr_destroy(arr)) {
    for (usize i = 0; i < 10; i++) {
      dyarr_push_front(arr, i);
    }
    EQUALS(dyarr_len(arr), 10);
    EQUALS(dyarr_cap(arr), 10);
    i32 j = 0;
    for (i32 i = dyarr_len(arr) - 1; i > 0; i--) {
      EQUALS(arr[i], j);
      j++;
    }
  }
}
END_TEST

TEST(insert) {
  DYARR(i32) arr = dyarr_create_empty(i32);
  defer(dyarr_destroy(arr)) {
    i32 val = 10;
    dyarr_push(arr, 0);
    dyarr_push(arr, 1);
    dyarr_insert_val(arr, 1, &val);
    EQUALS(dyarr_len(arr), 3);
    EQUALS(dyarr_cap(arr), 3);
    EQUALS(arr[0], 0);
    EQUALS(arr[1], val);
    EQUALS(arr[2], 1);
  }
}
END_TEST

TEST(pop) {
  DYARR(i32) arr = dyarr_create_empty(i32);
  defer(dyarr_destroy(arr)) {
    i32 val = 10;
    dyarr_push_val(arr, &val);
    i32 popped;
    dyarr_pop(arr, &popped);
    EQUALS(dyarr_len(arr), 0);
    EQUALS(dyarr_cap(arr), 1);
    EQUALS(popped, val);
  }
}
END_TEST

TEST(pop_at) {
  DYARR(i32) arr = dyarr_create_empty(i32);
  defer(dyarr_destroy(arr)) {
    i32 val = 10;
    dyarr_push(arr, 0);
    dyarr_push_val(arr, &val);
    dyarr_push(arr, 2);
    i32 popped;
    dyarr_pop_at(arr, 1, &popped);
    EQUALS(dyarr_len(arr), 2);
    EQUALS(dyarr_cap(arr), 3);
    EQUALS(popped, val);
    EQUALS(arr[0], 0);
    EQUALS(arr[1], 2);
  }
}
END_TEST

START_TEST_SUITE(test_dyarr) {
  RUN(create_empty);
  RUN(create_size);
  RUN(create_with_capacity);

  RUN(resize);
  RUN(resize_capacity);

  RUN(push_without_increasing_capacity);
  RUN(push_and_increase_capacity);
  RUN(push_front);
  RUN(insert);
  RUN(pop);
  RUN(pop_at);
}
END_TEST_SUITE

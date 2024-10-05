#include "tests/containers/test_containers.h"
#include "tests/framework.h"
#include <ignis/containers/strview.h>
#include <ignis/core/cstr.h>

TEST(CStringUtilities) {
    const char* cstr = "This is a test";
    EQUALS(CSTR_LEN(cstr), strlen(cstr));
    EQUALS(CSTR_CMP(cstr, cstr), strncmp(cstr, cstr, strlen(cstr)) == 0);
}
END_TEST

TEST(GetRef) {
    const char* str_a     = "C String with a const attribute";
    char*       str_b     = "C String with no const";
    usize       str_a_len = strlen(str_a);
    usize       str_b_len = strlen(str_b);

    strview_t view = strview(str_a);
    EQUALS(view.len, str_a_len);
    for (usize i = 0; i < str_a_len; i++) {
        EQUALS(view.cstr[i], str_a[i]);
    }

    view = strview(str_b);
    EQUALS(view.len, str_b_len);
    for (usize i = 0; i < str_b_len; i++) {
        EQUALS(view.cstr[i], str_b[i]);
    }

    strview_t view_b = strview(&view);
    EQUALS(view.len, view_b.len);
    for (usize i = 0; i < str_b_len; i++) {
        EQUALS(view.cstr[i], view_b.cstr[i]);
    }
}
END_TEST

TEST(Compare) {
    const char*     str_a = "C String with a const attribute";
    const strview_t str_b = strview("This is a test");

    EQUALS(strview_cmp(&str_b, str_a), false);
    EQUALS(strview_cmp(&str_b, &str_b), true);

    strview_t str_c = strview("This is a test");
    EQUALS(strview_cmp(&str_b, &str_c), true);
}
END_TEST

TEST(Slice) {
    const char* cstr     = "This is a test string";
    u64         cstr_len = strlen(cstr);

    strview_t view   = strview(cstr);
    strview_t sliced = str_slice(&view, 5, 10);
    EQUALS(sliced.len, 5);
    for (u64 i = 0; i < 5; i++) {
        EQUALS(sliced.cstr[i], cstr[5 + i]);
    }

    sliced = str_slice(&view, 0, 5);
    EQUALS(sliced.len, 5);
    for (u64 i = 0; i < 5; i++) {
        EQUALS(sliced.cstr[i], cstr[i]);
    }

    sliced = str_slice(&view, 5, NO_POS);
    EQUALS(sliced.len, cstr_len - 5);
    for (u64 i = 5; i < cstr_len - 5; i++) {
        EQUALS(sliced.cstr[i], cstr[5 + i]);
    }
}
END_TEST

TEST(Find) {
    const char* cstr = "This is a test string";

    strview_t view  = strview(cstr);
    u64       index = strview_find(&view, "test");
    EQUALS(index, 10);

    index = strview_find(&view, "not found");
    EQUALS(index, NO_POS);

    strview_t search = strview("test");
    index            = strview_find(&view, &search);
    EQUALS(index, 10);

    index = strview_find(&view, "not found");
    EQUALS(index, NO_POS);
}
END_TEST

TEST(ReverseFind) {
    const char* cstr = "This is a test string";

    strview_t view  = strview(cstr);
    u64       index = strview_rfind(&view, "test");
    EQUALS(index, 10);

    index = strview_rfind(&view, "not found");
    EQUALS(index, NO_POS);

    strview_t search = strview("test");
    index            = strview_rfind(&view, &search);
    EQUALS(index, 10);

    index = strview_rfind(&view, "not found");
    EQUALS(index, NO_POS);
}
END_TEST

TEST(FindRange) {
    const char* cstr   = "This test is a test string test";
    DYARR(u64) indices = NULL;

    strview_t view = strview(cstr);
    indices        = strview_find_range(&view, "test");
    defer(dyarr_destroy(indices)) {
        EQUALS(dyarr_len(indices), 3);
        EQUALS(indices[0], 5);
        EQUALS(indices[1], 15);
        EQUALS(indices[2], 27);
    }

    strview_t search = strview("not found");
    indices          = strview_find_range(&view, &search);
    defer(dyarr_destroy(indices)) {
        EQUALS(dyarr_len(indices), 0);
    }
}
END_TEST

START_TEST_SUITE(StringView) {
    RUN(CStringUtilities);
    RUN(GetRef);
    RUN(Compare);
    RUN(Slice);
    RUN(Find);
    RUN(ReverseFind);
    RUN(FindRange);
}
END_TEST_SUITE

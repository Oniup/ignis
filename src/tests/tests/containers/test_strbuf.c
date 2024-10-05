#include "tests/containers/test_containers.h"
#include "tests/framework.h"
#include <ignis/containers/strbuf.h>
#include <ignis/containers/strview.h>

static const char* common_cstr = "This is a test";

TEST(Create) {
    strbuf_t buf = strbuf_create(common_cstr);
    defer(strbuf_destroy(&buf)) {
        EQUALS(buf.len, strlen(common_cstr));
        EQUALS(buf.cap, strlen(common_cstr) + 1);
        EQUALS(buf.cstr[buf.len], '\0');
        for (usize i = 0; i < buf.len; i++) {
            EQUALS(buf.cstr[i], common_cstr[i]);
        }
    }

    strbuf_t str = strbuf_create("Copied from another String Buffer");
    buf          = strbuf_create(&str);
    defer(strbuf_destroy(&buf), strbuf_destroy(&str)) {
        EQUALS(buf.len, str.len);
        EQUALS(buf.cap, str.cap);
        EQUALS(buf.cstr[buf.len], '\0');
        for (usize i = 0; i < buf.len; i++) {
            EQUALS(buf.cstr[i], str.cstr[i]);
        }
    }

    strview_t view = strview("Copied from a strview");
    buf            = strbuf_create(&view);
    defer(strbuf_destroy(&buf)) {
        EQUALS(buf.len, view.len);
        EQUALS(buf.cap, view.len + 1);
        EQUALS(buf.cstr[buf.len], '\0');
        for (usize i = 0; i < buf.len; i++) {
            EQUALS(buf.cstr[i], view.cstr[i]);
        }
    }
}
END_TEST

TEST(Move) {
    strbuf_t str   = strbuf_create(common_cstr);
    strbuf_t moved = EMPTY_STRBUF;
    defer(strbuf_destroy(&moved)) {
        strbuf_move(&moved, &str);

        EQUALS(moved.len, strlen(common_cstr));
        EQUALS(moved.cap, strlen(common_cstr) + 1);
        EQUALS(strbuf_cmp(&moved, common_cstr), true);

        EQUALS(str.len, 0);
        EQUALS(str.cap, 0);
        EQUALS(str.cstr, NULL);
    }
}
END_TEST

TEST(ResizeAndRealloc) {
    strbuf_t buf = strbuf_create(common_cstr);
    defer(strbuf_destroy(&buf)) {
        EQUALS(buf.len, strlen(common_cstr));
        EQUALS(buf.cap, strlen(common_cstr) + 1);

        // Calls strbuf_realloc to correct for larger buffer size
        strbuf_resize(&buf, 20);
        EQUALS(buf.len, 20);
        EQUALS(buf.cap, 21);

        strbuf_resize(&buf, 10);
        EQUALS(buf.len, 10);
        EQUALS(buf.cap, 21);
    }
}
END_TEST

TEST(Clear) {
    strbuf_t buf = strbuf_create(common_cstr);
    defer(strbuf_destroy(&buf)) {
        EQUALS(buf.len, strlen(common_cstr));
        EQUALS(buf.cap, strlen(common_cstr) + 1);

        strbuf_clear(&buf);
        EQUALS(buf.len, 0);
        EQUALS(buf.cap, strlen(common_cstr) + 1);
    }
}
END_TEST

TEST(Compare) {
    const char* cstr = "This is a test";
    strview_t   view = strview("This is a test");

    strbuf_t buf = strbuf_create("This is a test from str buf");
    defer(strbuf_destroy(&buf)) {
        EQUALS(strbuf_cmp(&buf, cstr), false);
        EQUALS(strbuf_cmp(&buf, &view), false);
        EQUALS(strbuf_cmp(&buf, &buf), true);

        strbuf_t buf2 = strbuf_create("This is a test");
        defer(strbuf_destroy(&buf2)) {
            EQUALS(strbuf_cmp(&buf2, cstr), true);
            EQUALS(strbuf_cmp(&buf2, &view), true);
            EQUALS(strbuf_cmp(&buf2, &buf), false);
        }
    }
}
END_TEST

TEST(Copy) {
    const char* cstr = "Not the common C String";
    strbuf_t    buf1 = strbuf_create(common_cstr);
    strbuf_t    buf2 = strbuf_create(cstr);
    defer(strbuf_destroy(&buf1), strbuf_destroy(&buf2)) {
        EQUALS(strbuf_cmp(&buf1, common_cstr), true);
        EQUALS(strbuf_cmp(&buf2, cstr), true);
        EQUALS(strbuf_cmp(&buf1, &buf2), false);

        strbuf_cpy(&buf2, &buf1);
        EQUALS(strbuf_cmp(&buf2, common_cstr), true);
        EQUALS(strbuf_cmp(&buf2, cstr), false);
        EQUALS(strbuf_cmp(&buf2, &buf1), true);

        strview_t view = strview(cstr);
        strbuf_cpy(&buf2, &view);
        EQUALS(strbuf_cmp(&buf2, common_cstr), false);
        EQUALS(strbuf_cmp(&buf2, cstr), true);
        EQUALS(strbuf_cmp(&buf2, &buf1), false);
    }
}
END_TEST

TEST(Append) {
    strbuf_t buf = strbuf_create("Creating a string");
    defer(strbuf_destroy(&buf)) {
        EQUALS(strbuf_cmp(&buf, "Creating a string"), true);

        strbuf_append(&buf, ". Appended string");
        EQUALS(strbuf_cmp(&buf, "Creating a string. Appended string"), true);

        strview_t view = strview(". Second, appendage");
        strbuf_append(&buf, &view);
        EQUALS(
            strbuf_cmp(&buf,
                       "Creating a string. Appended string. Second, appendage"),
            true);
    }
}
END_TEST

TEST(Find) {
    strbuf_t buf = strbuf_create("This is a test string for finding");
    defer(strbuf_destroy(&buf)) {
        u64 index = strbuf_find(&buf, "test");
        EQUALS(index, 10);

        index = strbuf_find(&buf, "notfound");
        EQUALS(index, -1);

        index = strbuf_find(&buf, "This");
        EQUALS(index, 0);

        index = strbuf_find(&buf, "finding");
        EQUALS(index, 26);
    }
}
END_TEST

TEST(ReverseFind) {
    strbuf_t buf = strbuf_create("This is a test string for finding");
    defer(strbuf_destroy(&buf)) {
        u64 index = strbuf_rfind(&buf, "is");
        EQUALS(index, 5);

        index = strbuf_rfind(&buf, "notfound");
        EQUALS(index, NO_POS);

        index = strbuf_rfind(&buf, "This");
        EQUALS(index, 0);

        index = strbuf_rfind(&buf, "finding");
        EQUALS(index, 26);
    }
}
END_TEST

TEST(FindRange) {
    const char* cstr   = "This test is a test string test";
    DYARR(u64) indices = NULL;

    strbuf_t buf = strbuf_create(cstr);
    defer(strbuf_destroy(&buf)) {
        indices = strbuf_find_range(&buf, "test");
        defer(dyarr_destroy(indices)) {
            EQUALS(dyarr_len(indices), 3);
            EQUALS(indices[0], 5);
            EQUALS(indices[1], 15);
            EQUALS(indices[2], 27);
        }

        strview_t search = strview("not found");
        indices          = strbuf_find_range(&buf, &search);
        defer(dyarr_destroy(indices)) {
            EQUALS(dyarr_len(indices), 0);
        }
    }
}
END_TEST

TEST(Concatenate) {
    strbuf_t buf =
        strbuf_join(strview("This "), strview("is "), strview("a test"));
    defer(strbuf_destroy(&buf)) {
        EQUALS(strbuf_cmp(&buf, "This is a test"), true);
    }
}
END_TEST

TEST(ToUpper) {
    strbuf_t buf = strbuf_create(common_cstr);
    defer(strbuf_destroy(&buf)) {
        strbuf_to_upper(&buf);
        EQUALS(strbuf_cmp(&buf, "THIS IS A TEST"), true);
    }
}
END_TEST

TEST(ToLower) {
    strbuf_t buf = strbuf_create("THIS IS A TEST");
    defer(strbuf_destroy(&buf)) {
        strbuf_to_lower(&buf);
        EQUALS(strbuf_cmp(&buf, "this is a test"), true);
    }
}
END_TEST

TEST(ToPascal) {
    strbuf_t buf = strbuf_create(common_cstr);
    defer(strbuf_destroy(&buf)) {
        strbuf_to_pascal(&buf, false);
        EQUALS(strbuf_cmp(&buf, "This Is A Test"), true);

        strbuf_cpy(&buf, common_cstr);
        strbuf_to_pascal(&buf, true);
        EQUALS(strbuf_cmp(&buf, "ThisIsATest"), true);

        strbuf_cpy(&buf, "this-is_a tEsT\n");
        strbuf_to_pascal(&buf, true);
        EQUALS(strbuf_cmp(&buf, "ThisIsATest"), true);

        strbuf_cpy(&buf, "this-is_a tEsT\n");
        strbuf_to_pascal(&buf, false);
        EQUALS(strbuf_cmp(&buf, "This-Is_A Test\n"), true);
    }
}
END_TEST

TEST(ToCamel) {
    strbuf_t buf = strbuf_create(common_cstr);
    defer(strbuf_destroy(&buf)) {
        strbuf_to_camel(&buf, false);
        EQUALS(strbuf_cmp(&buf, "this Is A Test"), true);

        strbuf_cpy(&buf, common_cstr);
        strbuf_to_camel(&buf, true);
        EQUALS(strbuf_cmp(&buf, "thisIsATest"), true);

        strbuf_cpy(&buf, "This-is_a tEsT\n");
        strbuf_to_camel(&buf, true);
        EQUALS(strbuf_cmp(&buf, "thisIsATest"), true);

        strbuf_cpy(&buf, "this-is_a tEsT\n");
        strbuf_to_camel(&buf, false);
        EQUALS(strbuf_cmp(&buf, "this-Is_A Test\n"), true);
    }
}
END_TEST

TEST(ToNormalText) {
    strbuf_t buf = strbuf_create("this_is_a_test");
    defer(strbuf_destroy(&buf)) {
        strbuf_to_normal_text(&buf, true);
        EQUALS(strbuf_cmp(&buf, "this is a test"), true);
    }

    buf = strbuf_create("ThisIsATest");
    defer(strbuf_destroy(&buf)) {
        strbuf_to_normal_text(&buf, true);
        EQUALS(strbuf_cmp(&buf, "This is a test"), true);

        strbuf_cpy(&buf, "ThisIsATest");
        strbuf_to_normal_text(&buf, false);
        EQUALS(strbuf_cmp(&buf, "This Is A Test"), true);
    }

    buf = strbuf_create("ThisIsATest_with_snake_case");
    defer(strbuf_destroy(&buf)) {
        strbuf_to_normal_text(&buf, true);
        EQUALS(strbuf_cmp(&buf, "This is a test with snake case"), true);

        strbuf_cpy(&buf, "ThisIsATest_with_snake_case");
        strbuf_to_normal_text(&buf, false);
        EQUALS(strbuf_cmp(&buf, "This Is A Test with snake case"), true);
    }
}
END_TEST

START_TEST_SUITE(StringBuffer) {
    RUN(Create);
    RUN(Move);
    RUN(ResizeAndRealloc);
    RUN(Clear);
    RUN(Compare);
    RUN(Copy);
    RUN(Append);
    RUN(Find);
    RUN(ReverseFind);
    RUN(FindRange);
    RUN(Concatenate);
    RUN(ToUpper);
    RUN(ToLower);
    RUN(ToPascal);
    RUN(ToCamel);
    RUN(ToNormalText);
}
END_TEST_SUITE

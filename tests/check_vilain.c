#include <stdlib.h>
#include <check.h>
#include "../include/buffer.h"
#include "../include/cursor.h"
#include "../include/io_text.h"
#include "../include/settings.h"
#include "../include/ui.h"

BUFFER *buff = NULL;
char *str = NULL;

void buffer_setup(void)
{
    buff = new_buffer();
}

void buffer_teardown(void)
{
    if (str != NULL) {
        free(str);
    }

    free_buffer(buff);
}

START_TEST(test_insert_empty_line)
{
    ck_assert_int_eq(insert_line(buff, "", 0), 0);

    str = get_line(buff, 0);
    ck_assert_ptr_ne(str, NULL);
    ck_assert_str_eq(str, "\n");
}

END_TEST START_TEST(test_insert_character)
{
    ck_assert_int_eq(insert_line(buff, "", 0), 0);
    ck_assert_int_eq(insert_char(buff, 'a', 0, 0), 0);
    ck_assert_int_eq(insert_char(buff, 'b', 0, 1), 0);
    ck_assert_int_eq(insert_char(buff, 'z', 0, 0), 0);

    str = get_line(buff, 0);
    ck_assert_ptr_ne(str, NULL);
    ck_assert_str_eq(str, "zab\n");
}

END_TEST START_TEST(test_insert_line)
{
    ck_assert_int_eq(insert_line(buff, "hello", 0), 0);
    ck_assert_int_eq(insert_line(buff, "world", 1), 0);
    ck_assert_int_eq(insert_line(buff, "pre", 0), 0);

    str = get_text(buff, 0, 0, 2, strlen("world") - 1);
    ck_assert_ptr_ne(str, NULL);
    ck_assert_str_eq(str, "pre\nhello\nworld\n");
}

END_TEST START_TEST(test_insert_text)
{
    ck_assert_int_eq(insert_text(buff, "lo\nwor", 0, 0), 0);
    ck_assert_int_eq(insert_text(buff, "ld", 1, strlen("wor")), 0);
    ck_assert_int_eq(insert_text(buff, "hel", 0, 0), 0);

    str = get_text(buff, 0, 0, 1, strlen("world") - 1);
    ck_assert_ptr_ne(str, NULL);
    ck_assert_str_eq(str, "hello\nworld\n");
}

END_TEST START_TEST(test_delete_character)
{
    ck_assert_int_eq(insert_line(buff, "hello world", 0), 0);
    ck_assert_int_eq(delete_char(buff, 0, 0), 0);
    ck_assert_int_eq(delete_char(buff, 0, get_line_length(buff, 0) - 1), 0);
    ck_assert_int_eq(delete_char(buff, 0, strlen("ello")), 0);

    str = get_line(buff, 0);
    ck_assert_ptr_ne(str, NULL);
    ck_assert_str_eq(str, "elloworl\n");
}

END_TEST START_TEST(test_delete_line)
{
    ck_assert_int_eq(insert_text(buff, "hello\nworld\n!\nbye", 0, 0), 0);
    ck_assert_int_eq(delete_line(buff, 0), 0);
    ck_assert_int_eq(delete_line(buff, 1), 0);
    ck_assert_int_eq(delete_line(buff, 1), 0);

    ck_assert_int_eq(get_line_count(buff), 1);
    str = get_line(buff, 0);
    ck_assert_ptr_ne(str, NULL);
    ck_assert_str_eq(str, "world\n");
}

END_TEST START_TEST(test_delete_text)
{
    ck_assert_int_eq(insert_text(buff, "hello world\n!\nor good bye", 0, 0), 0);
    ck_assert_int_eq(delete_text(buff, 0, strlen("hello worl"), 2, 0), 0);
    ck_assert_int_eq(delete_text(buff, 0, 0, 0, 1), 0);
    ck_assert_int_eq(delete_text
                     (buff, 0, strlen("llo worlr good b"), 0,
                      strlen("llo worlr good by")), 0);

    ck_assert_int_eq(get_line_count(buff), 1);
    str = get_line(buff, 0);
    ck_assert_ptr_ne(str, NULL);
    ck_assert_str_eq(str, "llo worlr good b\n");
}

END_TEST START_TEST(test_override_char)
{
    ck_assert_int_eq(insert_line(buff, "hello world!", 0), 0);
    ck_assert_int_eq(override_char(buff, '_', 0, strlen("hello")), 0);

    str = get_line(buff, 0);
    ck_assert_ptr_ne(str, NULL);
    ck_assert_str_eq(str, "hello_world!\n");
}

END_TEST START_TEST(test_override_line)
{
    ck_assert_int_eq(insert_line(buff, "hello world!", 0), 0);
    ck_assert_int_eq(override_line(buff, "This is completely different", 0), 0);

    ck_assert_int_eq(get_line_count(buff), 1);
    str = get_line(buff, 0);
    ck_assert_ptr_ne(str, NULL);
    ck_assert_str_eq(str, "This is completely different\n");
}

END_TEST START_TEST(test_override_text)
{
    ck_assert_int_eq(insert_text(buff, "helloERRORworld!", 0, 0), 0);
    ck_assert_int_eq(override_text
                     (buff, "\n", 0, strlen("hello"), 0, strlen("helloERRO")),
                     0);

    str = get_text(buff, 0, 0, 1, strlen("world"));
    ck_assert_ptr_ne(str, NULL);
    ck_assert_str_eq(str, "hello\nworld!\n");
}

END_TEST Suite *buffer_suite(void)
{
    TCase *tc_buffer;
    Suite *s;

    tc_buffer = tcase_create("Core");
    tcase_add_checked_fixture(tc_buffer, buffer_setup, buffer_teardown);
    tcase_add_test(tc_buffer, test_insert_empty_line);
    tcase_add_test(tc_buffer, test_insert_character);
    tcase_add_test(tc_buffer, test_insert_line);
    tcase_add_test(tc_buffer, test_insert_text);
    tcase_add_test(tc_buffer, test_delete_character);
    tcase_add_test(tc_buffer, test_delete_line);
    tcase_add_test(tc_buffer, test_delete_text);
    tcase_add_test(tc_buffer, test_override_char);
    tcase_add_test(tc_buffer, test_override_line);
    tcase_add_test(tc_buffer, test_override_text);

    s = suite_create("Buffer");
    suite_add_tcase(s, tc_buffer);

    return s;
}

int main(void)
{
    int failures;
    Suite *s;
    SRunner *sr;

    s = buffer_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    failures = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

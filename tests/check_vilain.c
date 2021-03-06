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

END_TEST START_TEST(test_autosplit_line)
{
    ck_assert_int_eq(insert_line(buff, "hello\nworld\n!", 0), 0);
    ck_assert_int_eq(autosplit_line(buff, 0), 0);

    ck_assert_int_eq(get_line_count(buff), 3);
    str = get_text(buff, 0, 0, 2, 0);
    ck_assert_ptr_ne(str, NULL);
    ck_assert_str_eq(str, "hello\nworld\n!\n");
}

END_TEST START_TEST(test_split_line_at)
{
    ck_assert_int_eq(insert_line(buff, "hello world!", 0), 0);
    ck_assert_int_eq(split_line_at(buff, 0, strlen("hello")), 0);

    ck_assert_int_eq(get_line_count(buff), 2);
    str = get_text(buff, 0, 0, 1, strlen(" world"));
    ck_assert_ptr_ne(str, NULL);
    ck_assert_str_eq(str, "hello\n world!\n");
}

END_TEST START_TEST(test_join_lines)
{
    ck_assert_int_eq(insert_text(buff, "hello\nworld!", 0, 0), 0);
    ck_assert_int_eq(join_lines(buff, 0, 1, 0), 0);

    ck_assert_int_eq(get_line_count(buff), 1);
    str = get_line(buff, 0);
    ck_assert_ptr_ne(str, NULL);
    ck_assert_str_eq(str, "helloworld!\n");

    free(str);
    str = NULL;

    ck_assert_int_eq(split_line_at(buff, 0, strlen("hello")), 0);
    ck_assert_int_eq(join_lines(buff, 0, 1, 1), 0);

    ck_assert_int_eq(get_line_count(buff), 1);
    str = get_line(buff, 0);
    ck_assert_ptr_ne(str, NULL);
    ck_assert_str_eq(str, "hello world!\n");
}

END_TEST START_TEST(test_out_of_bounds_columns)
{
    ck_assert_int_eq(insert_line(buff, "hello world!", 0), 0);

    ck_assert_int_eq(get_char(buff, 0, -1), 0);
    ck_assert_int_eq(get_char(buff, 0, strlen("hello world!")), 0);

    ck_assert_ptr_eq(get_text(buff, 0, -1, 0, strlen("hello world")), NULL);
    ck_assert_ptr_eq(get_text(buff, 0, 0, 0, strlen("hello world!")), NULL);

    ck_assert_int_eq(insert_char(buff, 'A', 0, -1), -1);
    ck_assert_int_eq(insert_char(buff, 'A', 0, strlen("hello world!") + 1), -1);

    ck_assert_int_eq(insert_text(buff, "text", 0, -1), -1);
    ck_assert_int_eq(insert_text(buff, "text", 0, strlen("hello world!") + 1),
                     -1);

    ck_assert_int_eq(delete_char(buff, 0, -1), -1);
    ck_assert_int_eq(delete_char(buff, 0, strlen("hello world!")), -1);

    ck_assert_int_eq(delete_text(buff, 0, -1, 0, strlen("hello world")), -1);
    ck_assert_int_eq(delete_text(buff, 0, 0, 0, strlen("hello world!")), -1);

    ck_assert_int_eq(override_char(buff, 'A', 0, -1), -1);
    ck_assert_int_eq(override_char(buff, 'A', 0, strlen("hello world!")), -1);

    ck_assert_int_eq(override_text
                     (buff, "text", 0, -1, 0, strlen("hello world")), -1);
    ck_assert_int_eq(override_text
                     (buff, "text", 0, 0, 0, strlen("hello world!")), -1);

    ck_assert_int_eq(split_line_at(buff, 0, -1), -1);
    ck_assert_int_eq(split_line_at(buff, 0, strlen("hello world!") + 1), -1);
}

END_TEST START_TEST(test_out_of_bounds_lines)
{
    ck_assert_int_eq(insert_line(buff, "hello world!", 0), 0);

    ck_assert_int_eq(get_char(buff, -1, 0), 0);
    ck_assert_int_eq(get_char(buff, 1, 0), 0);

    ck_assert_ptr_eq(get_line(buff, -1), NULL);
    ck_assert_ptr_eq(get_line(buff, 1), NULL);

    ck_assert_ptr_eq(get_text(buff, -1, 0, 0, strlen("hello world")), NULL);
    ck_assert_ptr_eq(get_text(buff, 0, 0, 1, strlen("hello world")), NULL);

    ck_assert_int_eq(insert_char(buff, 'A', -1, 0), -1);
    ck_assert_int_eq(insert_char(buff, 'A', 1, 0), -1);

    ck_assert_int_eq(insert_line(buff, "line", -1), -1);
    ck_assert_int_eq(insert_line(buff, "line", 2), -1);

    ck_assert_int_eq(insert_text(buff, "text", -1, 0), -1);
    ck_assert_int_eq(insert_text(buff, "text", 2, 0), -1);

    ck_assert_int_eq(delete_char(buff, -1, 0), -1);
    ck_assert_int_eq(delete_char(buff, 1, 0), -1);

    ck_assert_int_eq(delete_line(buff, -1), -1);
    ck_assert_int_eq(delete_line(buff, 1), -1);

    ck_assert_int_eq(delete_text(buff, -1, 0, 0, strlen("hello world")), -1);
    ck_assert_int_eq(delete_text(buff, 0, 0, 1, strlen("hello world")), -1);

    ck_assert_int_eq(override_char(buff, 'A', -1, 0), -1);
    ck_assert_int_eq(override_char(buff, 'A', 1, 0), -1);

    ck_assert_int_eq(override_line(buff, "line", -1), -1);
    ck_assert_int_eq(override_line(buff, "line", 1), -1);

    ck_assert_int_eq(override_text
                     (buff, "text", -1, 0, 0, strlen("hello world")), -1);
    ck_assert_int_eq(override_text
                     (buff, "text", 0, 0, 1, strlen("hello world")), -1);

    ck_assert_int_eq(autosplit_line(buff, -1), -1);
    ck_assert_int_eq(autosplit_line(buff, 1), -1);

    ck_assert_int_eq(split_line_at(buff, -1, 0), -1);
    ck_assert_int_eq(split_line_at(buff, 1, 0), -1);

    ck_assert_int_eq(join_lines(buff, -1, 0, 0), -1);
    ck_assert_int_eq(join_lines(buff, 0, 1, 0), -1);

    ck_assert_int_eq(get_line_length(buff, -1), -1);
    ck_assert_int_eq(get_line_length(buff, 1), -1);
}

END_TEST START_TEST(test_lines_growth)
{
    ck_assert_int_eq(insert_line
                     (buff,
                      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
                      0), 0);

    ck_assert_int_eq(insert_text(buff, "XYZ", 0, 0), 0);

    str = get_line(buff, 0);
    ck_assert_ptr_ne(str, NULL);
    ck_assert_str_eq(str,
                     "XYZAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");

    free(str);
    str = NULL;

    ck_assert_int_eq(override_line
                     (buff,
                      "XYZAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
                      0), 0);

    str = get_line(buff, 0);
    ck_assert_ptr_ne(str, NULL);
    ck_assert_str_eq(str,
                     "XYZAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
}

END_TEST START_TEST(test_buffer_growth)
{
    for (int i = 0; i < 100; i++) {
        ck_assert_int_eq(insert_line(buff, "Just a line", i), 0);
    }

    ck_assert_int_eq(insert_line(buff, "An additional line!", 100), 0);

    ck_assert_int_eq(get_line_count(buff), 101);

    str = get_line(buff, 99);
    ck_assert_ptr_ne(str, NULL);
    ck_assert_str_eq(str, "Just a line\n");

    free(str);
    str = NULL;

    str = get_line(buff, 100);
    ck_assert_ptr_ne(str, NULL);
    ck_assert_str_eq(str, "An additional line!\n");
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
    tcase_add_test(tc_buffer, test_autosplit_line);
    tcase_add_test(tc_buffer, test_split_line_at);
    tcase_add_test(tc_buffer, test_join_lines);
    tcase_add_test(tc_buffer, test_out_of_bounds_columns);
    tcase_add_test(tc_buffer, test_out_of_bounds_lines);
    tcase_add_test(tc_buffer, test_lines_growth);
    tcase_add_test(tc_buffer, test_buffer_growth);

    s = suite_create("Buffer");
    suite_add_tcase(s, tc_buffer);

    return s;
}

// IO_TEXT MODULE TESTS
char *path = "file.save";
START_TEST(test_save_empty_buffer)
{
    ck_assert_int_eq(insert_line(buff, "", 0), 0);

    ck_assert_int_eq(save_buffer(path, buff), 0);
    remove(path);
}

END_TEST START_TEST(test_save_one_char_buffer)
{
    ck_assert_int_eq(insert_line(buff, "", 0), 0);
    ck_assert_int_eq(insert_char(buff, 'a', 0, 0), 0);

    ck_assert_int_eq(save_buffer(path, buff), 0);
    remove(path);
}

END_TEST START_TEST(test_save_one_line_buffer)
{
    ck_assert_int_eq(insert_line(buff, "hello", 0), 0);

    ck_assert_int_eq(save_buffer(path, buff), 0);
    remove(path);
}

END_TEST START_TEST(test_save_buffer_with_long_line)
{
    ck_assert_int_eq(insert_line(buff,
                                 "helloooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
                                 0), 0);

    ck_assert_int_eq(save_buffer(path, buff), 0);
    remove(path);
}

END_TEST START_TEST(test_save_multiple_lines)
{
    ck_assert_int_eq(insert_line(buff, "hello\n", 0), 0);
    ck_assert_int_eq(insert_line(buff, "helloooooooooo   ooo\n", 1), 0);
    ck_assert_int_eq(insert_line(buff, "hellooooooooooooo oooooooo\n", 2), 0);
    ck_assert_int_eq(insert_line(buff, "wooorld", 3), 0);

    ck_assert_int_eq(save_buffer(path, buff), 0);
    remove(path);
}

END_TEST START_TEST(test_save_buffer_two_times)
{
    ck_assert_int_eq(insert_line(buff, "hello", 0), 0);
    ck_assert_int_eq(save_buffer(path, buff), 0);

    ck_assert_int_eq(insert_char(buff, 'a', 0, 0), 0);
    ck_assert_int_eq(save_buffer(path, buff), 0);

    remove(path);
}

END_TEST START_TEST(test_load_empty_buffer)
{
    ck_assert_int_eq(insert_line(buff, "", 0), 0);
    ck_assert_int_eq(save_buffer(path, buff), 0);
    ck_assert_ptr_ne(load_file(path), NULL);

    BUFFER *buff2 = new_buffer();
    buff2 = load_file(path);

    str = get_line(buff2, 0);

    ck_assert_ptr_ne(str, NULL);
    ck_assert_str_eq(str, "\n");

    free(str);
    free_buffer(buff2);
    remove(path);
}

END_TEST START_TEST(test_load_one_line_buffer)
{
    ck_assert_int_eq(insert_line(buff, "", 0), 0);
    ck_assert_int_eq(insert_char(buff, 'a', 0, 0), 0);
    ck_assert_int_eq(insert_char(buff, 'b', 0, 1), 0);
    ck_assert_int_eq(save_buffer(path, buff), 0);

    ck_assert_ptr_ne(load_file(path), NULL);

    BUFFER *buff2 = new_buffer();
    buff2 = load_file(path);

    char *str = get_line(buff2, 0);
    ck_assert_str_eq(str, "ab\n");

    free(str);
    free_buffer(buff2);
    remove(path);
}

END_TEST START_TEST(test_load_long_line_buffer)
{
    ck_assert_int_eq(insert_line
                     (buff,
                      "helloooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
                      0), 0);
    ck_assert_int_eq(save_buffer(path, buff), 0);

    BUFFER *buff2 = new_buffer();
    buff2 = load_file(path);

    str = get_line(buff2, 0);
    ck_assert_ptr_ne(str, NULL);
    ck_assert_str_eq(str,
                     "helloooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");

    free(str);
    free_buffer(buff2);
    remove(path);
}

END_TEST START_TEST(test_load_multiple_lines_buffer)
{
    ck_assert_int_eq(insert_line(buff, "hello", 0), 0);
    ck_assert_int_eq(insert_line(buff, "helloooooooooo   ooo", 1), 0);
    ck_assert_int_eq(insert_line(buff, "hellooooooooooooo oooooooo", 2), 0);
    ck_assert_int_eq(insert_line(buff, "wooorld", 3), 0);
    ck_assert_int_eq(save_buffer(path, buff), 0);

    BUFFER *buff2 = new_buffer();
    buff2 = load_file(path);

    str = get_line(buff2, 0);
    ck_assert_str_eq(str, "hello\n");
    str = get_line(buff2, 1);
    ck_assert_str_eq(str, "helloooooooooo   ooo\n");
    str = get_line(buff2, 2);
    ck_assert_str_eq(str, "hellooooooooooooo oooooooo\n");
    str = get_line(buff2, 3);
    ck_assert_str_eq(str, "wooorld\n");

    free(str);
    free_buffer(buff2);
    remove(path);
}

END_TEST Suite *io_text_suite(void)
{
    TCase *tc_io_text;
    Suite *s;

    tc_io_text = tcase_create("Core");
    tcase_add_checked_fixture(tc_io_text, buffer_setup, buffer_teardown);
    tcase_add_test(tc_io_text, test_save_empty_buffer);
    tcase_add_test(tc_io_text, test_save_one_char_buffer);
    tcase_add_test(tc_io_text, test_save_one_line_buffer);
    tcase_add_test(tc_io_text, test_save_buffer_with_long_line);
    tcase_add_test(tc_io_text, test_save_multiple_lines);
    tcase_add_test(tc_io_text, test_save_buffer_two_times);
    tcase_add_test(tc_io_text, test_load_empty_buffer);
    tcase_add_test(tc_io_text, test_load_one_line_buffer);
    tcase_add_test(tc_io_text, test_load_long_line_buffer);
    tcase_add_test(tc_io_text, test_load_multiple_lines_buffer);

    s = suite_create("Io_text");
    suite_add_tcase(s, tc_io_text);

    return s;
}

// SETTINGS MODULE TESTS
START_TEST(test_save_sets)
{
    SETTINGS *sets = (SETTINGS *) malloc(sizeof(int) * 8);
    set_auto_fill_mode(sets, 1);
    set_save_shortcut(sets, 18);
    set_load_shortcut(sets, 12);
    set_settings_shortcut(sets, 16);
    set_copy_shortcut(sets, 23);
    set_cut_shortcut(sets, 24);
    set_paste_shortcut(sets, 22);
    set_toogle_selection_shortcut(sets, 20);

    ck_assert_int_eq(save(sets, path), 0);
    free(sets);
    remove(path);
}

END_TEST START_TEST(test_load_sets)
{
    SETTINGS *sets = (SETTINGS *) malloc(sizeof(int) * 8);
    set_auto_fill_mode(sets, 1);
    set_save_shortcut(sets, 18);
    set_load_shortcut(sets, 12);
    set_settings_shortcut(sets, 16);
    set_copy_shortcut(sets, 23);
    set_cut_shortcut(sets, 24);
    set_paste_shortcut(sets, 22);
    set_toogle_selection_shortcut(sets, 20);

    ck_assert_int_eq(save(sets, path), 0);

    SETTINGS *sets2 = (SETTINGS *) malloc(sizeof(int) * 8);
    ck_assert_int_eq(load(sets2, path), 0);

    ck_assert_int_eq(get_auto_fill_mode(sets2), 1);
    ck_assert_int_eq(get_save_shortcut(sets2), 18);
    ck_assert_int_eq(get_load_shortcut(sets2), 12);
    ck_assert_int_eq(get_settings_shortcut(sets2), 16);
    ck_assert_int_eq(get_copy_shortcut(sets2), 23);
    ck_assert_int_eq(get_cut_shortcut(sets2), 24);
    ck_assert_int_eq(get_paste_shortcut(sets2), 22);
    ck_assert_int_eq(get_toogle_selection_shortcut(sets2), 20);
    free(sets);
    free(sets2);
    remove(path);

}

END_TEST START_TEST(test_to_string)
{
    ck_assert_str_eq(to_string(1), "Ctrl + A");
    ck_assert_str_eq(to_string(2), "Ctrl + B");
    ck_assert_str_eq(to_string(4), "Ctrl + D");
    ck_assert_str_eq(to_string(5), "Ctrl + E");
    ck_assert_str_eq(to_string(6), "Ctrl + F");
    ck_assert_str_eq(to_string(7), "Ctrl + G");
    ck_assert_str_eq(to_string(8), "Ctrl + H");
    ck_assert_str_eq(to_string(9), "Ctrl + I");
    ck_assert_str_eq(to_string(11), "Ctrl + K");
    ck_assert_str_eq(to_string(12), "Ctrl + L");
    ck_assert_str_eq(to_string(14), "Ctrl + N");
    ck_assert_str_eq(to_string(15), "Ctrl + O");
    ck_assert_str_eq(to_string(16), "Ctrl + P");
    ck_assert_str_eq(to_string(18), "Ctrl + R");
    ck_assert_str_eq(to_string(20), "Ctrl + T");
    ck_assert_str_eq(to_string(21), "Ctrl + U");
    ck_assert_str_eq(to_string(22), "Ctrl + V");
    ck_assert_str_eq(to_string(23), "Ctrl + W");
    ck_assert_str_eq(to_string(24), "Ctrl + X");
    ck_assert_str_eq(to_string(25), "Ctrl + Y");
}

END_TEST START_TEST(test_good_config_file)
{
    SETTINGS *sets = (SETTINGS *) malloc(sizeof(int) * 8);
    set_auto_fill_mode(sets, 1);
    set_save_shortcut(sets, 18);
    set_load_shortcut(sets, 12);
    set_settings_shortcut(sets, 16);
    set_copy_shortcut(sets, 23);
    set_cut_shortcut(sets, 24);
    set_paste_shortcut(sets, 22);
    set_toogle_selection_shortcut(sets, 20);

    ck_assert_int_eq(save(sets, path), 0);

    ck_assert_int_eq(is_valid_config(sets, path), 1);
    remove(path);
    free(sets);
}

END_TEST Suite *settings_suite(void)
{
    TCase *tc_settings;
    Suite *s;

    tc_settings = tcase_create("Core");
    tcase_add_test(tc_settings, test_save_sets);
    tcase_add_test(tc_settings, test_load_sets);
    tcase_add_test(tc_settings, test_to_string);
    tcase_add_test(tc_settings, test_good_config_file);

    s = suite_create("Settings");
    suite_add_tcase(s, tc_settings);

    return s;
}

int main(void)
{
    int failures;
    Suite *s1;
    Suite *s2;
    Suite *s3;
    SRunner *sr1;
    SRunner *sr2;
    SRunner *sr3;

    s1 = buffer_suite();
    s2 = io_text_suite();
    s3 = settings_suite();
    sr1 = srunner_create(s1);
    sr2 = srunner_create(s2);
    sr3 = srunner_create(s3);

    srunner_run_all(sr1, CK_NORMAL);
    srunner_run_all(sr2, CK_NORMAL);
    srunner_run_all(sr3, CK_NORMAL);
    failures = srunner_ntests_failed(sr1);
    failures = failures + srunner_ntests_failed(sr2);
    failures = failures + srunner_ntests_failed(sr3);
    srunner_free(sr1);
    srunner_free(sr2);
    srunner_free(sr3);

    return (failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

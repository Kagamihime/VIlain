#include <stdlib.h>
#include <string.h>
#include "../include/buffer.h"

#define LINE_SIZE 81
#define INITIAL_LINE_COUNT 100

static int split_lines(char *str, char ***lines, int *line_count)
{
    char *current_line_ptr = str;
    *line_count = 0;

    if (str == NULL) {
        return -1;
    }

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            (*line_count)++;
        }
    }
    // Count when the last line doesn't end with a line return
    if (str[strlen(str) - 1] != '\n') {
        (*line_count)++;
    }

    if ((*lines = (char **)calloc(*line_count, sizeof(char *))) == NULL) {
        return -1;
    }

    for (int i = 0; i < *line_count; i++) {
        int line_length = 0;

        for (char *tmp_ptr = current_line_ptr;
             *tmp_ptr != '\n' && *tmp_ptr != '\0'; tmp_ptr++, line_length++) {
        }

        if (((*lines)[i] =
             (char *)calloc(line_length + 1, sizeof(char))) == NULL) {
            free(*lines);
            *lines = NULL;
            return -1;
        }

        strncpy((*lines)[i], current_line_ptr, line_length);

        current_line_ptr += line_length;
        if (*current_line_ptr == '\n') {
            current_line_ptr++;
        }
    }

    return 0;
}

struct LINE {
    // Contains the string matching the line without its line return character
    // Contains `NULL` if the line is empty
    char *str;

    // Should always be greater or equal to `length` + 1
    int capacity;
    // Contains the line length ('\0' is not counted)
    int length;
};

static struct LINE *new_line()
{
    struct LINE *line = NULL;

    if ((line = (struct LINE *)calloc(1, sizeof(struct LINE))) == NULL) {
        return NULL;
    }

    if ((line->str = (char *)calloc(LINE_SIZE, sizeof(char))) == NULL) {
        free(line);
        return NULL;
    }

    line->capacity = LINE_SIZE;
    line->length = 0;

    return line;
}

static int grow_line(struct LINE *line)
{
    if (line == NULL) {
        return -1;
    }

    if ((line->str =
         (char *)realloc((void *)line->str,
                         (line->capacity * 2) * sizeof(char))) == NULL) {
        return -1;
    }

    line->capacity *= 2;

    return 0;
}

static void free_line(struct LINE *line)
{
    if (line != NULL) {
        if (line->str != NULL) {
            free(line->str);
            line->str = NULL;
        }

        free(line);
        line = NULL;
    }
}

static char line_get_char(struct LINE *line, int col)
{
    if (line == NULL || col < 0 || col >= line->length) {
        return '\0';
    }

    return line->str[col];
}

static char *line_get_segment(struct LINE *line, int from_col, int to_col)
{
    char *ret = NULL;

    if (line == NULL || line->length == 0 || from_col < 0 || from_col > to_col
        || to_col >= line->length) {
        return NULL;
    }

    if ((ret = (char *)calloc(line->length + 2, sizeof(char))) == NULL) {
        return NULL;
    }

    strncpy(ret, line->str + from_col, to_col - from_col + 1);

    return ret;
}

static int line_insert_segment(struct LINE *line, char *str, int col)
{
    size_t str_length = strlen(str);

    if (line == NULL || str == NULL || col < 0 || col >= line->length + 1) {
        return -1;
    }

    while (line->length + str_length + 1 > line->capacity) {
        if (grow_line(line) == -1) {
            return -1;
        }
    }

    if (line->length > 0) {
        for (int i = line->length - 1; i >= col; i--) {
            line->str[i + str_length] = line->str[i];
        }
    }
    line->str[line->length + str_length] = '\0';

    strncpy(&line->str[col], str, str_length);

    line->length += str_length;

    return 0;
}

static int line_insert_char(struct LINE *line, char c, int col)
{
    char str[2] = { c, '\0' };

    return line_insert_segment(line, str, col);
}

// FIXME: Duplicated code
static int line_delete_char(struct LINE *line, int col)
{
    if (line == NULL || col < 0 || col >= line->length) {
        return -1;
    }

    for (int i = col; i < line->length - 1; i++) {
        line->str[i] = line->str[i + 1];
    }
    line->str[line->length - 1] = '\0';

    line->length--;

    return 0;
}

// FIXME: Duplicated code
static int line_delete_segment(struct LINE *line, int from_col, int to_col)
{
    size_t segment_length = to_col - from_col + 1;

    if (line == NULL || from_col < 0 || from_col > to_col
        || to_col >= line->length) {
        return -1;
    }

    for (int i = from_col; i <= line->length - segment_length + 1; i++) {
        line->str[i] = line->str[i + segment_length];
    }
    line->str[line->length - segment_length] = '\0';

    line->length -= segment_length;

    return 0;
}

static void line_truncate(struct LINE *line)
{
    if (line == NULL || line->str == NULL) {
        return;
    }

    line->str[0] = '\0';

    line->length = 0;
}

static int line_override_char(struct LINE *line, char c, int col)
{
    if (line == NULL || col < 0 || col >= line->length) {
        return -1;
    }

    line->str[col] = c;

    return 0;
}

struct BUFFER {
    struct LINE **lines;

    int capacity;
    int line_count;
};

BUFFER *new_buffer()
{
    BUFFER *buff = NULL;

    if ((buff = (BUFFER *) calloc(1, sizeof(BUFFER))) == NULL) {
        return NULL;
    }
    buff->lines = NULL;

    if ((buff->lines =
         (struct LINE **)calloc(INITIAL_LINE_COUNT,
                                sizeof(struct LINE *))) == NULL) {
        free(buff);
        return NULL;
    }

    for (int i = 0; i < INITIAL_LINE_COUNT; i++) {
        if ((buff->lines[i] = new_line()) == NULL) {
            free(buff);
            return NULL;
        }
    }

    buff->capacity = INITIAL_LINE_COUNT;
    buff->line_count = 0;

    return buff;
}

static int grow_buffer(BUFFER * buff)
{
    if (buff == NULL) {
        return -1;
    }

    if ((buff->lines =
         (struct LINE **)realloc((void *)buff->lines,
                                 (buff->capacity * 2) *
                                 sizeof(struct LINE *))) == NULL) {
        return -1;
    }
    for (int i = buff->capacity; i < buff->capacity * 2; i++) {
        buff->lines[i] = NULL;
    }

    buff->capacity *= 2;

    return 0;
}

void free_buffer(BUFFER * buff)
{
    if (buff != NULL) {
        for (int i = 0; i < buff->capacity; i++) {
            if (buff->lines[i] != NULL) {
                free_line(buff->lines[i]);
                buff->lines[i] = NULL;
            }
        }

        free(buff);
        buff = NULL;
    }
}

char get_char(BUFFER * buff, int line, int col)
{
    if (buff == NULL || line < 0 || line >= buff->line_count) {
        return '\0';
    }

    return line_get_char(buff->lines[line], col);
}

char *get_line(BUFFER * buff, int line)
{
    char *ret = NULL;

    if (buff == NULL || line < 0 || line >= buff->line_count) {
        return NULL;
    }

    if (buff->lines[line]->length == 0) {
        if ((ret = (char *)calloc(2, sizeof(char))) == NULL) {
            return NULL;
        }

        strcpy(ret, "\n");
        return ret;
    }

    if ((ret =
         line_get_segment(buff->lines[line], 0,
                          buff->lines[line]->length - 1)) == NULL) {
        return NULL;
    }

    strcat(ret, "\n");

    return ret;
}

char *get_text(BUFFER * buff, int from_line, int from_col,
               int to_line, int to_col)
{
    char *text = NULL;
    int text_length = 0;

    if (buff == NULL || from_line < 0 || from_line > to_line
        || to_line >= buff->line_count) {
        return NULL;
    }

    for (int i = from_line; i <= to_line; i++) {
        text_length +=
            ((i == to_line) ? to_col : buff->lines[i]->length - 1) - ((i ==
                                                                       from_line)
                                                                      ? from_col
                                                                      : 0) + 1;
        text_length++;          // Line return
    }
    if ((text = (char *)calloc(text_length + 1, sizeof(char))) == NULL) {
        return NULL;
    }

    for (int i = from_line; i <= to_line; i++) {
        int line_from_col = (i == from_line) ? from_col : 0;
        int line_to_col = (i == to_line) ? to_col : buff->lines[i]->length - 1;
        char *line_str = NULL;

        if ((line_str =
             line_get_segment(buff->lines[i], line_from_col,
                              line_to_col)) == NULL) {
            free(text);
            return NULL;
        }
        strncat(text, line_str, line_to_col - line_from_col + 1);
        strcat(text, "\n");

        free(line_str);
    }

    // Trim the last line return if needed
    if (to_col < buff->lines[to_line]->length - 1
        && text[strlen(text) - 1] == '\n') {
        text[strlen(text) - 1] = '\0';
    }

    return text;
}

int insert_char(BUFFER * buff, char c, int line, int col)
{
    if (buff == NULL || line < 0 || line >= buff->line_count) {
        return -1;
    }

    return line_insert_char(buff->lines[line], c, col);
}

int insert_line(BUFFER * buff, char *str, int line)
{
    if (buff == NULL || str == NULL || line < 0 || line >= buff->line_count + 1) {
        return -1;
    }

    while (buff->line_count + 1 > buff->capacity) {
        if (grow_buffer(buff) == -1) {
            return -1;
        }
    }

    if (buff->line_count > 0) {
        for (int i = buff->line_count - 1; i >= line; i--) {
            buff->lines[i + 1] = buff->lines[i];
        }
    }

    if ((buff->lines[line] = new_line()) == NULL) {
        return -1;
    }
    if (line_insert_segment(buff->lines[line], str, 0) == -1) {
        return -1;
    }

    buff->line_count++;

    return 0;
}

int insert_text(BUFFER * buff, char *str, int line, int col)
{
    if (buff == NULL || str == NULL || line < 0 || line >= buff->line_count + 1) {
        return -1;
    }

    if (line_insert_segment(buff->lines[line], str, col) == -1) {
        return -1;
    }

    if (autosplit_line(buff, line) == -1) {
        return -1;
    }

    return 0;
}

int delete_char(BUFFER * buff, int line, int col)
{
    if (buff == NULL || line < 0 || line >= buff->line_count) {
        return -1;
    }

    return line_delete_char(buff->lines[line], col);
}

int delete_line(BUFFER * buff, int line)
{
    if (buff == NULL || line < 0 || line >= buff->line_count) {
        return -1;
    }

    free_line(buff->lines[line]);

    for (int i = line; i < buff->line_count - 1; i++) {
        buff->lines[i] = buff->lines[i + 1];
    }

    buff->lines[buff->line_count - 1] = NULL;

    buff->line_count--;

    return 0;
}

int delete_text(BUFFER * buff, int from_line, int from_col,
                int to_line, int to_col)
{
    if (buff == NULL || from_line < 0 || from_line > to_line
        || to_line >= buff->line_count) {
        return -1;
    }

    for (int i = from_line; i < to_line; i++) {
        to_col += buff->lines[i]->length;
    }

    if (join_lines(buff, from_line, to_line, 0) == -1) {
        return -1;
    }

    if (line_delete_segment(buff->lines[from_line], from_col, to_col) == -1) {
        return -1;
    }

    return 0;
}

int override_char(BUFFER * buff, char c, int line, int col)
{
    if (buff == NULL || line < 0 || line >= buff->line_count) {
        return -1;
    }

    return line_override_char(buff->lines[line], c, col);
}

int override_line(BUFFER * buff, char *str, int line)
{
    if (buff == NULL || str == NULL || line < 0 || line >= buff->line_count) {
        return -1;
    }

    line_truncate(buff->lines[line]);
    return line_insert_segment(buff->lines[line], str, 0);
}

int override_text(BUFFER * buff, char *str, int from_line,
                  int from_col, int to_line, int to_col)
{
    if (buff == NULL || str == NULL || from_line < 0 || from_line > to_line
        || to_line >= buff->line_count) {
        return -1;
    }

    return (delete_text(buff, from_line, from_col, to_line, to_col)
            || insert_text(buff, str, from_line, from_col)) ? -1 : 0;
}

int autosplit_line(BUFFER * buff, int line)
{
    char *line_to_split = NULL;
    char **splitted_line = NULL;
    int splitted_line_count = 0;

    if (buff == NULL || line < 0 || line >= buff->line_count + 1
        || buff->lines[line]->length == 0) {
        return -1;
    }

    if ((line_to_split =
         line_get_segment(buff->lines[line], 0,
                          buff->lines[line]->length - 1)) == NULL) {
        return -1;
    }
    line_truncate(buff->lines[line]);

    if (split_lines(line_to_split, &splitted_line, &splitted_line_count) == -1) {
        return -1;
    }

    if (line_insert_segment(buff->lines[line], splitted_line[0], 0) == -1) {
        return -1;
    }

    if (line == buff->line_count) {
        buff->line_count++;
    }

    for (int i = 1; i < splitted_line_count; i++) {
        if (insert_line(buff, splitted_line[i], line + i) == -1) {
            return -1;
        }
    }

    for (int i = 0; i < splitted_line_count; i++) {
        if (splitted_line[i] != NULL) {
            free(splitted_line[i]);
            splitted_line[i] = NULL;
        }
    }
    if (splitted_line != NULL) {
        free(splitted_line);
        splitted_line = NULL;
    }
    return 0;
}

int split_line_at(BUFFER * buff, int line, int col)
{
    if (buff == NULL || line < 0 || line >= buff->line_count) {
        return -1;
    }

    if (line_insert_char(buff->lines[line], '\n', col) == -1) {
        return -1;
    }

    if (autosplit_line(buff, line) == -1) {
        return -1;
    }

    return 0;
}

int join_lines(BUFFER * buff, int from_line, int to_line, int with_spaces)
{
    char *str = NULL;
    int str_length = 0;

    char *to_append = NULL;

    int lines_to_delete = to_line - from_line;

    if (buff == NULL || from_line < 0 || from_line > to_line
        || to_line >= buff->line_count) {
        return -1;
    }

    for (int i = from_line; i <= to_line; i++) {
        str_length += buff->lines[i]->length;

        if (with_spaces && i < to_line) {
            str_length++;
        }
    }
    str_length++;               // For '\0'
    if ((str = (char *)calloc(str_length, sizeof(char))) == NULL) {
        return -1;
    }

    for (int i = from_line; i <= to_line; i++) {
        if ((to_append =
             line_get_segment(buff->lines[i], 0,
                              buff->lines[i]->length - 1)) == NULL) {
            free(str);
            return -1;
        }
        strncat(str, to_append, str_length);
        if (with_spaces && i < to_line) {
            strcat(str, " ");
        }
    }

    line_truncate(buff->lines[from_line]);
    for (int i = 0; i < lines_to_delete; i++) {
        delete_line(buff, from_line + 1);
    }

    if (line_insert_segment(buff->lines[from_line], str, 0) == -1) {
        free(str);
        return -1;
    }

    free(str);
    return 0;
}

int get_line_count(BUFFER * buff)
{
    if (buff == NULL) {
        return -1;
    }

    return buff->line_count;
}

int get_line_length(BUFFER * buff, int line)
{
    if (buff == NULL || line < 0 || line >= buff->line_count) {
        return -1;
    }

    return buff->lines[line]->length;
}

int get_buffer_length(BUFFER * buff)
{
    int buff_length = 0;

    if (buff == NULL) {
        return -1;
    }

    for (int i = 0; i < buff->line_count; i++) {
        buff_length += buff->lines[i]->length + 1;  // Count the line returns
    }

    return buff_length;
}

#include <stdlib.h>
#include <string.h>
#include "../include/buffer.h"

#define LINE_SIZE 1000
#define MAX_LINE 10000

struct LINE {
    // Contains the string matching the line without its line return character
    // Contains `NULL` if the line is empty
    char *str;

    // Should always be greater or equal to `length` + 1
    unsigned int capacity;
    // Contains the line length ('\0' is not counted)
    unsigned int length;
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

static char line_get_char(struct LINE *line, unsigned int col)
{
    // Check if `col` is out of bound
    if (col > line->length) {
        return '\0';
    }
    // Return the requested character
    return line->str[col];
}

static char *line_get_segment(struct LINE *line, unsigned int from_col,
                              unsigned int to_col)
{
    char *ret = NULL;

    // Check if the cols are incorrect
    if (from_col > to_col || to_col >= line->length) {
        return NULL;
    }
    // Return NULL if the line is empty
    if (line->length == 0) {
        return NULL;
    }
    // Return the line segment in a new allocated string
    if ((ret = (char *)calloc(line->length + 2, sizeof(char))) == NULL) {
        return NULL;
    }
    strncpy(ret, line->str + from_col, to_col - from_col + 1);
    return ret;
}

// FIXME: Duplicated code
static int line_insert_char(struct LINE *line, unsigned char c,
                            unsigned int col)
{
    // Check if `col` is out of bound
    if (col >= line->length) {
        return -1;
    }
    // Check that the line's capacity is enough
    if (line->length + 2 > line->capacity) {
        return -1;
    }
    // Shift all characters after `col` to the right
    for (unsigned int i = line->length - 1; i >= col; i--) {
        line->str[i + 1] = line->str[i];
    }

    // Make sure that the string is null-terminated
    line->str[line->length + 1] = '\0';

    // Write the new character at `col`
    line->str[col] = c;

    // Increment the line length
    line->length++;

    return 0;
}

// FIXME: Duplicated code
static int line_insert_segment(struct LINE *line, char *str, unsigned int col)
{
    size_t str_length = strlen(str);

    // Check if `col` is out of bound
    if (col >= line->length) {
        return -1;
    }
    // Check that the line's capacity is enough
    if (line->length + str_length + 1 > line->capacity) {
        return -1;
    }
    // Shift all characters after `col` to the right
    for (unsigned int i = line->length - 1; i >= col; i--) {
        line->str[i + str_length] = line->str[i];
    }

    // Make sure that the string is null-terminated
    line->str[line->length + str_length] = '\0';

    // Copy the string at `col`
    strncpy(&line->str[col], str, str_length);

    // Increment the line length
    line->length += str_length;

    return 0;
}

// FIXME: Duplicated code
static int line_delete_char(struct LINE *line, unsigned int col)
{
    // Check if `col` is out of bound
    if (col >= line->length) {
        return -1;
    }
    // Shift all the characters after `col` to the left
    for (unsigned int i = col; i < line->length - 1; i++) {
        line->str[i] = line->str[i + 1];
    }

    // Null-terminate the string to make it shorter
    line->str[line->length - 1] = '\0';

    // Decrement the line length
    line->length--;

    return 0;
}

static int line_truncate(struct LINE *line)
{
    // Check if the line is already empty
    if (line->str == NULL) {
        return -1;
    }
    // Truncate the line
    line->str[0] = '\0';

    // Update the line length
    line->length = 0;

    return 0;
}

// FIXME: Duplicated code
static int line_delete_segment(struct LINE *line, unsigned int from_col,
                               unsigned int to_col)
{
    size_t segment_length = to_col - from_col + 1;

    // Check if the cols are incorrect
    if (from_col > to_col || to_col >= line->length) {
        return -1;
    }
    // Shift all the characters after `to_col` (excluded) to the left, by an
    // offset of `segment_length`
    for (int i = to_col + 1; i <= line->length - segment_length; i++) {
        line->str[i] = line->str[i + segment_length];
    }

    // Null-terminate the string to make it shorter
    line->str[line->length - segment_length] = '\0';

    // Decrement the line length
    line->length -= segment_length;

    return 0;
}

static int line_override_char(struct LINE *line, char c, unsigned int col)
{
    // Check if `col` is out of bound
    if (col >= line->length) {
        return -1;
    }
    // Override the character at `col`
    line->str[col] = c;

    return 0;
}

static int line_override_segment(struct LINE *line, char *str,
                                 unsigned int from_col, unsigned int to_col)
{
    // Check if the cols are incorrect
    if (from_col > to_col || to_col >= line->length) {
        return -1;
    }

    return line_delete_segment(line, from_col, to_col)
        || line_insert_segment(line, str, from_col);
}

struct BUFFER {
    struct LINE **lines;

    unsigned int capacity;
    unsigned int line_count;
};

BUFFER *new_buffer()
{
    // Allocate the BUFFER struct
    BUFFER *buff = NULL;
    if ((buff = (BUFFER *) calloc(1, sizeof(BUFFER))) == NULL) {
        return NULL;
    }
    // Allocate the lines
    buff->lines = NULL;
    if ((buff->lines =
         (struct LINE **)calloc(MAX_LINE, sizeof(struct LINE *))) == NULL) {
        free(buff);
        return NULL;
    }
    for (unsigned int i = 0; i < MAX_LINE; i++) {
        if ((buff->lines[i] = new_line()) == NULL) {
            free(buff);
            return NULL;
        }
    }

    // Initialize the other fields
    buff->capacity = MAX_LINE;
    buff->line_count = 0;

    return buff;
}

void free_buffer(BUFFER * buff)
{
    if (buff != NULL) {
        for (unsigned int i = 0; i < buff->capacity; i++) {
            if (buff->lines[i] != NULL) {
                free_line(buff->lines[i]);
                buff->lines[i] = NULL;
            }
        }

        free(buff);
        buff = NULL;
    }
}

char get_char(BUFFER * buff, unsigned int line, unsigned int col)
{
    // Check if `line` is out of bound
    if (line >= buff->line_count) {
        return '\0';
    }

    return line_get_char(buff->lines[line], col);
}

char *get_line(BUFFER * buff, unsigned int line)
{
    // Check if `line` is out of bound
    if (line >= buff->line_count) {
        return NULL;
    }

    return
        strcat(line_get_segment
               (buff->lines[line], 0, buff->lines[line]->length), "\n");
}

char *get_text(BUFFER * buff, unsigned int from_line, unsigned int from_col,
               unsigned int to_line, unsigned int to_col)
{
    unsigned int text_length = 0;
    char *text = NULL;
    unsigned int seg_count = 0;
    unsigned int **seg_cols = NULL;

    // Check if the lines are incorrect
    if (from_line > to_line || to_line > buff->line_count) {
        return NULL;
    }
    // Calculate the segments to get
    if (from_col > 0) {
        seg_count++;
    }
    if (to_col < buff->lines[to_line]->length - 1 && from_line < to_line) {
        seg_count++;
    }
    seg_count += to_line - from_line - 1;
    if ((seg_cols =
         (unsigned int **)calloc(seg_count, 2 * sizeof(int))) == NULL) {
        return NULL;
    }
    // Fill in the `from_col`s
    seg_cols[0][0] = from_col;
    // Fill in the `to_col`s
    for (unsigned int i = 0; i < seg_count - 1; i++) {
        seg_cols[i][1] = buff->lines[i]->length - 1;
    }
    seg_cols[seg_count - 1][1] = to_col;

    // Allocate the memory for the text
    for (unsigned int i = 0; i < seg_count; i++) {
        text_length += seg_cols[i][1] - seg_cols[i][0] + 1;
        text_length++;          // Line return
    }
    if ((text = (char *)calloc(text_length + 1, sizeof(char))) == NULL) {
        free(seg_cols);
        return NULL;
    }
    // Get the segments and concatenate them
    for (int i = 0; i < seg_count; i++) {
        strncat(text,
                line_get_segment(buff->lines[from_line + i], seg_cols[i][0],
                                 seg_cols[i][1]),
                buff->lines[from_line + i]->length);
        strcat(text, "\n");
    }

    // Trim the nast new line if needed
    if (to_col < buff->lines[to_line]->length - 1
        && text[strlen(text) - 1] == '\n') {
        text[strlen(text) - 1] = '\0';
    }

    free(seg_cols);
    return text;
}

int insert_char(BUFFER * buff, char c, unsigned int line, unsigned int col)
{
    // Check if `line` is out of bound
    if (line >= buff->line_count) {
        return -1;
    }

    return line_insert_char(buff->lines[line], c, col);
}

int insert_line(BUFFER * buff, char *str, unsigned int line)
{
    // Check if `line` is out of bound
    if (line >= buff->line_count) {
        return -1;
    }
    // Check that the buffer capacity is enough
    if (buff->line_count + 1 > buff->capacity) {
        return -1;
    }
    // Shift all lines after `line` to the end of the buffer
    for (unsigned int i = buff->line_count - 1; i >= line; i--) {
        buff->lines[i + 1] = buff->lines[i];
    }

    // Insert the new line at `line`
    if ((buff->lines[line] = new_line()) == NULL) {
        return -1;
    }
    if (line_insert_segment(buff->lines[line], str, 0) == -1) {
        return -1;
    }
    // Increment the line count
    buff->line_count++;

    return 0;
}

int insert_text(BUFFER * buff, char *str, unsigned int from_line,
                unsigned int from_col, unsigned int to_line,
                unsigned int to_col)
{
    return 0;
}

int override_char(BUFFER * buff, char c, unsigned int line, unsigned int col)
{
    // Check if `line` is out of bound
    if (line >= buff->line_count) {
        return -1;
    }

    return line_override_char(buff->lines[line], c, col);
}

int override_line(BUFFER * buff, char *str, unsigned int line)
{
    // Check if `line` is out of bound
    if (line >= buff->line_count) {
        return -1;
    }

    return line_truncate(buff->lines[line])
        || line_insert_segment(buff->lines[line], str, 0);
}

int override_text(BUFFER * buff, char *str, unsigned int from_line,
                  unsigned int from_col, unsigned int to_line,
                  unsigned int to_col)
{
    return 0;
}

int delete_char(BUFFER * buff, unsigned int line, unsigned int col)
{
    // Check if `line` is out of bound
    if (line >= buff->line_count) {
        return -1;
    }

    return line_delete_char(buff->lines[line], col);
}

int delete_line(BUFFER * buff, int unsigned line)
{
    // Check if `line` is out of bound
    if (line >= buff->line_count) {
        return -1;
    }
    // Free the line at `line`
    free_line(buff->lines[line]);

    // Shift all the lines after `line` to the beginning of the buffer
    for (unsigned int i = line; i < buff->line_count - 1; i++) {
        buff->lines[i] = buff->lines[i + 1];
    }

    // Erase the last pointer
    buff->lines[buff->line_count - 1] = NULL;

    // Decrement the line count
    buff->line_count--;

    return 0;
}

int delete_text(BUFFER * buff, unsigned int from_line, unsigned int from_col,
                unsigned int to_line, unsigned int to_col)
{
    return 0;
}

unsigned int get_line_count(BUFFER * buff)
{
    return buff->line_count;
}

unsigned int get_line_length(BUFFER * buff, unsigned int line)
{
    return buff->lines[line]->length;
}

unsigned int get_buffer_length(BUFFER * buff)
{
    unsigned int buff_length = 0;

    for (unsigned int i = 0; i < buff->line_count; i++) {
        buff_length += buff->lines[i]->length + 1;  // Count the line returns
    }

    return buff_length;
}

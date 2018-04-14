#include <stdlib.h>
#include <string.h>
#include "../include/buffer.h"

#define LINE_SIZE 81
#define MAX_LINE 100

static int split_lines(char *str, char ***lines, int *line_count)
{
    char *ptr = str;

    // Check if the string is empty
    if (str == NULL) {
        return -1;
    }
    // Place the pointer at the beginning of the string
    ptr = str;

    // Count the number of lines
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            (*line_count)++;
        }
    }
    // Count when the last line doesn't end with a line return
    if (str[strlen(str) - 1] != '\n') {
        (*line_count)++;
    }
    // Allocate the lines pointers
    if ((*lines = (char **)calloc(*line_count, sizeof(char *))) == NULL) {
        return -1;
    }
    // Allocate and copy the line in `lines` for each line
    for (int i = 0; i < *line_count; i++) {
        int line_length = 0;

        // Count the lines length
        for (char *tmp_ptr = ptr; *tmp_ptr != '\n' && *tmp_ptr != '\0';
             tmp_ptr++, line_length++) {
        }

        // Allocate the lines
        if (((*lines)[i] =
             (char *)calloc(line_length + 1, sizeof(char))) == NULL) {
            free(*lines);
            *lines = NULL;
            return -1;
        }
        // Copy the line
        strncpy((*lines)[i], ptr, line_length);

        // Place the pointer at the beginning of the next line
        ptr += line_length;
        if (*ptr == '\n') {
            ptr++;
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
    // Check if `col` is out of bound
    if (col < 0 || col >= line->length) {
        return '\0';
    }
    // Return the requested character
    return line->str[col];
}

static char *line_get_segment(struct LINE *line, int from_col, int to_col)
{
    char *ret = NULL;

    // Check if the cols are incorrect
    if (from_col < 0 || from_col > to_col || to_col >= line->length) {
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
static int line_insert_char(struct LINE *line, unsigned char c, int col)
{
    // Check if `col` is out of bound
    if (col < 0 || col >= line->length + 1) {
        return -1;
    }
    // Check that the line's capacity is enough
    if (line->length + 2 > line->capacity) {
        return -1;
    }
    // Shift all characters after `col` to the right
    if (line->length > 0) {
        for (int i = line->length - 1; i >= col; i--) {
            line->str[i + 1] = line->str[i];
        }
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
static int line_insert_segment(struct LINE *line, char *str, int col)
{
    size_t str_length = strlen(str);

    // Check if `col` is out of bound
    if (col < 0 || col >= line->length + 1) {
        return -1;
    }
    // Check that the line's capacity is enough
    if (line->length + str_length + 1 > line->capacity) {
        return -1;
    }
    // Shift all characters after `col` to the right
    if (line->length > 0) {
        for (int i = line->length - 1; i >= col; i--) {
            line->str[i + str_length] = line->str[i];
        }
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
static int line_delete_char(struct LINE *line, int col)
{
    // Check if `col` is out of bound
    if (col < 0 || col >= line->length) {
        return -1;
    }
    // Shift all the characters after `col` to the left
    for (int i = col; i < line->length - 1; i++) {
        line->str[i] = line->str[i + 1];
    }

    // Null-terminate the string to make it shorter
    line->str[line->length - 1] = '\0';

    // Decrement the line length
    line->length--;

    return 0;
}

// FIXME: Duplicated code
static int line_delete_segment(struct LINE *line, int from_col, int to_col)
{
    size_t segment_length = to_col - from_col + 1;

    // Check if the cols are incorrect
    if (from_col < 0 || from_col > to_col || to_col >= line->length) {
        return -1;
    }
    // Shift all the characters after `from_col` to the left, by an
    // offset of `segment_length`
    for (int i = from_col; i <= line->length - segment_length + 1; i++) {
        line->str[i] = line->str[i + segment_length];
    }

    // Null-terminate the string to make it shorter
    line->str[line->length - segment_length] = '\0';

    // Decrement the line length
    line->length -= segment_length;

    return 0;
}

static void line_truncate(struct LINE *line)
{
    // Check if the line is already empty
    if (line->str == NULL) {
        return;
    }
    // Truncate the line
    line->str[0] = '\0';

    // Update the line length
    line->length = 0;
}

static int line_override_char(struct LINE *line, char c, int col)
{
    // Check if `col` is out of bound
    if (col < 0 || col >= line->length) {
        return -1;
    }
    // Override the character at `col`
    line->str[col] = c;

    return 0;
}

static int line_override_segment(struct LINE *line, char *str,
                                 int from_col, int to_col)
{
    // Check if the cols are incorrect
    if (from_col < 0 || from_col > to_col || to_col >= line->length) {
        return -1;
    }

    return line_delete_segment(line, from_col, to_col)
        || line_insert_segment(line, str, from_col);
}

struct BUFFER {
    struct LINE **lines;

    int capacity;
    int line_count;
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
    for (int i = 0; i < MAX_LINE; i++) {
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
    // Check if `line` is out of bound
    if (line < 0 || line >= buff->line_count) {
        return '\0';
    }

    return line_get_char(buff->lines[line], col);
}

char *get_line(BUFFER * buff, int line)
{
    char *ret = NULL;

    // Check if `line` is out of bound
    if (line < 0 || line >= buff->line_count) {
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

    // Check if the lines are incorrect
    if (from_line < 0 || from_line > to_line || to_line >= buff->line_count) {
        return NULL;
    }
    // Allocate the memory for the text
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

        // Get segments and concatenate them with line returns
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
    // Check if `line` is out of bound
    if (line < 0 || line >= buff->line_count) {
        return -1;
    }

    return line_insert_char(buff->lines[line], c, col);
}

int insert_line(BUFFER * buff, char *str, int line)
{
    // Check if `line` is out of bound
    if (line < 0 || line >= buff->line_count + 1) {
        return -1;
    }
    // Check that the buffer capacity is enough
    if (buff->line_count + 1 > buff->capacity) {
        return -1;
    }
    // Shift all lines after `line` to the end of the buffer
    if (buff->line_count > 0) {
        for (int i = buff->line_count - 1; i >= line; i--) {
            buff->lines[i + 1] = buff->lines[i];
        }
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

int insert_text(BUFFER * buff, char *str, int line, int col)
{
    // Check if `line` is out of bound
    if (line < 0 || line >= buff->line_count + 1) {
        return -1;
    }
    // Insert the string at `line` and `col`
    if (line_insert_segment(buff->lines[line], str, col) == -1) {
        return -1;
    }
    // Autosplit this line
    if (autosplit_line(buff, line) == -1) {
        return -1;
    }

    return 0;
}

int delete_char(BUFFER * buff, int line, int col)
{
    // Check if `line` is out of bound
    if (line < 0 || line >= buff->line_count) {
        return -1;
    }

    return line_delete_char(buff->lines[line], col);
}

int delete_line(BUFFER * buff, int line)
{
    // Check if `line` is out of bound
    if (line < 0 || line >= buff->line_count) {
        return -1;
    }
    // Free the line at `line`
    free_line(buff->lines[line]);

    // Shift all the lines after `line` to the beginning of the buffer
    for (int i = line; i < buff->line_count - 1; i++) {
        buff->lines[i] = buff->lines[i + 1];
    }

    // Erase the last pointer
    buff->lines[buff->line_count - 1] = NULL;

    // Decrement the line count
    buff->line_count--;

    return 0;
}

int delete_text(BUFFER * buff, int from_line, int from_col,
                int to_line, int to_col)
{
    // Check if the lines are incorrect
    if (from_line < 0 || from_line > to_line || to_line >= buff->line_count) {
        return -1;
    }
    // Recalculate `to_col`
    for (int i = from_line; i < to_line; i++) {
        to_col += buff->lines[i]->length;
    }

    // Join the lines
    if (join_lines(buff, from_line, to_line, 0) == -1) {
        return -1;
    }
    // Delete the segment
    if (line_delete_segment(buff->lines[from_line], from_col, to_col) == -1) {
        return -1;
    }

    return 0;
}

int override_char(BUFFER * buff, char c, int line, int col)
{
    // Check if `line` is out of bound
    if (line < 0 || line >= buff->line_count) {
        return -1;
    }

    return line_override_char(buff->lines[line], c, col);
}

int override_line(BUFFER * buff, char *str, int line)
{
    // Check if `line` is out of bound
    if (line < 0 || line >= buff->line_count) {
        return -1;
    }

    line_truncate(buff->lines[line]);
    return line_insert_segment(buff->lines[line], str, 0);
}

int override_text(BUFFER * buff, char *str, int from_line,
                  int from_col, int to_line, int to_col)
{
    // Check if the lines are incorrect
    if (from_line < 0 || from_line > to_line || to_line >= buff->line_count) {
        return -1;
    }

    return delete_text(buff, from_line, from_col, to_line, to_col)
        || insert_text(buff, str, from_line, from_col);
}

int autosplit_line(BUFFER * buff, int line)
{
    char *line_to_split = NULL;
    char **splitted_line = NULL;
    int splitted_line_count = 0;

    // Check if the line is empty
    if (buff->lines[line]->length == 0) {
        return -1;
    }
    // Get and remove the content of the line
    if ((line_to_split =
         line_get_segment(buff->lines[line], 0,
                          buff->lines[line]->length - 1)) == NULL) {
        return -1;
    }
    line_truncate(buff->lines[line]);

    // Split the retrieved line
    if (split_lines(line_to_split, &splitted_line, &splitted_line_count) == -1) {
        return -1;
    }
    // Insert the splitted line
    if (line_insert_segment(buff->lines[line], splitted_line[0], 0) == -1) {
        return -1;
    }
    // Increment the line count if we are creating a new line at the end
    if (line == buff->line_count) {
        buff->line_count++;
    }

    for (int i = 1; i < splitted_line_count; i++) {
        if (insert_line(buff, splitted_line[i], line + i) == -1) {
            return -1;
        }
    }

    // Free memory and exit normally
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
    // Check if `line` is out of bound
    if (line < 0 || line >= buff->line_count) {
        return -1;
    }
    // Insert a line return at `line` and `col`
    if (line_insert_char(buff->lines[line], '\n', col) == -1) {
        return -1;
    }
    // Autosplit the line
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

    // Check if the lines are incorrect
    if (from_line < 0 || from_line > to_line || to_line >= buff->line_count) {
        return -1;
    }
    // Allocate the temporary string that will hold the joined lines
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
    // Append to the string the lines
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

    // Truncate/delete the lines
    line_truncate(buff->lines[from_line]);
    for (int i = 0; i < lines_to_delete; i++) {
        delete_line(buff, from_line + 1);
    }

    // Insert the string in the first line
    if (line_insert_segment(buff->lines[from_line], str, 0) == -1) {
        free(str);
        return -1;
    }
    // Free the temporary string and exit normally
    free(str);
    return 0;
}

int get_line_count(BUFFER * buff)
{
    return buff->line_count;
}

int get_line_length(BUFFER * buff, int line)
{
    return buff->lines[line]->length;
}

int get_buffer_length(BUFFER * buff)
{
    int buff_length = 0;

    for (int i = 0; i < buff->line_count; i++) {
        buff_length += buff->lines[i]->length + 1;  // Count the line returns
    }

    return buff_length;
}

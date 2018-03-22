#ifndef BUFFER_H
#define BUFFER_H

typedef struct BUFFER BUFFER;

BUFFER *new_buffer();
void free_buffer(BUFFER * buff);
char get_char(BUFFER * buff, int line, int col);
char *get_line(BUFFER * buff, int line);
char *get_text(BUFFER * buff, int from_line, int from_col,
               int to_line, int to_col);
int insert_char(BUFFER * buff, char c, int line, int col);
int insert_line(BUFFER * buff, char *str, int line);
int insert_text(BUFFER * buff, char *str, int line, int col);
int delete_char(BUFFER * buff, int line, int col);
int delete_line(BUFFER * buff, int line);
int delete_text(BUFFER * buff, int from_line, int from_col,
                int to_line, int to_col);
int override_char(BUFFER * buff, char c, int line, int col);
int override_line(BUFFER * buff, char *str, int line);
int override_text(BUFFER * buff, char *str, int from_line,
                  int from_col, int to_line, int to_col);
// Split the line at every occurencies of a line return
int autosplit_line(BUFFER * buff, int line);
int split_line_at(BUFFER * buff, int line, int col);
int join_lines(BUFFER * buff, int from_line, int to_line, int with_spaces);
int get_line_count(BUFFER * buff);
int get_line_length(BUFFER * buff, int line);
int get_buffer_length(BUFFER * buff);

#endif

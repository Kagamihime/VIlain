#ifndef BUFFER_H
#define BUFFER_H

typedef struct BUFFER BUFFER;

BUFFER *new_buffer();
void free_buffer(BUFFER * buff);
char get_char(BUFFER * buff, unsigned int line, unsigned int col);
char *get_line(BUFFER * buff, unsigned int line);
char *get_text(BUFFER * buff, unsigned int from_line, unsigned int from_col,
               unsigned int to_line, unsigned int to_col);
int insert_char(BUFFER * buff, char c, unsigned int line, unsigned int col);
int insert_line(BUFFER * buff, char *str, unsigned int line);
int insert_text(BUFFER * buff, char *str, unsigned int from_line,
                unsigned int from_col, unsigned int to_line,
                unsigned int to_col);
int override_char(BUFFER * buff, char c, unsigned int line, unsigned int col);
int override_line(BUFFER * buff, char *str, unsigned int line);
int override_text(BUFFER * buff, char *str, unsigned int from_line,
                  unsigned int from_col, unsigned int to_line,
                  unsigned int to_col);
int delete_char(BUFFER * buff, unsigned int line, unsigned int col);
int delete_line(BUFFER * buff, unsigned int line);
int delete_text(BUFFER * buff, unsigned int from_line, unsigned int from_col,
                unsigned int to_line, unsigned int to_col);
unsigned int get_line_count(BUFFER * buff);
unsigned int get_line_length(BUFFER * buff, unsigned int line);
unsigned int get_buffer_length(BUFFER * buff);

#endif

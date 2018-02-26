#include <stdlib.h>
#include <stdio.h>
#include "../include/buffer.h"

BUFFER *new_buffer()
{
    return NULL;
}

void free_buffer(BUFFER * buff)
{
}

char get_char(BUFFER * buff, int line, int col)
{
    return 'a';
}

char *get_line(BUFFER * buff, int line)
{
    return NULL;
}

char *get_text(BUFFER * buff, int from_line, int from_col, int to_line,
               int to_col)
{
    return NULL;
}

void insert_char(BUFFER * buff, char c, int line, int col)
{
}

void insert_line(BUFFER * buff, char *str, int line)
{
}

void insert_text(BUFFER * buff, char *str, int from_line, int from_col,
                 int to_line, int to_col)
{
}

void override_char(BUFFER * buff, char c, int line, int col)
{
}

void override_line(BUFFER * buff, char *str, int line)
{
}

void override_text(BUFFER * buff, char *str, int from_line, int from_col,
                   int to_line, int to_col)
{
}

void delete_char(BUFFER * buff, int line, int col)
{
}

void delete_line(BUFFER * buff, int line)
{
}

void delete_text(BUFFER * buff, int from_line, int from_col, int to_line,
                 int to_col)
{
}

int get_line_count(BUFFER * buff)
{
    return -1;
}

int get_line_length(BUFFER * buff)
{
    return -1;
}

int get_buffer_length(BUFFER * buff)
{
    return -1;
}

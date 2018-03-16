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

char get_char(BUFFER * buff, unsigned int line, unsigned int col)
{
    return 'a';
}

char *get_line(BUFFER * buff, unsigned int line)
{
    return NULL;
}

char *get_text(BUFFER * buff, unsigned int from_line, unsigned int from_col,
               unsigned int to_line, unsigned int to_col)
{
    return NULL;
}

int insert_char(BUFFER * buff, char c, unsigned int line, unsigned int col)
{
}

int insert_line(BUFFER * buff, char *str, unsigned int line)
{
}

int insert_text(BUFFER * buff, char *str, unsigned int from_line,
                unsigned int from_col, unsigned int to_line,
                unsigned int to_col)
{
    return 0;
}

int override_char(BUFFER * buff, char c, unsigned int line, unsigned int col)
{
}

int override_line(BUFFER * buff, char *str, unsigned int line)
{
}

int override_text(BUFFER * buff, char *str, unsigned int from_line,
                  unsigned int from_col, unsigned int to_line,
                  unsigned int to_col)
{
    return 0;
}

int delete_char(BUFFER * buff, unsigned int line, unsigned int col)
{
}

int delete_line(BUFFER * buff, int unsigned line)
{
}

int delete_text(BUFFER * buff, unsigned int from_line, unsigned int from_col,
                unsigned int to_line, unsigned int to_col)
{
    return 0;
}

unsigned int get_line_count(BUFFER * buff)
{
    return -1;
}

unsigned int get_line_length(BUFFER * buff, unsigned int line)
{
    return -1;
}

unsigned int get_buffer_length(BUFFER * buff)
{
    return -1;
}

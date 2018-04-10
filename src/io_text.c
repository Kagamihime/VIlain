#include <stdlib.h>
#include <stdio.h>
#include "../include/io_text.h"
#include "../include/buffer.h"
#define FILE_MAX_LENGTH 8192
#define LINE_MAX_LENGTH 512

struct IO_TEXT
{
	char ** buffer_text;
};

BUFFER *load_file(IO_TEXT * io_text, char *path)
{
    return NULL;
}

int save_buffer(IO_TEXT * io_text, char *path, BUFFER * buff)
{
    return -1;
}

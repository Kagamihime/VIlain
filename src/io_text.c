#include <stdlib.h>
#include <stdio.h>
#include "../include/io_text.h"
#include "../include/buffer.h"
#define FILE_MAX_LENGTH 8192
#define LINE_MAX_LENGTH 512

struct IO_TEXT {
    char **buffer_text;
};

BUFFER *load_file(IO_TEXT * io_text, char *path)
{
    int i = 0;
    BUFFER *res = new_buffer();
    char *line = malloc(sizeof(char *) * LINE_MAX_LENGTH);
    FILE *file;
    file = fopen(path, "r");
    while (fgets(line, FILE_MAX_LENGTH, file) != NULL) {
        insert_line(res, line, i);
        i++;
    }
    free(line);
    fclose(file);
    return res;
}

int save_buffer(IO_TEXT * io_text, char *path, BUFFER * buff)
{
    return -1;
}

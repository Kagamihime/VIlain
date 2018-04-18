#include <stdlib.h>
#include <stdio.h>
#include "../include/io_text.h"
#include "../include/buffer.h"
#define FILE_MAX_LENGTH 8192
#define LINE_MAX_LENGTH 512

BUFFER *load_file(char *path)
{
    int i = 0;
    BUFFER *res = new_buffer();
    char *line = malloc(sizeof(char *) * LINE_MAX_LENGTH);
    FILE *file = NULL;
    file = fopen(path, "r");
    if (file == NULL) {
        free(line);
        fclose(file);
        return NULL;
    }
    while (fgets(line, LINE_MAX_LENGTH, file) != NULL) {
        insert_line(res, line, i);
        i++;
    }
    free(line);
    fclose(file);
    return res;
}

int save_buffer(char *path, BUFFER * buff)
{
    FILE *file = NULL;
    file = fopen(path, "w");
    if (file == NULL) {
        fclose(file);
        return -1;
    }
    for (int i = 0; i < get_line_count(buff); i++) {
        if (fprintf(file, "%s", get_line(buff, i)) < 0) {
            fclose(file);
            return -1;
        }
    }
    fclose(file);
    return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include "../include/io_text.h"
#include "../include/buffer.h"

BUFFER *load_file(char *path)
{
    int i = 1;
    int j = 0;
    int c;
    BUFFER *res = new_buffer();
    insert_line(res, "", 0);
    FILE *file = NULL;
    file = fopen(path, "r");
    if (file == NULL) {
        fclose(file);
        return NULL;
    }
    do {
        c = fgetc(file);
        if (feof(file)) {
            break;
        }
        if (c == '\n') {
            insert_line(res, "", i);
            j = 0;
            i++;
        } else {
            insert_char(res, c, i, j);
            j++;
        }
    } while (1);
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

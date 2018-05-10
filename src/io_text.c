#include <stdio.h>
#include <stdlib.h>

#include "../include/buffer.h"
#include "../include/io_text.h"

BUFFER *load_file(char *path)
{
    FILE *file = NULL;
    BUFFER *res = NULL;
    int i = 0;
    int j = 0;
    int c = 0;

    if ((file = fopen(path, "r")) == NULL) {
        return NULL;
    }

    if ((res = new_buffer()) == NULL) {
        fclose(file);
        return NULL;
    }

    if (insert_line(res, "", 0) == -1) {
        free_buffer(res);
        fclose(file);
        return NULL;
    }

    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            if (insert_line(res, "", i + 1) == -1) {
                free_buffer(res);
                fclose(file);
                return NULL;
            }

            j = 0;
            i++;
        } else {
            if (insert_char(res, c, i, j) == -1) {
                free_buffer(res);
                fclose(file);
                return NULL;
            }

            j++;
        }
    }

    fclose(file);
    return res;
}

int save_buffer(char *path, BUFFER * buff)
{
    FILE *file = NULL;

    if ((file = fopen(path, "w")) == NULL) {
        return -1;
    }

    for (int i = 0; i < get_line_count(buff); i++) {
        char *str = NULL;

        if ((str = get_line(buff, i)) == NULL) {
            fclose(file);
            return -1;
        }

        if (fprintf(file, "%s", str) < 0) {
            fclose(file);
            free(str);
            return -1;
        }

        free(str);
    }

    fclose(file);
    return 0;
}

#ifndef IO_TEXT_H
#define IO_TEXT_H

#include "../include/buffer.h"

BUFFER *load_file(char *path);
int save_buffer(char *path, BUFFER * buff);

#endif

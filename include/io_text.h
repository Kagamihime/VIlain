#ifndef IO_TEXT_H
#define IO_TEXT_H

typedef struct IO_TEXT IO_TEXT;
typedef struct BUFFER BUFFER;

BUFFER *load_file(IO_TEXT * io_text, char *path);
int save_buffer(IO_TEXT * io_text, char *path, BUFFER * buff);

#endif

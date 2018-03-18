#ifndef CURSOR_H
#define CURSOR_H

typedef struct CURSOR CURSOR;

CURSOR *new_curs();
int get_pos_x(CURSOR * curs);
int get_pos_y(CURSOR * curs);
void set_pos_x(CURSOR * curs, int i);
void set_pos_y(CURSOR * curs, int i);

#endif

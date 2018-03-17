#include <stdlib.h>
#include <stdio.h>
#include "../include/cursor.h"

struct CURSOR {
    int x;                      //the horizontal position of the cursor
    int y;                      //the vertical position of the cursor
};
int get_pos_x(struct CURSOR *curs)
{
    return curs->x;
}

int get_pos_y()
{
    return -1;
}

void set_pos_x()
{
}

void set_pos_y()
{
}

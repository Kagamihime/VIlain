#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include "../include/buffer.h"
#include "../include/cursor.h"
#include "../include/settings.h"
#include "../include/io_text.h"
#include "../include/ui.h"

int main(int argc, char *argv[])
{
    //Initialize
    struct BUFFER *buff;
    buff = new_buffer();
    insert_line(buff, "", 0);
    
    //Launch VIlain with a new file
    if (argc == 1) {
        initscr();
        exec_user_action(buff);
        endwin();
    }

    return 0;
}

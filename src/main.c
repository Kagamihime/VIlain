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
    //Launch VIlain with an existing file
    else if (argc == 2) {
        buff = load_file(argv[1]);
        if (buff == NULL) {
            printf("ERROR: the file %s does not exist.\n", argv[1]);
        } else {
            initscr();
            exec_user_action(buff);
            endwin();
        }
    } else {
        printf("Error: expecting 0 or 1 arguments.");
    }

    return 0;
}

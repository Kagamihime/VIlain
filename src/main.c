#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include "../include/buffer.h"
#include "../include/cursor.h"
#include "../include/settings.h"
#include "../include/io_text.h"
#include "../include/ui.h"

int main(void)
{
    struct BUFFER *buff;
    buff = new_buffer();
    char *str1 = "Hello world";
    char *str2 = "Hello jack";
    char *str3 = "Hello billy";

    insert_line(buff, str1, 0);
    insert_line(buff, str2, 1);
    insert_line(buff, str3, 2);

    initscr();
    exec_user_action(buff);
    getch();
    endwin();

    return 0;
}

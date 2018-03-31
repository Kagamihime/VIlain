#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include "../include/buffer.h"
#include "../include/ui.h"
#include "../include/settings.h"
#include "../include/cursor.h"

#define TEXT_WIDTH 80
#define TEXT_HEIGHT 30
#define MENU_WIDTH 30
#define MENU_HEIGHT 9
WINDOW *text_win;
WINDOW *menu_win;
WINDOW *shortcuts_win;
CURSOR *curs;
char *menu_choices[] = {
    "Save file",
    "Open file",
    "Shortcuts options",
    "LineWrapping option",
    "Exit",
};

int number_menu_choices = sizeof(menu_choices) / sizeof(char *);
void print_menu(WINDOW * menu_win, int highlight);

void print_text(BUFFER * buff, unsigned int first_line, unsigned int first_col)
{
    for (int i = first_line; i < get_line_count(buff); i++) {
        if (i >= get_line_length(buff, i)) {
            move(i, 0);
            clrtoeol();
        } else {
            mvwprintw(text_win, i, 0, get_line(buff, i));
        }
        clrtoeol();
    }
}

void print_wrapped_text(BUFFER * buff, unsigned int first_line)
{
}

void exec_user_action(BUFFER * buff)
{
    //Creation of the window and the cursor
    text_win = newwin(TEXT_HEIGHT, TEXT_WIDTH, 0, 0);
    curs = new_curs();

    //Print the current buffer and  place the cursor at the end
    print_text(buff, 0, 0);
    set_pos_y(curs, get_line_count(buff) - 1);
    set_pos_x(curs, get_line_length(buff, get_line_count(buff) - 1));
    wmove(text_win, get_pos_y(curs), get_pos_x(curs));

    //Set up the parameters to listen to keyboard events and enter the loop
    keypad(text_win, TRUE);
    int ch;
    char *tmp;
    noecho();
    int exit = 0;

    // Wait for the user to press a key
    while (!exit) {
        ch = wgetch(text_win);
        switch (ch) {
        case KEY_UP:           //UP ARROW: move cursor up
            if (get_pos_y(curs) > 0)
                set_pos_y(curs, get_pos_y(curs) - 1);
            break;
        case KEY_DOWN:         //DOWN ARROW: move cursor down
            if (get_pos_y(curs) < TEXT_HEIGHT)
                set_pos_y(curs, get_pos_y(curs) + 1);
            break;
        case KEY_RIGHT:        //RIGHT ARROW: move cursor right
            if (get_pos_x(curs) < TEXT_WIDTH)
                set_pos_x(curs, get_pos_x(curs) + 1);
            break;
        case KEY_LEFT:         //LEFT ARROW: move cursor left
            if (get_pos_x(curs) > 0)
                set_pos_x(curs, get_pos_x(curs) - 1);
            break;

        case 10:               //ENTER: create a new line containing the end of the current line
            tmp =
                get_text(buff, get_pos_y(curs), get_pos_x(curs),
                         get_pos_y(curs), get_line_length(buff,
                                                          get_pos_x(curs)));
            delete_text(buff, get_pos_y(curs), get_pos_x(curs), get_pos_y(curs),
                        get_line_length(buff, get_pos_x(curs)));
            set_pos_x(curs, 0);
            set_pos_y(curs, get_pos_y(curs) + 1);

            insert_line(buff, tmp, get_pos_y(curs));
            break;
        case KEY_BACKSPACE:    // BACKSPACE
            if (get_pos_x(curs) > 0) {
                set_pos_x(curs, get_pos_x(curs) - 1);
                delete_char(buff, get_pos_y(curs), get_pos_x(curs));
            }

            break;

        case 16:               //Ctrl+P: display the options menu
            clear();
            refresh();
            settings_menu(NULL);
            break;
            // TODO: SHORTCUTS
        case 27:               //ESCAPE : exit the program
            exit = 1;
            break;
        default:
            insert_char(buff, ch, get_pos_y(curs), get_pos_x(curs));
            set_pos_x(curs, get_pos_x(curs) + 1);
            break;
        }
        print_text(buff, 0, 0);
        wmove(text_win, get_pos_y(curs), get_pos_x(curs));
        if (exit == 1)
            break;
    }
}

void select_text(BUFFER * buff, CURSOR * curs)
{
}

void print_status_bar(char *str)
{
}

void loading_menu()
{
}

void saving_menu()
{
}

void settings_menu(SETTINGS * set)
{
}

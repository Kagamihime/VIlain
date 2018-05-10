#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>
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

char *tmp;
int number_menu_choices = sizeof(menu_choices) / sizeof(char *);

void print_menu(WINDOW * menu_win, int highlight, char **choices,
                int number_choices)
{
    //Initialize
    int x, y, i;
    x = 2;
    y = 2;

    //Print the menu and highlight the current choice
    box(menu_win, 0, 0);
    for (i = 0; i < number_choices; ++i) {
        if (highlight == i + 1) {   /* High light the present choice */
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        } else
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        ++y;
    }
    wrefresh(menu_win);
}

int move_in_menu(WINDOW * menu_win, int highlight, char **menu_choices,
                 int number_choices)
{
    //Initialize
    int ch;
    int choice = 0;

    //Wait for the user to press a key
    while (1) {
        ch = wgetch(menu_win);
        switch (ch) {
        case KEY_UP:
            if (highlight == 1) //if in first choice go to last choice
                highlight = number_choices;
            else
                --highlight;    //else go to previous choice
            break;
        case KEY_DOWN:         //if in last choice go to first choice
            if (highlight == number_choices)
                highlight = 1;
            else
                ++highlight;    //else go to next choice
            break;
        case 10:               //ENTER: confirm choice
            choice = highlight;
            break;
        default:
            break;
        }

        //Re-print the menu and change the highlight
        print_menu(menu_win, highlight, menu_choices, number_choices);

        // Check if the user did a choice
        if (choice != 0)
            return choice;
    }
};

void shortcuts_menu(SETTINGS * sets)
{
    //Initialize the two windows
    int startx = (80 - SHORTCUTS_WIDTH) / 2;
    int starty = (24 - SHORTCUTS_HEIGHT) / 2;
    int startxx = startx + SHORTCUTS_WIDTH - 1;
    WINDOW *shortcuts_win =
        newwin(SHORTCUTS_HEIGHT, SHORTCUTS_WIDTH, starty, startx);
    WINDOW *keys_win =
        newwin(SHORTCUTS_HEIGHT, SHORTCUTS_WIDTH - 10, starty, startxx);
    char *shortcuts[] = {
        "Text Selection",
        "Cut",
        "Copy",
        "Paste",
        "Options",
        "Save file",
        "Load file",
        "Exit"
    };
    char *keys[] = {
        to_string(get_toogle_selection_shortcut(sets)),
        to_string(get_cut_shortcut(sets)),
        to_string(get_copy_shortcut(sets)),
        to_string(get_paste_shortcut(sets)),
        to_string(get_settings_shortcut(sets)),
        to_string(get_save_shortcut(sets)),
        to_string(get_load_shortcut(sets))
    };
    int number_s = sizeof(shortcuts) / sizeof(char *);
    int number_k = sizeof(keys) / sizeof(char *);
    int highlight = 1;
    int ch;
    mvprintw(5, 19, "Choose the shortcut you want to edit and press ENTER.");

    //Print the menu
    keypad(shortcuts_win, TRUE);
    refresh();
    print_menu(shortcuts_win, highlight, shortcuts, number_s);
    print_menu(keys_win, 0, keys, number_k);

    //Wait for the user to make a choice in the menu
    int choice = move_in_menu(shortcuts_win, highlight, shortcuts, number_s);

    //Erase eventual message on line 23
    move(23, 0);
    clrtoeol();

    //Give instructions to user
    move(5, 0);
    clrtoeol();
    mvprintw(5, 21, "Press the new shortcut by holding Ctrl + a letter.");

    //Wait for the new shortcut and check if it is valide
    if (choice != 8) {          //If not exit
        ch = getch();
        if (ch > 0 && ch < 27) {
            if (!isCurrentShortcut(sets, ch)) {
                switch (choice) {
                case 1:
                    set_toogle_selection_shortcut(sets, ch);
                    break;
                case 2:
                    set_cut_shortcut(sets, ch);
                    break;
                case 3:
                    set_copy_shortcut(sets, ch);
                    break;
                case 4:
                    set_paste_shortcut(sets, ch);
                    break;
                case 5:
                    set_settings_shortcut(sets, ch);
                    break;
                case 6:
                    set_save_shortcut(sets, ch);
                    break;
                case 7:
                    set_load_shortcut(sets, ch);
                    break;
                }
                mvprintw(23, 0, "Shortcut saved.");
            } else {
                mvprintw(23, 0,
                         "This shortcut is already used for another shortcut.");
            }
        } else {
            mvprintw(23, 0, "This shortcut is not valid.");
        }

        //Change the shortcut and save it in ui.cfg
        save(sets, "./etc/ui.cfg");
    }
    //Refresh the menu or leave the function
    if (choice != 8)
        shortcuts_menu(sets);
    else {
        clear();
        refresh();
    }

}

void settings_menu(SETTINGS * sets)
{
    // Initialize
    char *menu_choices[] = {
        "Save file",
        "Open file",
        "See shortcuts",
        "LineWrapping option",
        "Exit",
    };
    int number_choices = sizeof(menu_choices) / sizeof(char *);
    int highlight = 1;

    //TODO: change the values here
    int startx = (80 - MENU_WIDTH) / 2;
    int starty = (24 - MENU_HEIGHT) / 2;
    menu_win = newwin(MENU_HEIGHT, MENU_WIDTH, starty, startx);

    //Print the menu
    keypad(menu_win, TRUE);
    refresh();
    print_menu(menu_win, highlight, menu_choices, number_choices);

    //wait for the user to make a choice in the menu
    int choice =
        move_in_menu(menu_win, highlight, menu_choices, number_choices);

    // Erase the menu
    //DEBUG:
    //mvprintw(23, 0, "You chose: %d %s\n", choice, menu_choices[choice - 1]);
    //getch();
    clear();
    refresh();

    // Execute the action corresponding to the choice
    //DEBUG:
    //mvprintw(23, 0, "You chose: %d %s\n", choice, menu_choices[choice - 1]);
    switch (choice) {
    case 1:
        break;
    case 2:
        break;
    case 3:
        shortcuts_menu(sets);
        break;
    case 4:
        break;
    case 5:
        break;
    }
}

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

void move_cursor(BUFFER * buff, CURSOR * curs, int ch)
{
    switch (ch) {
    case KEY_UP:               //UP ARROW: move cursor up
        if (get_pos_y(curs) > 0)
            set_pos_y(curs, get_pos_y(curs) - 1);
        if (get_pos_x(curs) > get_line_length(buff, get_pos_y(curs)))
            set_pos_x(curs, get_line_length(buff, get_pos_y(curs)));
        break;
    case KEY_DOWN:             //DOWN ARROW: move cursor down
        if (get_pos_y(curs) < get_line_count(buff) - 1)
            set_pos_y(curs, get_pos_y(curs) + 1);
        if (get_pos_x(curs) > get_line_length(buff, get_pos_y(curs)))
            set_pos_x(curs, get_line_length(buff, get_pos_y(curs)));
        break;
    case KEY_RIGHT:            //RIGHT ARROW: move cursor right
        if (get_pos_x(curs) < get_line_length(buff, get_pos_y(curs)))
            set_pos_x(curs, get_pos_x(curs) + 1);
        break;
    case KEY_LEFT:             //LEFT ARROW: move cursor left
        if (get_pos_x(curs) > 0)
            set_pos_x(curs, get_pos_x(curs) - 1);
        break;
    }
    print_status_bar(buff, " ");
}

void exec_user_action(BUFFER * buff)
{
    //Create the window, the cursor and charge the settings
    text_win = newwin(TEXT_HEIGHT, TEXT_WIDTH, 0, 0);
    curs = new_curs();
    SETTINGS *sets = new_sets();

    //Print the current buffer and  place the cursor at the end
    print_text(buff, 0, 0);
    set_pos_y(curs, get_line_count(buff) - 1);
    set_pos_x(curs, get_line_length(buff, get_line_count(buff) - 1));
    print_status_bar(buff, " ");

    //Set up the parameters to listen to keyboard events and enter the loop
    keypad(text_win, TRUE);
    int ch;
    noecho();
    int exit = 0;

    // Wait for the user to press a key
    while (!exit) {
        ch = wgetch(text_win);

        //Moving the cursor
        if (ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT || ch == KEY_RIGHT) {
            move_cursor(buff, curs, ch);
        }
        //BACKSPACE
        else if (ch == KEY_BACKSPACE) {
            if (get_pos_x(curs) > 0) {
                set_pos_x(curs, get_pos_x(curs) - 1);
                delete_char(buff, get_pos_y(curs), get_pos_x(curs));
            } else {
                //TODO: concatener cette ligne avec celle d'avant
            }
        }
        //ESCPAPE: exit the program
        else if (ch == 27) {
            exit = 1;
        }
        //Display the settings menu
        else if (ch == (get_settings_shortcut(sets))) {
            clear();
            refresh();
            curs_set(0);        //hide the cursor
            settings_menu(sets);
            curs_set(1);        //show the cursor
        }
        //Text selection
        else if (ch == (get_toogle_selection_shortcut(sets))) {
            curs_set(0);        //hide the cursor
            select_text(buff, curs);
            curs_set(1);        //show the cursor
        }
        //Paste the text in tmp
        else if (ch == (get_paste_shortcut(sets))) {
            if (tmp != NULL) {
                insert_text(buff, tmp, get_pos_y(curs), get_pos_x(curs));
            }
            print_status_bar(buff, "");
        }
        //Write in the buffer
        else {
            insert_char(buff, ch, get_pos_y(curs), get_pos_x(curs));
            set_pos_x(curs, get_pos_x(curs) + 1);
        }

        //Update window
        print_text(buff, 0, 0);
        wmove(text_win, get_pos_y(curs), get_pos_x(curs));
        print_status_bar(buff, " ");
        if (exit == 1)
            break;
    }
}

char *strdup(const char *str)
{
    int n = strlen(str) + 1;
    char *dup = malloc(n);
    if (dup) {
        strcpy(dup, str);
    }
    return dup;
}

void select_text(BUFFER * buff, CURSOR * curs)
{
    //Initialize
    char *text;
    int first_x = get_pos_x(curs);
    int first_y = get_pos_y(curs);
    int from_x, from_y, to_x, to_y;
    int ch;
    int exit = 0;

    //Highlight the char in the cursor

    wattron(text_win, A_REVERSE);
    mvwprintw(text_win, first_y, first_x, "%c",
              get_char(buff, first_y, first_x));
    wattroff(text_win, A_REVERSE);

    //Wait for the user to press a key
    while (!exit) {
        ch = wgetch(text_win);

        //Move the cursor
        if (ch == KEY_UP || ch == KEY_LEFT || ch == KEY_DOWN || ch == KEY_RIGHT) {
            move_cursor(buff, curs, ch);
            if (first_y < get_pos_y(curs)
                || (first_y == get_pos_y(curs) && first_x < get_pos_x(curs))) {
                from_x = first_x;
                from_y = first_y;
                to_x = get_pos_x(curs);
                to_y = get_pos_y(curs);
            } else {
                from_x = get_pos_x(curs);
                from_y = get_pos_y(curs);
                to_x = first_x;
                to_y = first_y;
            }
            text = get_text(buff, from_y, from_x, to_y, to_x);
            //DEBUG: mvwprintw(text_win,10,0,"From %d:%d  to  %d:%d    ", from_y,from_x, to_y,to_x);
            //DEBUG: mvwprintw(text_win,10,0,"%s     ", text);
            if (text != NULL) {
                print_text(buff, 0, 0);
                //print in highlight the selected text
                wattron(text_win, A_REVERSE);
                mvwprintw(text_win, from_y, from_x, "%s", text);
                wattroff(text_win, A_REVERSE);
            }
        }
    }
}

void print_status_bar(BUFFER * buff, char *str)
{
    mvwprintw(text_win, TEXT_HEIGHT - 1, 0, "%d/%d : %d/%d       ",
              get_pos_y(curs), get_line_count(buff), get_pos_x(curs),
              get_line_length(buff, get_pos_y(curs)));
    mvwprintw(text_win, TEXT_HEIGHT - 1, TEXT_WIDTH / 2, "%s       ", str);
    wmove(text_win, get_pos_y(curs), get_pos_x(curs));
}

void loading_menu()
{
}

void saving_menu()
{
}

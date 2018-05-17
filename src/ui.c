#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include "../include/buffer.h"
#include "../include/ui.h"
#include "../include/settings.h"
#include "../include/cursor.h"
#include "../include/io_text.h"

#define TEXT_WIDTH 80
#define TEXT_HEIGHT 30
#define MENU_WIDTH 30
#define MENU_HEIGHT 9
#define SHORTCUTS_WIDTH 25
#define SHORTCUTS_HEIGHT 12
#define AFM_HEIGHT 6
#define AFM_WIDTH 20

BUFFER *buff;
WINDOW *text_win;
WINDOW *shortcuts_win;
CURSOR *curs;

int scrollx;
int scrolly;
int auto_fill_mode;
char *tmp;

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

void cut_long_lines()
{
    for (int i = 0; i < get_line_count(buff); i++) {
        if (get_line_length(buff, i) > TEXT_WIDTH - 1)
            split_line_at(buff, i, TEXT_WIDTH - 1);
    }
}

void auto_fill_mode_menu(SETTINGS * sets)
{
    // Initialize
    char *afm_choices[] = {
        "Yes",
        "No",
    };
    int number_choices = sizeof(afm_choices) / sizeof(char *);
    int highlight = 1;

    //TODO: change the values here
    int startx = (80 - AFM_WIDTH) / 2;
    int starty = (24 - AFM_HEIGHT) / 2;
    WINDOW *afm_win = newwin(AFM_HEIGHT, AFM_WIDTH, starty, startx);

    //Prin menu and informations
    keypad(afm_win, TRUE);
    mvprintw(6, 23, "Enable auto-fill mode ?");
    mvprintw(17, 8,
             "When auto-fill mode is enabled the lines will be cut at the end of the window");
    mvprintw(18, 8, "by automatically adding a line return.");
    mvprintw(20, 8,
             "WARNING: Enable auto-fill mode will cut the lines that are beyond the window");
    mvprintw(21, 17, "limit in your current file.");
    refresh();
    print_menu(afm_win, highlight, afm_choices, number_choices);

    //wait for the user to make a choice in the menu
    int choice = move_in_menu(afm_win, highlight, afm_choices, number_choices);

    // Erase the menu
    clear();
    refresh();

    // Execute the action corresponding to the choice
    switch (choice) {
    case 1:
        cut_long_lines();
        auto_fill_mode = 1;
        set_auto_fill_mode(sets, 1);
        scrollx = 0;
        break;
    case 2:
        auto_fill_mode = 0;
        set_auto_fill_mode(sets, 0);
        break;
    }
    save(sets, "./etc/ui.cfg");
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
            if (!is_current_shortcut(sets, ch)) {
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
        "Auto-fill mode",
        "Exit",
    };
    int number_choices = sizeof(menu_choices) / sizeof(char *);
    int highlight = 1;
    char *path = NULL;

    //TODO: change the values here
    int startx = (80 - MENU_WIDTH) / 2;
    int starty = (24 - MENU_HEIGHT) / 2;
    WINDOW *menu_win = newwin(MENU_HEIGHT, MENU_WIDTH, starty, startx);

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
        clear();
        refresh();
        curs_set(1);
        if ((path = malloc(60 * sizeof(char))) != NULL) {
            saving_file(buff, path);
        }
        clear();
        refresh();
        break;
    case 2:
        clear();
        refresh();
        curs_set(1);
        if ((path = malloc(60 * sizeof(char))) != NULL) {
            loading_file(path);
        }
        clear();
        refresh();
        break;
    case 3:
        shortcuts_menu(sets);
        break;
    case 4:
        clear();
        refresh();
        auto_fill_mode_menu(sets);
        break;
    case 5:
        break;
    }
    free(path);
}

void print_text(BUFFER * buff, unsigned int first_line, unsigned int first_col)
{
    clear();
    if (get_line_count(buff) > TEXT_HEIGHT - 1) {
        for (int i = 0; i < TEXT_HEIGHT - 1; i++) {
            mvwprintw(text_win, i, 0,
                      get_line(buff, i + first_line) + first_col);
            clrtoeol();
        }
    } else {
        int y = 0;
        for (int i = first_line; i < get_line_count(buff); i++) {
            mvwprintw(text_win, y, 0, get_line(buff, i));
            y++;
            clrtoeol();
        }
    }
}

void move_cursor(BUFFER * buff, CURSOR * curs, int ch)
{
    switch (ch) {
    case KEY_UP:               //UP ARROW: move cursor up
        if (get_pos_y(curs) > 0)
            set_pos_y(curs, get_pos_y(curs) - 1);
        if (get_pos_x(curs) >
            get_line_length(buff, get_pos_y(curs) + scrolly) - scrollx)
            set_pos_x(curs,
                      get_line_length(buff,
                                      get_pos_y(curs) + scrolly) - scrollx);
        break;
    case KEY_DOWN:             //DOWN ARROW: move cursor down
        if (get_pos_y(curs) < get_line_count(buff) - 1
            && get_pos_y(curs) < TEXT_HEIGHT - 2)
            set_pos_y(curs, get_pos_y(curs) + 1);
        if (get_pos_x(curs) >
            get_line_length(buff, get_pos_y(curs) + scrolly) - scrollx)
            set_pos_x(curs,
                      get_line_length(buff,
                                      get_pos_y(curs) + scrolly) - scrollx);
        break;
    case KEY_RIGHT:            //RIGHT ARROW: move cursor right
        if (get_pos_x(curs) <
            get_line_length(buff, get_pos_y(curs) + scrolly) - scrollx)
            set_pos_x(curs, get_pos_x(curs) + 1);
        else if (get_pos_y(curs) < get_line_count(buff) - 1 && scrollx == 0
                 && get_pos_y(curs) < TEXT_HEIGHT - 2) {
            set_pos_y(curs, get_pos_y(curs) + 1);
            set_pos_x(curs, 0);
        }
        break;
    case KEY_LEFT:             //LEFT ARROW: move cursor left
        if (get_pos_x(curs) > 0)
            set_pos_x(curs, get_pos_x(curs) - 1);
        else if (get_pos_y(curs) > 0
                 && get_line_length(buff,
                                    get_pos_y(curs) + scrolly - 1) <
                 TEXT_WIDTH - 1) {
            set_pos_y(curs, get_pos_y(curs) - 1);
            set_pos_x(curs, get_line_length(buff, get_pos_y(curs) + scrolly));
        }
        break;
    }
    print_status_bar(buff, " ");
}

int scroll_win(int ch)
{
    if (ch == KEY_DOWN && get_pos_y(curs) == TEXT_HEIGHT - 2
        && scrolly < get_line_count(buff) - TEXT_HEIGHT + 1) {
        scrolly++;
        return 1;
    } else if (ch == KEY_UP && get_pos_y(curs) == 0 && scrolly > 0) {
        scrolly--;
        return 1;
    } else if (ch == KEY_RIGHT && get_pos_x(curs) == TEXT_WIDTH - 1
               && scrollx < get_line_length(buff,
                                            get_pos_y(curs) + scrolly) -
               TEXT_WIDTH + 1) {
        scrollx++;
        return 1;
    } else if (ch == KEY_LEFT && get_pos_x(curs) == 0 && scrollx > 0) {
        scrollx--;
        return 1;
    } else
        return 0;
}

void exec_user_action(BUFFER * bu)
{
    //Create the window, the cursor and charge the settings
    text_win = newwin(TEXT_HEIGHT, TEXT_WIDTH, 0, 0);
    curs = new_curs();
    curs_set(1);
    SETTINGS *sets = new_sets();

    //Print the current buffer and  place the cursor at the end
    buff = bu;
    print_text(buff, scrolly, scrollx);
    if (get_line_count(buff) < TEXT_HEIGHT - 2)
        set_pos_y(curs, get_line_count(buff) - 1);
    else
        set_pos_y(curs, TEXT_HEIGHT - 2);
    set_pos_x(curs, get_line_length(buff, get_line_count(buff) + scrolly - 1));
    print_status_bar(buff, " ");

    //Set up the parameters to listen to keyboard events and enter the loop
    keypad(text_win, TRUE);
    scrolly = 0;
    scrollx = 0;
    int ch;
    noecho();
    int exit = 0;

    // Wait for the user to press a key
    while (!exit) {
        ch = wgetch(text_win);

        //Moving the cursor
        if (ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT || ch == KEY_RIGHT) {
            //Scroll
            int sc = scroll_win(ch);

            //move the cursor
            if (!sc)
                move_cursor(buff, curs, ch);
        }
        //BACKSPACE
        else if (ch == KEY_BACKSPACE) {
            if (get_pos_x(curs) > 0) {
                set_pos_x(curs, get_pos_x(curs) - 1);
                delete_char(buff, get_pos_y(curs) + scrolly,
                            get_pos_x(curs) + scrollx);
            } else if (get_pos_y(curs) + scrolly > 0) {
                set_pos_x(curs,
                          get_line_length(buff, get_pos_y(curs) - 1 + scrolly));
                if (get_line_length(buff, get_pos_y(curs) + scrolly) == 0)
                    delete_line(buff, get_pos_y(curs) + scrolly);
                else if (get_line_length(buff, get_pos_y(curs) + scrolly - 1) ==
                         0)
                    delete_line(buff, get_pos_y(curs) + scrolly - 1);
                else
                    join_lines(buff, get_pos_y(curs) + scrolly - 1,
                               get_pos_y(curs) + scrollx, 1);
                set_pos_y(curs, get_pos_y(curs) - 1);
            }
        }
        //ENTER
        else if (ch == 10) {
            if (get_pos_x(curs) <
                get_line_length(buff, get_pos_y(curs) + scrolly))
                split_line_at(buff, get_pos_y(curs) + scrolly,
                              get_pos_x(curs) + scrollx);
            else
                insert_line(buff, "", get_pos_y(curs) + scrolly + 1);
            set_pos_x(curs, 0);
            if (get_pos_y(curs) < TEXT_HEIGHT - 2)
                set_pos_y(curs, get_pos_y(curs) + 1);
            else
                scrolly++;
            scrollx = 0;
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
            select_text(buff, curs, sets);
            curs_set(1);        //show the cursor
        }
        //Paste the text in tmp
        else if (ch == (get_paste_shortcut(sets))) {
            if (tmp != NULL) {
                insert_text(buff, tmp, get_pos_y(curs) + scrolly,
                            get_pos_x(curs) + scrollx);
            }
            print_status_bar(buff, "");
        }
        //Display the save menu
        else if (ch == (get_save_shortcut(sets))) {
            clear();
            refresh();
            char *path = NULL;
            if ((path = malloc(60 * sizeof(char))) != NULL) {
                saving_file(buff, path);
            }
            clear();
            refresh();
        }
        //Display the load menu
        else if (ch == (get_load_shortcut(sets))) {
            clear();
            refresh();
            char *path = NULL;
            if ((path = malloc(60 * sizeof(char))) != NULL) {
                loading_file(path);
            }
            clear();
            refresh();
        }
        //Write in the buffer
        else {
            insert_char(buff, ch, get_pos_y(curs) + scrolly,
                        get_pos_x(curs) + scrollx);
            if (scrollx < 1 && get_pos_x(curs) < TEXT_WIDTH - 1)
                set_pos_x(curs, get_pos_x(curs) + 1);
            else
                scrollx++;
        }

        //Update window
        print_text(buff, scrolly, scrollx);
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

void select_text(BUFFER * buff, CURSOR * curs, SETTINGS * sets)
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
                from_x = first_x + scrollx;
                from_y = first_y + scrolly;
                to_x = get_pos_x(curs) + scrollx;
                to_y = get_pos_y(curs) + scrolly;
            } else {
                from_x = get_pos_x(curs) + scrollx;
                from_y = get_pos_y(curs) + scrolly;
                to_x = first_x + scrollx;
                to_y = first_y + scrolly;
            }
            text = get_text(buff, from_y, from_x, to_y, to_x);
            //DEBUG: mvwprintw(text_win,10,0,"From %d:%d  to  %d:%d    ", from_y,from_x, to_y,to_x);
            //DEBUG: mvwprintw(text_win,10,0,"%s     ", text);
            if (text != NULL) {
                print_text(buff, scrolly, scrollx);
                //print in highlight the selected text
                wattron(text_win, A_REVERSE);
                mvwprintw(text_win, from_y, from_x, "%s", text);
                wattroff(text_win, A_REVERSE);
            }
        }
        //Cut
        else if (ch == get_cut_shortcut(sets)) {
            tmp = strdup(text);
            delete_text(buff, from_y + scrolly, from_x + scrollx,
                        to_y + scrolly, to_x + scrollx);
            exit = 1;
            print_status_bar(buff, "");
        }
        //Copy
        else if (ch == get_copy_shortcut(sets)) {
            tmp = strdup(text);
            exit = 1;
            print_status_bar(buff, "");
        }
        //Text_selection = exit
        else if (ch == get_toogle_selection_shortcut(sets)) {
            exit = 1;
        }
    }
}

void print_status_bar(BUFFER * buff, char *str)
{
    move(TEXT_HEIGHT - 1, 0);
    clrtoeol();
    refresh();
    mvwprintw(text_win, TEXT_HEIGHT - 1, TEXT_WIDTH - 16,
              "%d/%d : %d/%d       ", get_pos_y(curs) + scrolly + 1,
              get_line_count(buff), get_pos_x(curs) + scrollx + 1,
              get_line_length(buff, get_pos_y(curs) + scrolly) + 1);
    mvwprintw(text_win, TEXT_HEIGHT - 1, TEXT_WIDTH / 2, "%s       ", str);
    wmove(text_win, get_pos_y(curs), get_pos_x(curs));
}

void append(char *s, char c)
{
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
}

void loading_file(char *path)
{
    //Initialize
    int height = 3;
    int width = 60;
    WINDOW *load_win = newwin(height, width, 7, 10);
    strcpy(path, "./files/");
    int exit = 0;

    //Draw
    box(load_win, 0, 0);
    mvprintw(5, 11, "Enter file name:");
    mvprintw(4, 11,
             "Warning: Opening a file will erase your current changes if they have not been saved");
    mvprintw(16, 11, "Press ESCAPE to leave.");
    move(8, 11);
    printw(path);
    wrefresh(load_win);
    refresh();

    //Wait for the user to enter the path
    while (!exit) {
        int ch = getch();
        if ((ch > 45 && ch < 58)    //number or / or .
            || (ch == 95 || ch == 45)   //_ or -
            || (ch > 64 && ch < 91) //maj
            || (ch > 96 && ch < 123))   //min
        {
            append(path, ch);
            printw("%c", ch);
        } else if (ch == 10)    //ENTER
        {
            exit = 1;
        } else if (ch == 127) { //DELETE
            path[strlen(path) - 1] = '\0';
            move(8, 11);
            clrtoeol();
            printw("%s", path);
        } else if (ch == 27) {  //ESCAPE
            exit = 2;
        }
        //Load the file
        if (exit == 1) {
            BUFFER *buff_tmp = new_buffer();
            insert_line(buff_tmp, "", 0);
            buff_tmp = load_file(path);
            if (buff_tmp == NULL) {
                move(15, 0);
                clrtoeol();
                mvprintw(15, 11, "This file doesn't exist.");
                getch();
            } else {
                buff = load_file(path);
                set_pos_y(curs, get_line_count(buff) - 1);
                set_pos_x(curs,
                          get_line_length(buff, get_line_count(buff) - 1));
            }
        }
    }
    free(path);
}

void saving_file(BUFFER * buff, char *path)
{
    //Initialize
    int height = 3;
    int width = 60;
    WINDOW *save_win = newwin(height, width, 7, 10);
    strcpy(path, "./files/");
    int exit = 0;
    int len = 6;

    //Draw
    box(save_win, 0, 0);
    mvprintw(5, 11, "Enter file name:");
    mvprintw(16, 11, "Press ESCAPE to leave.");
    move(8, 11);
    printw(path);
    wrefresh(save_win);
    refresh();

    //Wait for the user to enter the path
    while (!exit) {
        int ch = getch();
        if (((ch > 45 && ch < 58)   //number or / or .
             || (ch == 95 || ch == 45)  //_ or -
             || (ch > 64 && ch < 91)    //maj
             || (ch > 96 && ch < 123))
            && len < 55)        //min
        {
            append(path, ch);
            printw("%c", ch);
            len++;
        } else if (ch == 10)    //ENTER
        {
            exit = 1;
        } else if (ch == 127) { //DELETE
            path[strlen(path) - 1] = '\0';
            move(8, 11);
            clrtoeol();
            printw("%s", path);
            len--;
        } else if (ch == 27) {  //ESCAPE
            exit = 2;
        }
    }

    //Save the file
    if (exit == 1) {
        int a = save_buffer(path, buff);
        if (a == -1) {
            move(15, 0);
            clrtoeol();
            mvprintw(15, 11, "Impossible to save this file.");
        } else {
            move(15, 0);
            clrtoeol();
            mvprintw(15, 11, "File Saved.");
        }
        getch();
    }
    free(path);
}

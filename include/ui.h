#ifndef UI_H
#define UI_H

#include <ncurses.h>

typedef struct CURSOR CURSOR;
typedef struct SETTINGS SETTINGS;

void print_text(BUFFER * buff, unsigned int first_line, unsigned int first_col);
void print_wrapped_text(BUFFER * buff, unsigned int first_line);
void move_cursor(BUFFER * buff, CURSOR * curs, int ch);
void exec_user_action(BUFFER * buff);
void select_text(BUFFER * buff, CURSOR * curs);
void print_status_bar(BUFFER * buff, char *str);
void print_menu(WINDOW * menu_win, int highlight);
void loading_menu();
void saving_menu();
void settings_menu(SETTINGS * set);

#endif

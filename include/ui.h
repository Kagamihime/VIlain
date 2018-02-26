#ifndef UI_H
#define UI_H

typedef struct CURSOR CURSOR;
typedef struct SETTINGS SETTINGS;

void print_text(BUFFER* buff, unsigned int first_line, unsigned int first_col);
void print_wrapped_text(BUFFER* buff, unsigned int first_line);
void exec_user_action();
void select_text(BUFFER* buff, CURSOR* curs);
void print_status_bar(char* str);
void loading_menu();
void saving_menu();
void settings_menu(SETTINGS* set);


#endif

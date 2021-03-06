#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct SETTINGS SETTINGS;

int save(SETTINGS * sets, char *path);
int load(SETTINGS * sets, char *path);
SETTINGS *new_sets();
char *to_string(int code);
int is_current_shortcut(SETTINGS * sets, int code);
int is_valid_config(SETTINGS * sets, char *path);

int get_auto_fill_mode(SETTINGS * sets);
int get_save_shortcut(SETTINGS * sets);
int get_load_shortcut(SETTINGS * sets);
int get_settings_shortcut(SETTINGS * sets);
int get_copy_shortcut(SETTINGS * sets);
int get_cut_shortcut(SETTINGS * sets);
int get_paste_shortcut(SETTINGS * sets);
int get_toogle_selection_shortcut(SETTINGS * sets);

void set_auto_fill_mode(SETTINGS * sets, int val);
void set_save_shortcut(SETTINGS * sets, int val);
void set_load_shortcut(SETTINGS * sets, int val);
void set_settings_shortcut(SETTINGS * sets, int val);
void set_copy_shortcut(SETTINGS * sets, int val);
void set_cut_shortcut(SETTINGS * sets, int val);
void set_paste_shortcut(SETTINGS * sets, int val);
void set_toogle_selection_shortcut(SETTINGS * sets, int val);

#endif

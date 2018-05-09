#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct SETTINGS SETTINGS;

void save(SETTINGS * sets, char *path);
int load(SETTINGS * sets, char *path);

int get_line_wrapping(SETTINGS * sets);
int get_save_shortcut(SETTINGS * sets);
int get_load_shortcut(SETTINGS * sets);
int get_settings_shortcut(SETTINGS * sets);
int get_copy_shortcut(SETTINGS * sets);
int get_cut_shortcut(SETTINGS * sets);
int get_paste_shortcut(SETTINGS * sets);
int get_toogle_selection_shortcut(SETTINGS * sets);

void set_line_wrapping(SETTINGS * sets, int val);
void set_save_shortcut(SETTINGS * sets, int val);
void set_load_shortcut(SETTINGS * sets, int val);
void set_settings_shortcut(SETTINGS * sets, int val);
void set_copy_shortcut(SETTINGS * sets, int val);
void set_cut_shortcut(SETTINGS * sets, int val);
void set_paste_shortcut(SETTINGS * sets, int val);
void set_toogle_selection_shortcut(SETTINGS * sets, int val);

#endif

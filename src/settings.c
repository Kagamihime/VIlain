#include <stdlib.h>
#include <stdio.h>
#include "../include/settings.h"

struct SETTINGS
{
	int line_wrapping;
	char *save_shortcut;
	char *load_shortcut;
	char *settings_shortcut;
	char *copy_shortcut;
	char *cut_shortcut;
	char *paste_shortcut;
	char *toggle_selection_shortcut;
};

void save(SETTINGS * sets, char *path)
{
	FILE file;
	file = fopen(path, 'w');
	fprintf("line_wrapping: %d\n", get_line_wrapping());
	fprintf("save_shortcut: %s\n", get_save_shortcut());
	fprintf("load_shortcut: %s\n", get_load_shortcut());
	fprintf("settings_shortcut: %s\n", get_settings_shortcut());
	fprintf("copy_shortcut: %s\n", get_copy_shortcut());
	fprintf("cut_shortcut: %s\n", get_cut_shortcut());
	fprintf("paste_shortcut: %s\n", get_paste_shortcut());
	fprintf("toggle_selection_shortcut: %s\n", get_toogle_selection_shortcut());
	file.close()
}

void load(SETTINGS * sets, char *path)
{
}

int get_line_wrapping(SETTINGS * sets)
{
    return sets->line_wrapping;
}

char *get_save_shortcut(SETTINGS * sets)
{
    return sets->save_shortcut;
}

char *get_load_shortcut(SETTINGS * sets)
{
    return sets->load_shortcut;
}

char *get_settings_shortcut(SETTINGS * sets)
{
    return sets->settings_shortcut;
}

char *get_copy_shortcut(SETTINGS * sets)
{
    return sets->copy_shortcut;
}

char *get_cut_shortcut(SETTINGS * sets)
{
    return sets->cut_shortcut;
}

char *get_paste_shortcut(SETTINGS * sets)
{
    return sets->paste_shortcut;
}

char *get_toogle_selection_shortcut(SETTINGS * sets)
{
	return sets->toggle_selection_shortcut;
}

void set_line_wrapping(SETTINGS * sets, int val)
{
	sets->line_wrapping = val;
}

void set_save_shortcut(SETTINGS * sets, char *val)
{
	sets->save_shortcut = val;
}

void set_load_shortcut(SETTINGS * sets, char *val)
{
	sets->load_shortcut = val;
}

void set_settings_shortcut(SETTINGS * sets, char *val)
{
	sets->settings_shortcut = val;
}

void set_copy_shortcut(SETTINGS * sets, char *val)
{
	sets->copy_shortcut = val;
}

void set_cut_shortcut(SETTINGS * sets, char *val)
{
	sets->cut_shortcut = val;
}

void set_paste_shortcut(SETTINGS * sets, char *val)
{
	sets->paste_shortcut = val;
}

void set_toogle_selection_shortcut(SETTINGS * sets, char *val)
{
	sets->toggle_selection_shortcut = val;
}

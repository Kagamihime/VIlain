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
    return NULL;
}

char *get_settings_shortcut(SETTINGS * sets)
{
    return NULL;
}

char *get_copy_shortcut(SETTINGS * sets)
{
    return NULL;
}

char *get_cut_shortcut(SETTINGS * sets)
{
    return NULL;
}

char *get_paste_shortcut(SETTINGS * sets)
{
    return NULL;
}

char *get_toogle_selection_shortcut(SETTINGS * sets)
{
    return NULL;
}

void set_line_wrapping(SETTINGS * sets, int val)
{
}

void set_save_shortcut(SETTINGS * sets, char *val)
{
}

void set_load_shortcut(SETTINGS * sets, char *val)
{
}

void set_settings_shortcut(SETTINGS * sets, char *val)
{
}

void set_copy_shortcut(SETTINGS * sets, char *val)
{
}

void set_cut_shortcut(SETTINGS * sets, char *val)
{
}

void set_paste_shortcut(SETTINGS * sets, char *val)
{
}

void set_toogle_selection_shortcut(SETTINGS * sets, char *val)
{
}

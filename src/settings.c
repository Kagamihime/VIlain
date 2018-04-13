#include <stdlib.h>
#include <stdio.h>
#include "../include/settings.h"
#define FILE_MAX_LENGTH 256
#define LINE_MAX_LENGTH 128

struct SETTINGS {
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
    FILE *file;
    file = fopen(path, "w");
    fprintf(file, "line_wrapping: %d\n", get_line_wrapping(sets));
    fprintf(file, "save_shortcut: %s\n", get_save_shortcut(sets));
    fprintf(file, "load_shortcut: %s\n", get_load_shortcut(sets));
    fprintf(file, "settings_shortcut: %s\n", get_settings_shortcut(sets));
    fprintf(file, "copy_shortcut: %s\n", get_copy_shortcut(sets));
    fprintf(file, "cut_shortcut: %s\n", get_cut_shortcut(sets));
    fprintf(file, "paste_shortcut: %s\n", get_paste_shortcut(sets));
    fprintf(file, "toggle_selection_shortcut: %s\n",
            get_toogle_selection_shortcut(sets));
    fclose(file);
}

void load(SETTINGS * sets, char *path)
{
    int i = 0;
    int val;
    char *val_string = malloc(sizeof(char *) * LINE_MAX_LENGTH);
    char *param = malloc(sizeof(char *) * LINE_MAX_LENGTH);
    char *line = malloc(sizeof(char *) * LINE_MAX_LENGTH);
    FILE *file;
    file = fopen(path, "r");
    if (file != NULL) {
        while (fgets(line, LINE_MAX_LENGTH, file) != NULL) {
            switch (i) {
                sscanf(line, "%s %d", param, &val);
                set_line_wrapping(sets, val);
                break;
            case 1:
                sscanf(line, "%s %s", param, val_string);
                set_save_shortcut(sets, val_string);
                break;
            case 2:
                sscanf(line, "%s %s", param, val_string);
                set_load_shortcut(sets, val_string);
                break;
            case 3:
                sscanf(line, "%s %s", param, val_string);
                set_settings_shortcut(sets, val_string);
                break;
            case 4:
                sscanf(line, "%s %s", param, val_string);
                set_copy_shortcut(sets, val_string);
                break;
            case 5:
                sscanf(line, "%s %s", param, val_string);
                set_cut_shortcut(sets, val_string);
                break;
            case 6:
                sscanf(line, "%s %s", param, val_string);
                set_paste_shortcut(sets, val_string);
                break;
            case 7:
                sscanf(line, "%s %s", param, val_string);
                set_toogle_selection_shortcut(sets, val_string);
                break;
            }
            i++;
        }
    }
    free(val_string);
    free(param);
    free(line);
    fclose(file);
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

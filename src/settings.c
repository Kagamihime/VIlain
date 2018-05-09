#include <stdlib.h>
#include <stdio.h>
#include "../include/settings.h"
#define FILE_MAX_LENGTH 256
#define LINE_MAX_LENGTH 128

struct SETTINGS {
    int line_wrapping;
    int save_shortcut;
    int load_shortcut;
    int settings_shortcut;
    int copy_shortcut;
    int cut_shortcut;
    int paste_shortcut;
    int toggle_selection_shortcut;
};

void save(SETTINGS * sets, char *path)
{
    FILE *file;
    file = fopen(path, "w");
    fprintf(file, "line_wrapping: %d\n", get_line_wrapping(sets));
    fprintf(file, "save_shortcut: %d\n", get_save_shortcut(sets));
    fprintf(file, "load_shortcut: %d\n", get_load_shortcut(sets));
    fprintf(file, "settings_shortcut: %d\n", get_settings_shortcut(sets));
    fprintf(file, "copy_shortcut: %d\n", get_copy_shortcut(sets));
    fprintf(file, "cut_shortcut: %d\n", get_cut_shortcut(sets));
    fprintf(file, "paste_shortcut: %d\n", get_paste_shortcut(sets));
    fprintf(file, "toggle_selection_shortcut: %d\n",
            get_toogle_selection_shortcut(sets));
    fclose(file);
}

int load(SETTINGS * sets, char *path)
{
    int i = 0;
    int val;
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
                sscanf(line, "%s %d", param, &val);
                set_save_shortcut(sets, val);
                break;
            case 2:
                sscanf(line, "%s %d", param, &val);
                set_load_shortcut(sets, val);
                break;
            case 3:
                sscanf(line, "%s %d", param, &val);
                set_settings_shortcut(sets, val);
                break;
            case 4:
                sscanf(line, "%s %d", param, &val);
                set_copy_shortcut(sets, val);
                break;
            case 5:
                sscanf(line, "%s %d", param, &val);
                set_cut_shortcut(sets, val);
                break;
            case 6:
                sscanf(line, "%s %d", param, &val);
                set_paste_shortcut(sets, val);
                break;
            case 7:
                sscanf(line, "%s %d", param, &val);
                set_toogle_selection_shortcut(sets, val);
                break;
            }
            i++;
        }
    }
    free(param);
    free(line);
    fclose(file);
    if (i != 7) {
        return 1;
    } else {
        return 0;
    }
}

char *to_string(int code)
{
    char *res;
    switch (code) {
    case 1:
        res = "Ctrl + A";
        break;
    case 2:
        res = "Ctrl + B";
        break;
    case 3:
        res = "Ctrl + C";
        break;
    case 4:
        res = "Ctrl + D";
        break;
    case 5:
        res = "Ctrl + E";
        break;
    case 6:
        res = "Ctrl + F";
        break;
    case 7:
        res = "Ctrl + G";
        break;
    case 8:
        res = "Ctrl + H";
        break;
    case 9:
        res = "Ctrl + I";
        break;
    case 10:
        res = "Ctrl + J";
        break;
    case 11:
        res = "Ctrl + K";
        break;
    case 12:
        res = "Ctrl + L";
        break;
    case 13:
        res = "Ctrl + M";
        break;
    case 14:
        res = "Ctrl + N";
        break;
    case 15:
        res = "Ctrl + O";
        break;
    case 16:
        res = "Ctrl + P";
        break;
    case 17:
        res = "Ctrl + Q";
        break;
    case 18:
        res = "Ctrl + R";
        break;
    case 19:
        res = "Ctrl + S";
        break;
    case 20:
        res = "Ctrl + T";
        break;
    case 21:
        res = "Ctrl + U";
        break;
    case 22:
        res = "Ctrl + V";
        break;
    case 23:
        res = "Ctrl + W";
        break;
    case 24:
        res = "Ctrl + X";
        break;
    case 25:
        res = "Ctrl + Y";
        break;
    case 26:
        res = "Ctrl + Z";
        break;
    }
    return res;
}

int get_line_wrapping(SETTINGS * sets)
{
    return sets->line_wrapping;
}

int get_save_shortcut(SETTINGS * sets)
{
    return sets->save_shortcut;
}

int get_load_shortcut(SETTINGS * sets)
{
    return sets->load_shortcut;
}

int get_settings_shortcut(SETTINGS * sets)
{
    return sets->settings_shortcut;
}

int get_copy_shortcut(SETTINGS * sets)
{
    return sets->copy_shortcut;
}

int get_cut_shortcut(SETTINGS * sets)
{
    return sets->cut_shortcut;
}

int get_paste_shortcut(SETTINGS * sets)
{
    return sets->paste_shortcut;
}

int get_toogle_selection_shortcut(SETTINGS * sets)
{
    return sets->toggle_selection_shortcut;
}

void set_line_wrapping(SETTINGS * sets, int val)
{
    sets->line_wrapping = val;
}

void set_save_shortcut(SETTINGS * sets, int val)
{
    sets->save_shortcut = val;
}

void set_load_shortcut(SETTINGS * sets, int val)
{
    sets->load_shortcut = val;
}

void set_settings_shortcut(SETTINGS * sets, int val)
{
    sets->settings_shortcut = val;
}

void set_copy_shortcut(SETTINGS * sets, int val)
{
    sets->copy_shortcut = val;
}

void set_cut_shortcut(SETTINGS * sets, int val)
{
    sets->cut_shortcut = val;
}

void set_paste_shortcut(SETTINGS * sets, int val)
{
    sets->paste_shortcut = val;
}

void set_toogle_selection_shortcut(SETTINGS * sets, int val)
{
    sets->toggle_selection_shortcut = val;
}

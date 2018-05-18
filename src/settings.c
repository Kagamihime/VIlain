#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../include/settings.h"
#define FILE_MAX_LENGTH 256
#define LINE_MAX_LENGTH 128

struct SETTINGS {
    int auto_fill_mode;
    int save_shortcut;
    int load_shortcut;
    int settings_shortcut;
    int copy_shortcut;
    int cut_shortcut;
    int paste_shortcut;
    int toggle_selection_shortcut;
};

SETTINGS *new_sets()
{
    // Allocate the SETTINGS struct
    SETTINGS *sets = NULL;
    if ((sets = (SETTINGS *) calloc(1, sizeof(SETTINGS))) == NULL) {
        return NULL;
    }
    //Initialize the parameters
    load(sets, "./etc/ui.cfg");
    return sets;
}

int save(SETTINGS * sets, char *path)
{
    FILE *file = NULL;

    if ((file = fopen(path, "w")) == NULL) {
        return -1;
    } else {
        fprintf(file, "auto_fill_mode: %d\n", get_auto_fill_mode(sets));
        fprintf(file, "save_shortcut: %d\n", get_save_shortcut(sets));
        fprintf(file, "load_shortcut: %d\n", get_load_shortcut(sets));
        fprintf(file, "settings_shortcut: %d\n", get_settings_shortcut(sets));
        fprintf(file, "copy_shortcut: %d\n", get_copy_shortcut(sets));
        fprintf(file, "cut_shortcut: %d\n", get_cut_shortcut(sets));
        fprintf(file, "paste_shortcut: %d\n", get_paste_shortcut(sets));
        fprintf(file, "toggle_selection_shortcut: %d\n",
                get_toogle_selection_shortcut(sets));
        fclose(file);
        return 0;
    }
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
            case 0:
                sscanf(line, "%s %d", param, &val);
                set_auto_fill_mode(sets, val);
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
    if (i != 8) {
        return -1;
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
    case 11:
        res = "Ctrl + K";
        break;
    case 12:
        res = "Ctrl + L";
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
    case 18:
        res = "Ctrl + R";
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
    }
    return res;
}

int is_current_shortcut(SETTINGS * sets, int code)
{
    if (sets->save_shortcut == code
        || sets->load_shortcut == code
        || sets->settings_shortcut == code
        || sets->copy_shortcut == code
        || sets->cut_shortcut == code
        || sets->paste_shortcut == code
        || sets->toggle_selection_shortcut == code)
        return 1;
    return 0;
}

// Check if the configuration file is valid, if it is not, rewrite the ui.cfg file correctly
// Check if the configuration file is valid, if it is not, rewrite the ui.cfg file correctly
int is_valid_config(SETTINGS * sets, char *path)
{
    int res = 1;

    if (access(path, F_OK) == -1) {
        res = 0;
    }

    FILE *file;

    if ((file = fopen(path, "r")) == NULL) {
        res = 0;
    }

    char *line = malloc(sizeof(char *) * LINE_MAX_LENGTH);
    char *param = malloc(sizeof(char *) * LINE_MAX_LENGTH);
    int val;

    while (fgets(line, LINE_MAX_LENGTH, file) != NULL) {
        sscanf(line, "%s %d", param, &val);
        if (val <= 0 || val > 25 || val == 3 || val == 10
            || val == 13 || val == 17 || val == 19) {
            res = 0;
        }
    }
    fclose(file);
    free(line);
    free(param);

    if (load(sets, path) == -1) {
        res = 0;
    }

    if (res == 0) {
        remove(path);
        file = fopen(path, "w");
        fprintf(file, "auto_fill_mode: 0\n");
        fprintf(file, "save_shortcut: 4\n");
        fprintf(file, "load_shortcut: 12\n");
        fprintf(file, "settings_shortcut: 15\n");
        fprintf(file, "copy_shortcut: 23\n");
        fprintf(file, "cut_shortcut: 24\n");
        fprintf(file, "paste_shortcut: 22\n");
        fprintf(file, "toggle_selection_shortcut: 20\n");
        fclose(file);
    }
    return res;
}

int get_auto_fill_mode(SETTINGS * sets)
{
    return sets->auto_fill_mode;
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

void set_auto_fill_mode(SETTINGS * sets, int val)
{
    sets->auto_fill_mode = val;
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

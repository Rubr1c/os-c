#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "dir.h"

char **split_str(char *str, char del) {
    unsigned long size = strlen(str);
    char **out = malloc(sizeof(char *) * (size + 1));
    int idx = 0;
    int start = 0;
    for (int i = 0; i < size; i++) {
        if (str[i] == del) {
            int len = i - start;
            if (len > 0) {
                out[idx] = malloc(len + 1);
                memcpy(out[idx], str + start, len);
                out[idx][len] = '\0';
                idx++;
            }
            start = i + 1;
        }
    }
    int len = size - start;
    if (len > 0) { 
        out[idx] = malloc(len + 1);
        memcpy(out[idx], str + start, len);
        out[idx][len] = '\0';
        idx++;
    }
    out[idx] = NULL;
    return out;
}

void handle_cmd(char *path, char **cmd) {
    if (cmd[0] == NULL) return; 
    if (strcmp(cmd[0], "cd") == 0) {
    } else if (strcmp(cmd[0], "ls") == 0 || strcmp(cmd[0], "dir") == 0) {
        print_dirs(path);
    } else if (strcmp(cmd[0], "mkdir") == 0) {

        char n_path[256];
        sprintf(n_path, "%s/%s", path, cmd[1]);
        if (!create_dir(n_path)) {
            printf("Directory already exists\n\n");
            return;
        }
    }
}

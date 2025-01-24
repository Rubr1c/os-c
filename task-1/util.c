#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
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

void handle_cmd(char **cmd) {
    if (cmd[0] == NULL) return;

    if (strcmp(cmd[0], "cd") == 0) {
        char target_dir[256];
        const char *current_dir = getenv("PWD");
        char *new_dir = cmd[1];

        if (new_dir == NULL) {
            const char *pwd = getenv("PWD");
            printf("%s\n\n", pwd ? pwd : "PWD not set");
            return; 
        }

        if (new_dir[0] == '/') {
            snprintf(target_dir, sizeof(target_dir), "%s", new_dir);
        } else {
            snprintf(target_dir, sizeof(target_dir), "%s/%s", current_dir, new_dir);
        }

        if (chdir(target_dir) == 0) {
            char u_dir[256];
            getcwd(u_dir, sizeof(u_dir));
            setenv("PWD", u_dir, 1); 
        } else {
            perror("cd");
        }

    } else if (strcmp(cmd[0], "ls") == 0 || strcmp(cmd[0], "dir") == 0) {
        char *current_dir = getenv("PWD");
        print_dirs(current_dir);

    } else if (strcmp(cmd[0], "mkdir") == 0) {
        const char *current_dir = getenv("PWD");
        char n_path[256];
        snprintf(n_path, sizeof(n_path), "%s/%s", current_dir, cmd[1]);
        if (!create_dir(n_path)) {
            printf("Directory already exists\n\n");
        }
    } else if (strcmp(cmd[0], "env") == 0 || strcmp(cmd[0], "environ") == 0) {
        extern char **environ;

        for (char **var = environ; *var != NULL; var++) {
            printf("%s\n", *var);
        }
        printf("\n");
    }
}

void free_tokens(char **tokens) {
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

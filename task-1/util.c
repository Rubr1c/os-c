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

        // Handle "cd" with no args (go home)
        if (new_dir == NULL) {
            new_dir = getenv("HOME");
            if (new_dir == NULL) {
                fprintf(stderr, "cd: HOME not set\n");
                return;
            }
        }

        // Resolve target directory
        if (new_dir[0] == '/') {
            snprintf(target_dir, sizeof(target_dir), "%s", new_dir);
        } else {
            snprintf(target_dir, sizeof(target_dir), "%s/%s", current_dir, new_dir);
        }

        // Change directory and update PWD
        if (chdir(target_dir) == 0) {
            char updated_dir[256];
            getcwd(updated_dir, sizeof(updated_dir)); // Get absolute path
            setenv("PWD", updated_dir, 1); // Update environment
        } else {
            perror("cd");
        }

    } else if (strcmp(cmd[0], "ls") == 0 || strcmp(cmd[0], "dir") == 0) {
        const char *current_dir = getenv("PWD");
        print_dirs(current_dir);

    } else if (strcmp(cmd[0], "mkdir") == 0) {
        const char *current_dir = getenv("PWD");
        char new_path[256];
        snprintf(new_path, sizeof(new_path), "%s/%s", current_dir, cmd[1]);
        if (!create_dir(new_path)) {
            printf("Directory already exists\n\n");
        }
    }
}

void free_tokens(char **tokens) {
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

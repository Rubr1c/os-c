#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include "dir.h"
#include "util.h"

char path[256] = "root";

int main() {
    while (1) {
        char *cmd = NULL;
        size_t size = 0;
        ssize_t chars;

        printf("user@%s> ", path);
        chars = getline(&cmd, &size, stdin);
        if (chars == -1) {
            free(cmd);
            break; 
        }
        if (chars > 0 && cmd[chars - 1] == '\n') {
            cmd[chars - 1] = '\0';
        }
        char **tokens = split_str(cmd, ' ');
        if (strcmp(tokens[0], "quit") == 0) {
            free(cmd);
            for (int i = 0; tokens[i] != NULL; i++) {
                free(tokens[i]);
            }
            free(tokens);
            break;
        }
        handle_cmd(path, tokens);
        free(cmd);
        for (int i = 0; tokens[i] != NULL; i++) {
            free(tokens[i]);
        }
        free(tokens);
    }
    return 0;
}

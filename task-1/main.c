#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>

char path[256] = "root";

void print_dirs() {
    DIR *d;
    struct dirent *dir;
    d = opendir("./root");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                printf("%s ", dir->d_name);
            }
        }
        printf("\n\n");
        closedir(d);
    }
}

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
    } else if (strcmp(cmd[0], "ls") == 0) {
        print_dirs();
    }
}

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
        handle_cmd(tokens);
        free(cmd);
        for (int i = 0; tokens[i] != NULL; i++) {
            free(tokens[i]);
        }
        free(tokens);
    }
    return 0;
}

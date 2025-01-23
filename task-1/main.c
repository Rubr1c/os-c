#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char path[256] = "root";

char **split_str(char *str, char del) {
    unsigned long size = strlen(str);
    char ** out = malloc(sizeof(char *) * (size + 1));
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

    out[idx] = malloc(size - start);
    strcpy(out[idx], str + start);
    out[idx][size - start] = '\0'; 

    out[idx + 1] = NULL;
    return out;
}

void handle_cmd(char** cmd) {
    if (strcmp(cmd[0], "cd")) {
        
    }
}

int main() {

    while (1) {
        char *cmd;
        size_t size = 256;
        size_t chars;

        cmd = malloc(size * sizeof(char));
        printf("user@%s> ", path);
        chars = getline(&cmd, &size, stdin);
        char **tokens = split_str(cmd, ' ');
        if (strcmp(tokens[0], "quit")) {
            break;
        }
        handle_cmd(tokens);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include "dir.h"
#include "util.h"
#include "colors.h"

int main(int argc, char *argv[]) {
    FILE *input  = stdin;
    if (argc > 1) {
        input = fopen(argv[1], "r");
        if (!input) {
            perror("Error opening batch file");
            exit(EXIT_FAILURE);
        }
    }

    const char *home_dir = getenv("HOME");
    if (home_dir == NULL) {
        home_dir = getpwuid(getuid())->pw_dir;
    }
    chdir(home_dir);
    setenv("PWD", home_dir, 1);

    const char *default_path = "/bin:/usr/bin";
    if (getenv("PATH") == NULL) {
        setenv("PATH", default_path, 1);
    }

    while (1) {
        char *cmd = NULL;
        size_t size = 0;
        ssize_t chars;

        const char *current_dir = getenv("PWD");

        if (input == stdin) {
            printf(CYAN "user@%s> " RESET, current_dir);
        }
        chars = getline(&cmd, &size, input);
        if (chars == -1) {
            free(cmd);
            break;
        }
        if (chars > 0 && cmd[chars - 1] == '\n') {
            cmd[chars - 1] = '\0';
        }

        char **tokens = split_str(cmd, ' ');
        if (tokens[0] != NULL && strcmp(tokens[0], "quit") == 0) {
            free(cmd);
            free_tokens(tokens);
            break;
        }

        handle_cmd(tokens); 

        free(cmd);
        free_tokens(tokens);

    }
    if (input != stdin) fclose(input); 
    return 0;
}

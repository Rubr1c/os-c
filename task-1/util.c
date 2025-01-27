#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
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

    char *input_file = NULL;
    char *output_file = NULL;
    bool append_mode = false;
    bool background = false;

    for (int i = 0; cmd[i] != NULL; i++) {
        if (strcmp(cmd[i], "<") == 0 && cmd[i+1] != NULL) {
            input_file = cmd[i+1];
            cmd[i] = NULL;
            cmd[i+1] = NULL;
            i++;
        } 
        else if (strcmp(cmd[i], ">") == 0 && cmd[i+1] != NULL) {
            output_file = cmd[i+1];
            append_mode = false;
            cmd[i] = NULL;
            cmd[i+1] = NULL;
            i++;
        } 
        else if (strcmp(cmd[i], ">>") == 0 && cmd[i+1] != NULL) {
            output_file = cmd[i+1];
            append_mode = true;
            cmd[i] = NULL;
            cmd[i+1] = NULL;
            i++;
        } else if (strcmp(cmd[i], "&") == 0) {
            cmd[i] = NULL;  
            background = true;
        }
    }

    int idx = 0;
    for (int i = 0; cmd[i] != NULL; i++) {
        if (cmd[i] != NULL) {
            cmd[idx++] = cmd[i];
        }
    }
    cmd[idx] = NULL;

    bool is_builtin_cmd = false;
    const char *builtin_cmds[] = {"cd", "ls", "dir", "mkdir","environ", "env", "set", "echo", "help", "pause"};
    for (int k = 0; k < sizeof(builtin_cmds)/sizeof(builtin_cmds[0]); k++) {
        if (cmd[0] && strcmp(cmd[0], builtin_cmds[k]) == 0) {
            is_builtin_cmd = true;
            break;
        }
    }

    if (is_builtin_cmd) {
        int s_stdin = dup(STDIN_FILENO);
        int s_stdout = dup(STDOUT_FILENO);

        if (input_file) {
            int file_disc = open(input_file, O_RDONLY);
            if (file_disc != -1) {
                dup2(file_disc, STDIN_FILENO);
                close(file_disc);
            } else {
                perror("open input");
            }
        }

        if (output_file) {
            int flags = O_WRONLY | O_CREAT | (append_mode ? O_APPEND : O_TRUNC);
            int file_disc = open(output_file, flags, 0644);
            if (file_disc != -1) {
                dup2(file_disc, STDOUT_FILENO);
                close(file_disc);
            } else {
                perror("open output");
            }
        }

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

        } else if (strcmp(cmd[0], "ls") == 0) { 
            bool show_hidden = false;
            char *target = getenv("PWD");
            for (int i = 1; cmd[i] != NULL; i++) {
                if (strcmp(cmd[i], "-a") == 0) {
                    show_hidden = true;
                } else {
                    target = cmd[i];
                }
            }

            char path[256];
            if (target[0] != '/') {
                snprintf(path, sizeof(path), "%s/%s", getenv("PWD"), target);
            } else {
                strncpy(path, target, sizeof(path));
            }

            print_dirs(path, show_hidden, true);

        } else if (strcmp(cmd[0], "dir") == 0) {
            bool show_hidden = false;
            char *target = getenv("PWD");
            for (int i = 1; cmd[i] != NULL; i++) {
                if (strcmp(cmd[i], "-a") == 0) {
                    show_hidden = true;
                } else {
                    target = cmd[i];
                }
            }

            char path[256];
            if (target[0] != '/') {
                snprintf(path, sizeof(path), "%s/%s", getenv("PWD"), target);
            } else {
                strncpy(path, target, sizeof(path));
            }

            print_dirs(path, show_hidden, false);

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
        } else if (strcmp(cmd[0], "set") == 0) {
            setenv(cmd[1], cmd[2], 1); 
        } 
        else if (strcmp(cmd[0], "echo") == 0) {
            if (cmd[1][0] == '$') {
                printf("%s\n\n", getenv(cmd[1] + 1));
                return;
            }
            int idx = 1;
            char *curr = cmd[1];
            while (curr != NULL) {
                printf("%s ", curr);
                curr = cmd[++idx];
            }
            printf("\n\n");
        } else if (strcmp(cmd[0], "help") == 0) {
            FILE* more = popen("more", "w");
            if (!more) {
                perror("popen failed");
                return;
            }
            fprintf(more, "cd [DIR_NAME | DIR_PATH] -changes directory\n");
            fprintf(more, "ls [DIR_NAME] {-a: show hidden} -list all items in directory with colors\n");
            fprintf(more, "dir [DIR_NAME] {-a: show hidden} -list all items in directory\n");
            fprintf(more, "mkdir [DIR_NAME --required] -creates a directory\n");
            fprintf(more, "env | environ -show environment variables\n");
            fprintf(more, "set [KEY] [VALUE] -sets a environment variable\n");
            fprintf(more, "echo [MSG] {$ENV_KEY: env var} -prints to terminal\n\n");

            pclose(more);
        } else if (strcmp(cmd[0], "pause") == 0) {
            char i; 
            while ((i = getchar()) != '\n');
        }

        dup2(s_stdin, STDIN_FILENO);
        dup2(s_stdout, STDOUT_FILENO);
        close(s_stdin);
        close(s_stdout);

    } else {
        pid_t pid = fork();
        if (pid == 0) {
            if (input_file) {
                int file_disc = open(input_file, O_RDONLY);
                if (file_disc == -1) {
                    perror("open input");
                    exit(EXIT_FAILURE);
                }
                dup2(file_disc, STDIN_FILENO);
                close(file_disc);
            }

            if (output_file) {
                int flags = O_WRONLY | O_CREAT | (append_mode ? O_APPEND : O_TRUNC);
                int file_disc = open(output_file, flags, 0644);
                if (file_disc == -1) {
                    perror("open output");
                    exit(EXIT_FAILURE);
                }
                dup2(file_disc, STDOUT_FILENO);
                close(file_disc);
            }

            execvp(cmd[0], cmd);
            perror("execvp failed");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {

            if (!background) {
                wait(NULL);
            } else {
                printf("Command running in the background with PID %d\n\n", pid);
            }

        } else {
            perror("fork failed");
        }
    }
}

void free_tokens(char **tokens) {
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

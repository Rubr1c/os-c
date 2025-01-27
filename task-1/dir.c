#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "colors.h"

void print_dirs(char* path, bool hidden, bool colors) {
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (!hidden && dir->d_name[0] == '.') continue;
            if (dir->d_type == DT_DIR && colors) {
                printf(BLUE "%s " RESET, dir->d_name); 
            } else {
                printf("%s ", dir->d_name); 
            }
        }
        printf("\n\n");
        closedir(d);
    } else {
        perror("opendir");
        printf("\n");
    }
}

bool includes_dir(char *path,char *name) {
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, name) == 0 && dir->d_type == DT_DIR) {
                return true;
            }
        }
        closedir(d);
    }
    return false;
}
struct stat st = {0};

bool create_dir(char *path) {
    if (stat(path, &st) == -1) {
        mkdir(path, 0700);
        return true;
    }
    return false;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/types.h>


void list_directory(const char* path, bool show_hidden) {
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    
    printf("Directory '%s':\n", path);
    
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (!show_hidden && dir->d_name[0] == '.') continue;

            printf("%s\n", dir->d_name); 
            
        }
        closedir(d);
        printf("\n");
    } else {
        perror("opendir");
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    bool show_hidden = false;
    
    if (argc < 2) {
        list_directory(".", show_hidden);
        return 0;
    }
    
    int start_index = 1;
    if (strcmp(argv[1], "-a") == 0) {
        show_hidden = true;
        start_index++;
        
        if (argc == 2) {
            list_directory(".", show_hidden);
            return 0;
        }
    }
    
    for (int i = start_index; i < argc; i++) {
        list_directory(argv[i], show_hidden);
    }
    
    return 0;
}

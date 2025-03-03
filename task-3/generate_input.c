#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FILE *fp = fopen("input.txt", "w");
    if (fp == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    
    srand(42);
    
    for (int i = 0; i < 1000; i++) {
        int num = rand() % 50 + 1;
        fprintf(fp, "%d", num);
        if (i < 999) {
            fprintf(fp, " ");
        }
    }
    
    fclose(fp);
    return 0;
}


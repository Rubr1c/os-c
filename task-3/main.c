#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_FRAMES 20

typedef struct {
    unsigned int page_num;
    uint8_t age;
    unsigned valid:1;
    unsigned ref:1;
} Page;

typedef struct {
    int frame_count;
    int total_faults;
    double faults_per_1000;
} SimulationResult;

Page *read_file(char *filename, unsigned int *num_pages) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    
    unsigned int capacity = 1000;
    
    Page *pages = malloc(sizeof(Page) * capacity);
    if (!pages) {
        perror("Error allocating memory");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    
    while (fscanf(file, "%d", &pages[*num_pages].page_num) == 1) {
        pages[*num_pages].age = 0;
        pages[*num_pages].valid = 0;
        pages[*num_pages].ref = 0;

        (*num_pages)++;
        if (*num_pages >= capacity) {
            capacity *= 2;
            Page *temp = realloc(pages, capacity * sizeof(Page));
            pages = temp;
        }
    }
    
    fclose(file);
    return pages;
}

int find_page(unsigned int page_num, unsigned int *frames, int frame_count) {
    for (int i = 0; i < frame_count; i++) {
        if (frames[i] == page_num) {
            return i;
        }
    }
    return -1;
}

int find_min_age(Page *pages, unsigned int *frames, int frame_count) {
    uint8_t min_age = UINT8_MAX;
    int victim_index = 0;
    
    for (int i = 0; i < frame_count; i++) {
        if (pages[frames[i]].age < min_age) {
            min_age = pages[frames[i]].age;
            victim_index = i;
        }
    }
    
    return victim_index;
}

SimulationResult start_aging(Page *pages, unsigned int num_pages, int frame_count) {
    SimulationResult result = {frame_count, 0, 0.0};
    unsigned int *frames = calloc(frame_count, sizeof(unsigned int));
    int frame_index = 0;
    int frames_used = 0;
    
    for (unsigned int i = 0; i < num_pages; i++) {
        unsigned int current_page = pages[i].page_num;
        int page_location = find_page(current_page, frames, frames_used);
        
        if (page_location == -1) {
            result.total_faults++;
            
            if (frames_used < frame_count) {
                frames[frames_used++] = current_page;
                pages[current_page].valid = 1;
            } else {
                int victim = find_min_age(pages, frames, frame_count);
                pages[frames[victim]].valid = 0;
                frames[victim] = current_page;
                pages[current_page].valid = 1;
                pages[current_page].age = 0;
            }
        }
        
        pages[current_page].ref = 1;
        
        if (i % 4 == 0) {
            for (int j = 0; j < frames_used; j++) {
                unsigned int page_num = frames[j];
                pages[page_num].age = pages[page_num].age >> 1;
                if (pages[page_num].ref) {
                    pages[page_num].age = pages[page_num].age | 0x80;
                    pages[page_num].ref = 0;
                }
            }
        }
    }
    
    result.faults_per_1000 = (result.total_faults * 1000.0) / num_pages;
    free(frames);
    return result;
}

int main() {
    char file[50];
    printf("Enter filename: ");
    scanf("%49s", file);

    unsigned int num_pages = 0;
    Page *pages = read_file(file, &num_pages);
    
    printf("\nSimulation Results:\n");
    printf("Frames\tFaults\tFaults per 1000 refs\n");
    printf("------\t------\t------------------\n");
    
    for (int frames = 1; frames <= MAX_FRAMES; frames++) {
        Page *pages_copy = malloc(num_pages * sizeof(Page));
        memcpy(pages_copy, pages, num_pages * sizeof(Page));
        
        SimulationResult result = start_aging(pages_copy, num_pages, frames);
        printf("%d\t%d\t%.2f\n", frames, result.total_faults, result.faults_per_1000);
        
        free(pages_copy);
    }

    free(pages);
    return 0;
}


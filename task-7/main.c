#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int process_count;
    int resource_t_count;
    int *existing_resources;
    int *allocation_m;
    int *request_m;
} input_t;

typedef struct {
    size_t deadlock_count;
    int *deadlocks;
} Deadlocks;

input_t read_input(char *filename) {
   input_t input = {0};
   FILE *file = fopen(filename, "r"); 
   
   char line[256];
   fgets(line, sizeof(line), file);
   sscanf(line, "%d %d", &input.process_count, &input.resource_t_count);
   input.existing_resources = malloc(sizeof(int) * input.resource_t_count);
   fgets(line, sizeof(line), file);
   char *curr = line;
   int char_count;
   for (int i = 0; i < input.resource_t_count; i++) {
       sscanf(curr, "%d%n", &input.existing_resources[i], &char_count);
       curr += char_count;
   }

   input.allocation_m = malloc(sizeof(int) * (input.process_count * input.resource_t_count));
   for (int i = 0; i < input.process_count; i++) {
       fgets(line, sizeof(line), file);
       curr = line;
       for (int j = 0; j < input.resource_t_count; j++) {
           sscanf(curr, "%d%n", &input.allocation_m[i * input.resource_t_count + j], &char_count);
           curr += char_count;
       }
   }

   input.request_m = malloc(sizeof(int) * (input.process_count * input.resource_t_count));
   for (int i = 0; i < input.process_count; i++) {
       fgets(line, sizeof(line), file);
       curr = line;
       for (int j = 0; j < input.resource_t_count; j++) {
           sscanf(curr, "%d%n", &input.request_m[i * input.resource_t_count + j], &char_count);
           curr += char_count;
       }
   }

   fclose(file);
   return input;
}

int* get_available(int *existing_resources, 
                   int *allocation_m, 
                   int process_count, 
                   int resource_t_count) {

    int *available_resources = malloc(sizeof(int) * resource_t_count);
    for (int j = 0; j < resource_t_count; j++) {
        int total = 0;
        for (int i = 0; i < process_count; i++) {
            total += allocation_m[i * resource_t_count + j];
        }
        available_resources[j] = existing_resources[j] - total;
    }
    return available_resources;
}

Deadlocks get_deadlocked(int process_count, 
                    int resource_t_count, 
                    int* allocation_m, 
                    int* request_m, 
                    int* available_resources) {

    int *work = malloc(resource_t_count * sizeof(int));
    memcpy(work, available_resources, resource_t_count * sizeof(int));
    bool *finished = malloc(process_count * sizeof(bool));

    for (int i = 0; i < process_count; i++) {
        size_t sum = 0;
        for (int j = 0; j < resource_t_count; j++) {
            sum += allocation_m[i * resource_t_count + j];
        }

        finished[i] = sum == 0;
    }

    bool found;
    do {
        found = false;
        for (int i = 0; i < process_count; i++) {
            if (!finished[i]) {
                bool can_run = true;
                for (int j = 0; j < resource_t_count; j++) {
                    if (request_m[i * resource_t_count + j] > work[j]) {
                        can_run = false;
                        break;
                    }
                }
                if (can_run) {
                    for (int j = 0; j < resource_t_count; j++) {
                        work[j] = work[j] + allocation_m[i * resource_t_count + j];
                    }
                    finished[i] = true;
                    found = true;
                }
            }
        }

    } while (found);

    size_t deadlock_count = 0;
    int *deadlocked_p = malloc(process_count * sizeof(int));

    for (int i = 0; i < process_count; i++) {
        if (!finished[i]) {
           deadlocked_p[deadlock_count++] = i; 
        }
    }

    Deadlocks d = {deadlock_count, deadlocked_p};
    return d; 
} 

int main() {
    char file[50];
    printf("Enter input filename: ");
    scanf("%s", file);
    input_t in = read_input(file);

    int *av = get_available(in.existing_resources, 
                            in.allocation_m, 
                            in.process_count, 
                            in.resource_t_count);

    Deadlocks deadlocks = get_deadlocked(in.process_count, 
                                         in.resource_t_count,
                                         in.allocation_m, 
                                         in.request_m, 
                                         av); 

    printf("Deadlocked processes: \n");
    for (int i = 0; i < deadlocks.deadlock_count; i++) {
        printf("Process %d\n", deadlocks.deadlocks[i]);
    }

    return 0;
}

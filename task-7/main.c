#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int process_count;
    int resource_t_count;
    int *existing_resources;
    int *allocation_m;
    int *request_m;
} input_t;


input_t read_input(char *filename) {
   input_t input;
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
    
}

int main() {
    input_t in = read_input("input.txt");

    return 0;
}

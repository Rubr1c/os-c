#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>


size_t t_num;

typedef struct {
    size_t w_count;
    bool empty_end;
    bool empty_start;
} Result;

char *extract_text(const char *filename) {
    char *file_content = NULL;
    FILE *file = fopen(filename, "r");
    if (file) {
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);
        file_content = malloc(file_size + 1);
        fread(file_content, 1, file_size, file);
        file_content[file_size] = '\0';
        fclose(file);
    }
    return file_content;
}

void* count(void *str) {
    char *s = (char *)str;
    size_t len = strlen(s);
    Result *result = malloc(sizeof(Result));
    result->w_count = 0;
    result->empty_start = (len > 0) ? (s[0] != ' ' && s[0] != '\n') : false;
    result->empty_end = (len > 0) ? (s[len-1] != ' ' && s[len-1] != '\n') : false;

    printf("Thead %d reading %zu characters\n", syscall(SYS_gettid), len);

    bool inside_word = false;
    for (size_t i = 0; i < len; i++) {
        if (s[i] != ' ' && s[i] != '\n') {
            if (!inside_word) {
                result->w_count++;
                inside_word = true;
            }
        } else {
            inside_word = false;
        }
    }

    free(s);
    return result;
}

int main() {
    char filename[30];
    printf("Enter file name: ");
    scanf("%s", filename);
    printf("Enter segment count: ");
    scanf("%zu", &t_num);
    char *words = extract_text(filename);
    size_t len = strlen(words);
    
    size_t part_size = len / t_num;
    size_t remainder = len % t_num;
    pthread_t threads[t_num];
    Result *results[t_num];

    for (int i = 0; i < t_num; i++) {
        size_t start = i * part_size + (i < remainder ? i : 0);
        size_t size = part_size + (i < remainder ? 1 : 0);
        size_t end = start + size;
        if (end > len) end = len;
        size = end - start;

        char *s = malloc(size + 1);
        strncpy(s, words + start, size);
        s[size] = '\0';

        pthread_create(&threads[i], NULL, count, s);
    }

    size_t total = 0;
    for (int i = 0; i < t_num; i++) {
        pthread_join(threads[i], (void**)&results[i]);
        total += results[i]->w_count;
    }

    for (int i = 0; i < t_num - 1; i++) {
        if (results[i]->empty_end && results[i + 1]->empty_start) {
            total--;
        }
    }

    printf("Word count: %zu\n", total);
    
    for (int i = 0; i < t_num; i++) free(results[i]);
    free(words);
    return 0;
}

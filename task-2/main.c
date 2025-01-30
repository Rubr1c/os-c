#include <sched.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdbool.h>

size_t t_num;

void* print_s(void *str) {
    char *s = (char *)str;
    pid_t id = syscall(SYS_gettid);
    size_t len = strlen(s);
    size_t *count = malloc(sizeof(size_t));
    *count = 0;
    bool inside_word = false; 
    for (int i = 0; i < len; i++) {
        if (s[i] != ' ') {
            if (!inside_word) {
                (*count)++;
                inside_word = true;
            }
        } else {
            inside_word = true;
        }
    }
    free(s);
    return count;
}

int main() {
    printf("Enter segment count\n");
    scanf("%zu", &t_num);
    const char *words = "testing threads  1  2 3  ";
    size_t c_count = strlen(words) / t_num;
    pthread_t threads[t_num];
    size_t w_count = 0;

    for (int i = 0; i < t_num; i++) {
        char *s = malloc(c_count + 2);
        strncpy(s, words + (c_count * i), c_count);
        s[c_count] = '\0';
        pthread_create(&threads[i], NULL, print_s, s);
    }

    size_t *curr;
    for (int i = 0; i < t_num; i++) {
        pthread_join(threads[i], (void**)&curr);
        if (curr) { 
            w_count += *curr;
            free(curr);
        }
    }

    if (w_count > 0) w_count++;

    printf("%zu\n", w_count);
}

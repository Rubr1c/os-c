#include <sched.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

void* print_s(void *str) {
    char *s = (char *)str;
    pid_t id = syscall(SYS_gettid);
    printf("%d: %s\n", id, s);
    free(s);
    return NULL;
}

int main() {
    char *words = "testing threads 1 2 3";
    size_t t_num = 3;
    size_t c_count = strlen(words) / t_num;
    pthread_t threads[t_num];

    for (int i = 0; i < t_num; i++) {
        char *s = malloc(c_count + 2);
        strncpy(s, words + (c_count * i), c_count);
        s[c_count] = '\0';
        pthread_create(&threads[i], NULL, print_s, s);
    }

    for (int i = 0; i < t_num; i++) {
        pthread_join(threads[i], NULL);
    }
}

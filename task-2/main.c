#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

size_t t_num;


typedef struct {
    const char *filename;
    size_t s_offest;
    size_t size;
    bool is_first;
    bool is_last;
} Seg;

typedef struct WordNode {
    char *word;
    size_t count;
    struct WordNode *next;
} WordNode;

void add_word(WordNode **map, const char *word) {
    WordNode *cur = *map;
    while (cur) {
        if (strcmp(cur->word, word) == 0) {
            cur->count++;
            return;
        }
        cur = cur->next;
    }

    WordNode *node = malloc(sizeof(WordNode));
    node->word = strdup(word);
    node->count = 1;
    node->next = *map;
    *map = node;
}

void *count(void *arg) {
    Seg *targ = (Seg *)arg;
    
    FILE *file = fopen(targ->filename, "r");
    if (!file) {
        perror("Error opening file in thread");
        free(targ);
        pthread_exit(NULL);
    }

    long seg_s_offest = targ->s_offest;
    long seg_end = targ->s_offest + targ->size;

    if (!targ->is_first) {
        fseek(file, seg_s_offest, SEEK_SET);
        int c;
        while ((c = fgetc(file)) != EOF) {
            if (c == ' ' || c == '\n' || c == '\t')
                break;
            seg_s_offest++;
        }
    }
    
    if (!targ->is_last) {
        fseek(file, seg_end - 1, SEEK_SET);
        int last_char = fgetc(file);
        if (!(last_char == ' ' || last_char == '\n' || last_char == '\t')) {
            int c;
            fseek(file, seg_end, SEEK_SET);
            while ((c = fgetc(file)) != EOF) {
                seg_end++;
                if (c == ' ' || c == '\n' || c == '\t')
                    break;
            }
        }
    }

    size_t read_size = seg_end - seg_s_offest;
    char *buffer = malloc(read_size + 1);

    fseek(file, seg_s_offest, SEEK_SET);
    size_t bytes_read = fread(buffer, 1, read_size, file);
    buffer[bytes_read] = '\0';

    fclose(file);
    free(targ);

    printf("Thread %ld reading %zu characters (offset %ld to %ld)\n", 
           syscall(SYS_gettid), bytes_read, seg_s_offest, seg_end);

    WordNode *word = NULL;
    char *delim = " \n\t";
    char *saveptr;
    char *token = strtok_r(buffer, delim, &saveptr);
    while (token != NULL) {
        add_word(&word, token);
        token = strtok_r(NULL, delim, &saveptr);
    }

    free(buffer);
    return word;
}

void add_map(WordNode **dest, WordNode *src) {
    while (src) {
        add_word(dest, src->word);
        src = src->next;
    }
}

void free_map(WordNode *map) {
    while (map) {
        WordNode *next = map->next;
        free(map->word);
        free(map);
        map = next;
    }
}

int main() {
    char filename[256];
    printf("Enter file name: ");
    scanf("%255s", filename);
    printf("Enter segment (thread) count: ");
    scanf("%zu", &t_num);

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file in main");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fclose(file);

    size_t part_size = file_size / t_num;
    size_t remainder = file_size % t_num;
    
    pthread_t threads[t_num];

    for (size_t i = 0; i < t_num; i++) {
        size_t extra = (i < remainder ? 1 : 0);
        size_t offset = i * part_size + (i < remainder ? i : remainder);
        size_t size  = part_size + extra;

        Seg *targ = malloc(sizeof(Seg));
        targ->filename = filename;
        targ->s_offest = offset;
        targ->size = size;
        targ->is_first = (i == 0);
        targ->is_last  = (i == t_num - 1);
        
        if (pthread_create(&threads[i], NULL, count, targ) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    WordNode *map = NULL;
    for (size_t i = 0; i < t_num; i++) {
        WordNode *word = NULL;
        pthread_join(threads[i], (void**)&word);
        add_map(&map, word);
        free_map(word);
    }

    printf("\nWord frequencies:\n");
    WordNode *cur = map;
    while (cur) {
        printf("%s: %zu\n", cur->word, cur->count);
        cur = cur->next;
    }

    free_map(map);
    return 0;
}


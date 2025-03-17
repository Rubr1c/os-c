#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#define MAX_BINS 1000

typedef struct {
  unsigned long bin_width;
  unsigned long bin_count;
  unsigned long *bins;
  unsigned long file_count;
  unsigned long total_size;
} Histogram;

// Initialize a histogram with a specific bin width
Histogram *init_histogram(unsigned long bin_width) {
  Histogram *hist = malloc(sizeof(Histogram));
  if (!hist) {
    perror("Failed to allocate memory for histogram");
    exit(EXIT_FAILURE);
  }

  hist->bin_width = bin_width;
  hist->bin_count = 0;
  hist->bins = calloc(MAX_BINS, sizeof(unsigned long));
  hist->file_count = 0;
  hist->total_size = 0;

  if (!hist->bins) {
    perror("Failed to allocate memory for bins");
    free(hist);
    exit(EXIT_FAILURE);
  }

  return hist;
}

void free_histogram(Histogram *hist) {
  if (hist) {
    if (hist->bins) {
      free(hist->bins);
    }
    free(hist);
  }
}

void add_file_to_histogram(Histogram *hist, unsigned long file_size) {
  unsigned long bin_index = file_size / hist->bin_width;

  if (bin_index >= MAX_BINS) {
    fprintf(stderr, "Warning: File size exceeds maximum bin capacity\n");
    bin_index = MAX_BINS - 1;
  }

  hist->bins[bin_index]++;
  hist->file_count++;
  hist->total_size += file_size;

  if (bin_index >= hist->bin_count) {
    hist->bin_count = bin_index + 1;
  }
}

void traverse_directory(const char *path, Histogram *hist) {
  DIR *dir = opendir(path);
  if (!dir) {
    perror(path);
    return;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    char full_path[PATH_MAX];
    snprintf(full_path, PATH_MAX, "%s/%s", path, entry->d_name);

    struct stat st;
    if (stat(full_path, &st) == -1) {
      perror(full_path);
      continue;
    }

    if (S_ISDIR(st.st_mode)) {
      traverse_directory(full_path, hist);
    } else if (S_ISREG(st.st_mode)) {
      add_file_to_histogram(hist, st.st_size);
    }
  }

  closedir(dir);
}

void print_histogram(Histogram *hist) {
  printf("\nFile Size Histogram (bin width: %lu bytes)\n", hist->bin_width);
  printf("Total files: %lu\n", hist->file_count);
  printf("Total size: %lu bytes\n\n", hist->total_size);

  unsigned long max_bin_value = 0;
  for (unsigned long i = 0; i < hist->bin_count; i++) {
    if (hist->bins[i] > max_bin_value) {
      max_bin_value = hist->bins[i];
    }
  }

  const int max_bar_width = 60;

  for (unsigned long i = 0; i < hist->bin_count; i++) {
    if (hist->bins[i] > 0) {
      unsigned long start_range = i * hist->bin_width;
      unsigned long end_range = (i + 1) * hist->bin_width - 1;

      printf("%8lu - %8lu: %8lu ", start_range, end_range, hist->bins[i]);

      int bar_width = (hist->bins[i] * max_bar_width) / max_bin_value;
      for (int j = 0; j < bar_width; j++) {
        printf("#");
      }
      printf("\n");
    }
  }
}

int main(int argc, char *argv[]) {
  char directory[PATH_MAX] = ".";
  unsigned long bin_width = 1024;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-b") == 0 && i + 1 < argc) {
      bin_width = strtoul(argv[i + 1], NULL, 10);
      i++;
    } else {
      strncpy(directory, argv[i], PATH_MAX - 1);
      directory[PATH_MAX - 1] = '\0';
    }
  }

  Histogram *hist = init_histogram(bin_width);

  printf("Analyzing directory: %s\n", directory);
  printf("Using bin width: %lu bytes\n", bin_width);

  traverse_directory(directory, hist);

  print_histogram(hist);

  free_histogram(hist);

  return 0;
}

# How to run

## Compile

```
gcc main.c -o file_histogram
```

## Run

```
./file_histogram [directory_path] [-b bin_width]
```

## Parameters

- `directory_path`: Path to start traversal (defaults to current directory if not specified)
- `-b bin_width`: Width of each histogram bin in bytes (defaults to 1024 if not specified)

## Examples

```
./file_histogram                       # Analyze current directory with default bin width
./file_histogram /home/user/documents  # Analyze specific directory
./file_histogram -b 4096               # Use 4KB bin width
./file_histogram /var/log -b 2048      # Analyze directory with 2KB bin width
```

# Description

This program recursively traverses a directory structure starting from a specified path.
It records the size of each file found and creates a histogram showing the distribution
of file sizes using configurable bin widths.

The program performs the following operations:

1. Traverses the directory structure recursively
2. Records the size of each regular file encountered
3. Organizes file sizes into bins based on the specified bin width
4. Generates and displays a histogram showing the distribution of file sizes
5. Provides summary statistics including total file count and total size

The histogram output shows each populated bin with its size range, the number of files
in that range, and a visual bar representing the relative frequency.

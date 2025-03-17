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

The program performs a recursive traversal which begins at a designated path.
The program tracks file sizes from each discovery before generating distribution data through a histogram display.

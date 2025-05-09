# How to run

## Compile and link lcrypto library

```bash
gcc main.c -o encrypt -lcrypto
```

## Run

```bash
./encrypt input.txt
```

# Description

The program uses a DES-based hashing method by applying DES encryption 25 times to create DES25. The application gets a salt value and passwords from an input file while executing DES encryption with odd parity and the specified salt to output a 64-bit hash for every entry.

# Input

The input is taken as a command line arg

Format of the input file:

1. salt value as hexadecimal number (e.g., `0x1A2B`).
2. One password per line (10 passwords).

Example `input.txt`:

```
0x1A2B
password1
password2
...
```

# Output

For each password, the program prints a line in the format:

```
Entry {number}: SALT=0x{salt used} HASH={output hash}
```

number is the index of which password it is. salt is the salt value taken from the input file.

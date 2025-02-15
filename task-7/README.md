# How to run

## Compile
```
gcc main.c -o deadlock
```
## Run
```
./deadlock
```

## Input
filename: filename to read from (reliatve path)

# Description
Takes input from a file in the format of:
```
n = process count
m = number of resource types
e = number of resources of type
c = current allocation matrix
r = request matrix
line 1: n m
line 2: vector e[m]
line (3 : 2 + n): matrix c[n][m]
line (3 + n : 2 + 2n) : matrix r[n][m]
```
after this it sees the available resources and then checking each process and what resources it needs.
Then sees if there is enough and marks it as finished and reduces the available resources.
In the end the ones not marked as finished are the ones that are deadlocked.

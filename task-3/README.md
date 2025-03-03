# How to run

## Compile Aging Simulation
```
gcc main.c -o aging
```

## Compile Input Generator
```
gcc generate_input.c -o generate_input
```

## Run

### Generate Input File
```
./generate_input
```
-  This will generate a file named input.txt in the current directory.

### Run Main
```
./aging
```

# Description
Takes input from a file for the page references.
Simulates a paging system using the aging algorithm. 
The number of page frames is a parameter. the program outputs the number of page faults 
per 1000 memory references as a function of the number of page frames available.

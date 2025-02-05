# How to run

## Compile and link pthread library

```
gcc main.c -o t-word-counter -lpthread
```

## Run

```
./t-word-counter
```

## Input
-  filename: filename to read from (reliatve path)
-  thread/segment count: number of threads 

# Description

Splits the file depending on the file size into N segments then if there is any leftover bytes it gives one to each thread util it finishes. 
Each thread will have a start and an end and it has to make sure that it is not in the middle of a word so it adjusts the start and end byte accordingly.
Each thread will display the thread id and the range of bytes it read and some may overlap due to the ajustments but at the end it will not count anything incorrectly.
Each word is saved as a key value pair in a simple map sructure that uses nodes and it checks if the word exists it increments the count else it creates a new node for that word.
In the end it gets all the maps from each thread and adds it into one map and prints it to the terminal.

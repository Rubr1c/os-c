# How to run

## Compile the source file

```
gcc main.c -o cpu_scheduler
```

## Run the CPU scheduler simulator

```
./cpu_scheduler
```

# Description

This program simulates three CPU scheduling algorithms:

1. First-Come, First-Served (FCFS)
2. Shortest Job First (SJF)
3. Round Robin (RR)

The program compares the average waiting time for each algorithm.

## Input

- Number of processes
- Arrival time and burst time for each process
- Time quantum for Round Robin algorithm

## Example

```
Enter the number of processes: 3

Enter process details:

Process 1:
Arrival time: 0
Burst time: 5

Process 2:
Arrival time: 1
Burst time: 3

Process 3:
Arrival time: 2
Burst time: 1

Enter time quantum for Round Robin: 2

=== CPU Scheduling Simulation ===

1. First-Come, First-Served (FCFS)

FCFS Results:
Process Arrival Burst  Waiting Completion
P1      0       5      0       5
P2      1       3      4       8
P3      2       1      6       9
Average Waiting Time: 3.33

2. Shortest Job First (SJF)

SJF Results:
Process Arrival Burst  Waiting Completion
P1      0       5      0       5
P3      2       1      3       6
P2      1       3      5       9
Average Waiting Time: 2.67

3. Round Robin (RR) - Time Quantum = 2

Round Robin Results:
Process Arrival Burst  Waiting Completion
P1      0       5      4       9
P2      1       3      4       8
P3      2       1      2       5
Average Waiting Time: 3.33
```

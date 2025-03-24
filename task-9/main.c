#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 100

typedef struct {
  int id;
  int arrival_time;
  int burst_time;
  int remaining_time;
  int waiting_time;
  int completion_time;
  bool completed;
} Process;

void sort_by_arrival(Process processes[], int n) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if (processes[j].arrival_time > processes[j + 1].arrival_time) {
        Process temp = processes[j];
        processes[j] = processes[j + 1];
        processes[j + 1] = temp;
      }
    }
  }
}

void sjf(Process processes[], int n) {
  int current_time = 0;
  int completed = 0;

  while (completed < n) {
    int shortest_job_index = -1;
    int shortest_burst_time = __INT_MAX__;

    for (int i = 0; i < n; i++) {
      if (!processes[i].completed &&
          processes[i].arrival_time <= current_time) {
        if (processes[i].burst_time < shortest_burst_time) {
          shortest_burst_time = processes[i].burst_time;
          shortest_job_index = i;
        }
      }
    }

    if (shortest_job_index == -1) {
      current_time++;
      continue;
    }

    processes[shortest_job_index].waiting_time =
        current_time - processes[shortest_job_index].arrival_time;
    current_time += processes[shortest_job_index].burst_time;
    processes[shortest_job_index].completion_time = current_time;
    processes[shortest_job_index].completed = true;
    completed++;
  }
}

void round_robin(Process processes[], int n, int time_quantum) {
  sort_by_arrival(processes, n);

  int current_time = 0;
  int completed = 0;

  while (completed < n) {
    bool did_work = false;

    for (int i = 0; i < n; i++) {
      if (processes[i].completed) {
        continue;
      }

      if (processes[i].arrival_time > current_time) {
        continue;
      }

      did_work = true;

      int time_slice = (processes[i].remaining_time < time_quantum)
                           ? processes[i].remaining_time
                           : time_quantum;

      current_time += time_slice;
      processes[i].remaining_time -= time_slice;

      if (processes[i].remaining_time <= 0) {
        processes[i].completed = true;
        processes[i].completion_time = current_time;
        completed++;
      }
    }

    if (!did_work) {
      current_time++;
    }
  }

  for (int i = 0; i < n; i++) {
    processes[i].waiting_time = processes[i].completion_time -
                                processes[i].arrival_time -
                                processes[i].burst_time;
  }
}

void copy_processes(Process source[], Process destination[], int n) {
  for (int i = 0; i < n; i++) {
    destination[i] = source[i];
  }
}

float calc_avg_waiting_time(Process processes[], int n) {
  float total_waiting_time = 0;

  for (int i = 0; i < n; i++) {
    total_waiting_time += processes[i].waiting_time;
  }

  return total_waiting_time / n;
}

void fcfs(Process processes[], int n) {
  sort_by_arrival(processes, n);

  int current_time = 0;

  for (int i = 0; i < n; i++) {
    if (current_time < processes[i].arrival_time) {
      current_time = processes[i].arrival_time;
    }

    processes[i].waiting_time = current_time - processes[i].arrival_time;
    current_time += processes[i].burst_time;
    processes[i].completion_time = current_time;
    processes[i].completed = true;
  }
}

void print_results(Process processes[], int n, const char *algorithm_name) {
  printf("\n%s Results:\n", algorithm_name);
  printf("Process\tArrival\tBurst\tWaiting\tCompletion\n");

  for (int i = 0; i < n; i++) {
    printf("P%d\t%d\t%d\t%d\t%d\n", processes[i].id, processes[i].arrival_time,
           processes[i].burst_time, processes[i].waiting_time,
           processes[i].completion_time);
  }

  printf("Average Waiting Time: %.2f\n", calc_avg_waiting_time(processes, n));
}

int main() {
  int n;
  int time_quantum;
  Process original_processes[MAX_PROCESSES];
  Process processes[MAX_PROCESSES];

  printf("Enter the number of processes: ");
  scanf("%d", &n);

  if (n <= 0 || n > MAX_PROCESSES) {
    printf("Invalid number of processes\n");
    return 1;
  }

  printf("\nEnter process details:\n");
  for (int i = 0; i < n; i++) {
    original_processes[i].id = i + 1;
    printf("\nProcess %d:\n", i + 1);

    printf("Arrival time: ");
    scanf("%d", &original_processes[i].arrival_time);

    printf("Burst time: ");
    scanf("%d", &original_processes[i].burst_time);

    if (original_processes[i].burst_time <= 0) {
      printf("Burst time must be greater than 0\n");
      i--;
      continue;
    }

    original_processes[i].remaining_time = original_processes[i].burst_time;
    original_processes[i].waiting_time = 0;
    original_processes[i].completion_time = 0;
    original_processes[i].completed = false;
  }

  printf("\nEnter time quantum for Round Robin: ");
  scanf("%d", &time_quantum);

  if (time_quantum <= 0) {
    printf("Time quantum must be greater than 0\n");
    time_quantum = 2;
  }

  printf("\n=== CPU Scheduling Simulation ===\n");

  copy_processes(original_processes, processes, n);
  printf("\n1. First-Come, First-Served (FCFS)\n");
  fcfs(processes, n);
  print_results(processes, n, "FCFS");

  copy_processes(original_processes, processes, n);
  printf("\n2. Shortest Job First (SJF)\n");
  sjf(processes, n);
  print_results(processes, n, "SJF");

  copy_processes(original_processes, processes, n);
  printf("\n3. Round Robin (RR) - Time Quantum = %d\n", time_quantum);
  round_robin(processes, n, time_quantum);
  print_results(processes, n, "Round Robin");

  return 0;
}

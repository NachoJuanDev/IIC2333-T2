#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#include "process.h"

Process *process_init(pid_t pid, char *name, int deadline, int start_time, int state)
{
  Process *process = malloc(sizeof(Process));

  process->pid = pid;
  strcpy(process->name, name);
  process->deadline = deadline;
  process->start_time = start_time;
  process->state = state;
  process->cpu = -1;
  process->selections = 0;
  process->interruptions = 0;
  process->turnaround = 0;
  process->response = 0;
  process->waiting = 0;

  return process;
}

void process_add_behavior(Process *process, int n_behavior, int *behavior)
{
  process->n_behavior = n_behavior;
  process->behavior = calloc(n_behavior, sizeof(int));
  for (int i = 0; i < n_behavior; i++)
  {
    process->behavior[i] = behavior[i];
  }
}

void process_check(Process *process)
{
  if (process->state == RUNNING)
  {
    printf("Estado 0: RUNNING \n");
  }
  if (process->state == READY)
  {
    printf("Estado 1: READY \n");
  }
  if (process->state == WAITING)
  {
    printf("Estado 2: WAITING \n");
  }
  if (process->state == FINISHED)
  {
    printf("Estado 3: FINISHED \n");
  }
}

void free_process(Process *process)
{
  free(process->behavior);
  free(process);
}

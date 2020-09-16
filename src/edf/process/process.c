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
  process->avance_rafaga = 0;
  process->current_rafaga = 0;
  process->n_veces_agregado = 0;
  process->n_veces_interrumpido = 0;

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

void process_check(Process *process, int time)
{
  if (process->state == RUNNING)
  {
    process->avance_rafaga++;
    if (process->avance_rafaga == process->behavior[process->current_rafaga])
    {
      process->avance_rafaga = 0;
      process->current_rafaga++;
      if (process->current_rafaga == process->n_behavior)
      {
        process->state = FINISHED;
        process->finish_time = time;
        process->finished_correctly = process->deadline >= time;
      }
      else
      {
        process->state = WAITING;
      }
    }
  }
  else if (process->state == READY)
  {
    process->waiting++;
  }
  else if (process->state == WAITING)
  {
    process->avance_rafaga++;
    process->waiting++;
    if (process->avance_rafaga == process->behavior[process->current_rafaga])
    {
      process->avance_rafaga = 0;
      process->current_rafaga++;
      process->state = READY;
    }
  }
}

void free_process(Process *process)
{
  free(process->behavior);
  free(process);
}

Process *process_compare(Process *process1, Process *process2)
{
  if (process1->deadline < process2->deadline)
  {
    return process1;
  }

  if (process1->deadline == process2->deadline)
  {
    if (process1->pid <= process2->pid)
    {
      return process1;
    }
    return process2;
  }
  return process2;
}

Process *process_worst(Process *process1, Process *process2)
{
  Process *process = process_compare(process1, process2);
  if (process == process1)
  {
    return process2;
  }
  return process1;
}

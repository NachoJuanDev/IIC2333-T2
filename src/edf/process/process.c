#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#include "process.h"

Process *process_init(pid_t pid, char *name, int deadline, unsigned int start_time, int state)
{
  Process *process = malloc(sizeof(Process));

  process->pid = pid;
  strcpy(process->name, name);
  process->deadline = deadline;
  process->start_time = start_time;
  process->state = state;
  process->cpu = -1;

  return process;
}

void process_add_behavior(Process *process,unsigned int n_behavior,unsigned int *behavior)
{
  process->n_behavior = n_behavior;
  process->behavior = malloc(sizeof(unsigned int) * n_behavior);
  for (int i = 0; i < n_behavior; i++)
  {
    process->behavior[i] = behavior[i];
  }
}

void free_process(Process *process)
{
  free(process->behavior);
  free(process);
}

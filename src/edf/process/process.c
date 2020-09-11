#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#include "process.h"

Process *process_init(pid_t pid, char *name, int priority, int state)
{
  Process *process = malloc(sizeof(Process));

  process->pid = pid;
  strcpy(process->name, name);
  process->priority = priority;
  process->state = state;

  return process;
}

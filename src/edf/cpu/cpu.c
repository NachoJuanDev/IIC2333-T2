#include <sys/types.h>
#include <stdlib.h>

#include "cpu.h"
#include "../process/process.h"

CPU *cpu_init(int n_cores)
{
  CPU *cpu = malloc(sizeof(CPU));
  cpu->n_cores = n_cores;
  cpu->cores = calloc(n_cores, sizeof(CORE));

  for (int i = 0; i < n_cores; i++)
  {
    CORE *core = malloc(sizeof(CORE));
    core->n_core = i;
    core->proceso_actual = 0;

    cpu->cores[i] = core;
  }
  return cpu;
}

int cpu_core_empty(CPU *cpu)
{
  for (int i = 0; i < cpu->n_cores; i++)
  {
    CORE *core = cpu->cores[i];
    if (!core->proceso_actual)
    {
      return i;
    }
  }
  return -1;
}

void cpu_add_process(CPU *cpu, int n_core, Process *process)
{
  CORE *core = cpu->cores[n_core];
  core->proceso_actual = process;
  process->cpu = n_core;
}

void free_cpu(CPU *cpu)
{
  for (int i = 0; i < cpu->n_cores; i++)
  {
    free(cpu->cores[i]);
  }
  free(cpu->cores);
  free(cpu);
}

int core_free_finished_process(CPU *cpu, int n_core)
{
  if (cpu->cores[n_core]->proceso_actual)
  {
    if (cpu->cores[n_core]->proceso_actual->state == FINISHED)
    {
      cpu->cores[n_core]->proceso_actual = NULL;
      return 1;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    return 2;
  }
}
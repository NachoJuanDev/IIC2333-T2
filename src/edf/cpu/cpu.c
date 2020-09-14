#include <sys/types.h>
#include <stdlib.h>

#include "cpu.h"
#include "../process/process.h"

CPU *cpu_init(int n_cores)
{
  CPU *cpu = malloc(sizeof(CPU));
  cpu->n_cores = n_cores;
  cpu->cores = malloc(sizeof(CORE) * n_cores);

  for (int i = 0; i < n_cores; i++)
  {
    CORE *core = malloc(sizeof(CORE));
    core->n_core = i;

    core->process_pid = 0;
    cpu->cores[i] = core;
  }
  return cpu;
}

int cpu_core_empty(CPU *cpu)
{
  for (int i = 0; i < cpu->n_cores; i++)
  {
    CORE *core = cpu->cores[i];
    if (!core->process_pid)
    {
      return i;
    }
  }
  return -1;
}

void cpu_add_process(CPU *cpu, int n_core, Process *process)
{
  CORE *core = cpu->cores[n_core];
  core->process_pid = process->pid;
  core->proceso_actual = process;
  process->cpu = n_core;
}

void free_cpu(CPU *cpu)
{
  for (int i = 0; i < cpu->n_cores; i++)
  {
    free(cpu->cores[i]);
  }
  free(cpu);
}

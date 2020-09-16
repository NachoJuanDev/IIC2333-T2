#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

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

void cpu_add_process(CPU *cpu, int n_core, Process *process, int time)
{
  CORE *core = cpu->cores[n_core];
  core->proceso_actual = process;
  process->cpu = n_core;
  process->state = RUNNING;
  if (!process->n_veces_agregado)
  {
    process->first_time = time;
  }
  process->n_veces_agregado++;
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

void core_free_waiting_process(CPU *cpu)
{
  for (int n_core = 0; n_core < cpu->n_cores; n_core++)
  {
    if (cpu->cores[n_core]->proceso_actual)
    {
      if (cpu->cores[n_core]->proceso_actual->state == WAITING)
      {
        cpu->cores[n_core]->proceso_actual->cpu = -1;
        cpu->cores[n_core]->proceso_actual = NULL;
      }
    }
  }
}

void core_free_finished_process(CPU *cpu)
{
  for (int n_core = 0; n_core < cpu->n_cores; n_core++)
  {
    if (cpu->cores[n_core]->proceso_actual)
    {
      if (cpu->cores[n_core]->proceso_actual->state == FINISHED)
      {
        cpu->cores[n_core]->proceso_actual->cpu = -1;
        cpu->cores[n_core]->proceso_actual = NULL;
      }
    }
  }
}

Process *cpu_worst_process(CPU *cpu)
{
  Process *proceso_aux = cpu->cores[0]->proceso_actual;

  for (int i = 0; i < cpu->n_cores; i++)
  {
    if (cpu->cores[i]->proceso_actual != NULL)
    {
      proceso_aux = process_worst(proceso_aux, cpu->cores[i]->proceso_actual);
    }
  }
  return proceso_aux;
}

void cpu_interrumpt_process(CPU *cpu, int core)
{
  Process *process = cpu->cores[core]->proceso_actual;
  process->cpu = -1;
  process->n_veces_interrumpido++;
  cpu->cores[core]->proceso_actual = NULL;
}

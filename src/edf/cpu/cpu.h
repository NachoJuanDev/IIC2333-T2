#pragma once
#include <sys/types.h>

#include "../process/process.h"

struct cpu_core;
typedef struct cpu_core CORE;

struct cpu_core
{
  /** Número del núcleo */
  int n_core;

  /** puntero proceso */
  Process *proceso_actual;
};

struct cpu;
typedef struct cpu CPU;

struct cpu
{
  /** Número de núcleos */
  int n_cores;

  /** Núcleos */
  CORE **cores;
};

/** Inicia una CPU con su número de cores */
CPU *cpu_init(int n_cores);

/** Retorna el número de un core vacío, -1 si no hay vacíos */
int cpu_core_empty(CPU *cpu);

/** Añade un proceso a un core de la cpu */
void cpu_add_process(CPU *cpu, int n_core, Process *process);

/** Libera la memoria utilizada por una CPU */
void free_cpu(CPU *cpu);

/** Revisa si los procesos alojado en cpu para ver si terminaron y los quita */
void core_free_finished_process(CPU *cpu);

/* Encuentra el peor proceso de la cpu y devuelve el indice del core*/
int cpu_worst_process(CPU *cpu);
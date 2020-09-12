#pragma once
#include <sys/types.h>

struct process;
typedef struct process Process;

struct process
{
  /** PID del proceso */
  pid_t pid;

  /** Nombre del proceso */
  char name[32];

  /** Prioridad del proceso */
  int deadline;

  /** Tiempo de llegada */
  unsigned int start_time;

  /** Estado del proceso */
  int state;

  /** CPU */
  int cpu;

  /** Número de comportamientos */

  unsigned int n_behavior;

  /** Comportamiento del proceso */
  unsigned int *behavior;

};

typedef enum state
{
  RUNNING,
  READY,
  WAITING,
  FINISHED
} State;

/** Inicia un proceso */
Process *process_init(pid_t pid, char *name, int deadline, unsigned int start_time, int state);

/** Añade los comportamientos a un proceso */
void process_add_behavior(Process *process, unsigned int n_behavior, unsigned int *behavior);

/** Libera la memoria utilizada por un proceso */
void free_process(Process *process);

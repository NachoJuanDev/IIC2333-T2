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
  int priority;

  /** Estado del proceso */
  int state;

  /** Más atributos */
};

typedef enum state
{
  RUNNING,
  READY,
  WAITING,
  FINISHED
} State;

/** Inicia un proceso */
Process *process_init(pid_t pid, char *name, int priority, int state);

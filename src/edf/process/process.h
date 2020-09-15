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
  int start_time;

  /** Estado del proceso */
  int state;

  /** CPU en la que se encuentra*/
  int cpu;

  /** Número de comportamientos */
  int n_behavior;

  /** Comportamiento del proceso */
  int *behavior;

  /** Número de veces que el proceso fue elegido */
  int selections;

  /** Número de veces que el proceso fue interrumpido */
  int interruptions;

  /** El turnaround time */
  int turnaround;

  /** El response time */
  int response;

  /** El waiting time */
  int waiting;

  /** Si el proceso terminó o no antes de su deadline, 1 implica que sı́ y 0 si no. */
  int finished_correctly;

  /** Indicador de cuál es la ráfaga actual */
  int n_rafaga;

  /** Indicador de avance de la ráfaga actual */
  int avance_rafaga;

  /** Indicador de cuantas veces fue agregado a la cpu */
  int n_veces_agregado;
};

typedef enum state
{
  RUNNING,  // Proceso corriendo en un CORE de CPU
  READY,    // Proceso listo para ser asignado a un CORE de CPU
  WAITING,  // Proceso en espera de I/O
  FINISHED, // Proceso Finalizado
  INACTIVE  // Por complitud. No ha sido cargado a la simulación y Se encuentran en la cola process
} State;

/** Inicia un proceso */
Process *process_init(pid_t pid, char *name, int deadline, int start_time, int state);

/** Añade los comportamientos a un proceso */
void process_add_behavior(Process *process, int n_behavior, int *behavior);

/** Libera la memoria utilizada por un proceso */
void free_process(Process *process);

/** Revisa el proceso y se actualzia según su estado */
void process_check(Process *process);

/** Revisa cual proceso el masyor y retorna el mayor. */
Process process_compare(Process *process1, Process *process2)

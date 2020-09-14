#pragma once

#include "../process/process.h"

struct queue;
typedef struct queue Queue;

struct queue
{
  /** Cola de procesos */
  Process **process;

  Process **finished;
  Process **waiting;
  Process **ready;
  Process **running;

  /** Número de procesos */
  int n_process;
  /*  numero de procesos terminados */
  int n_finished;
  /*  numero de procesos en espera */
  int n_waiting;
  /*  numero de procesos en ready */
  int n_ready;
  /*  numero de procesos corriendo */
  int n_running;
};

/** Inicializa una cola vacía */
Queue *queue_init(int n_process);

/** Destruye una cola y sus procesos */
void free_queue(Queue *queue);

/* hace que los procesos chequeen sus estados y se actualicen */
void queue_process_checking(Queue *queue);

/* Recorre la lista y toma todos los procesos que inician en el tiempo
actual y los mueve a ready en el último espacio indicado por n_ready*/
void from_nothing_to_ready(Queue *queue, int tiempo);

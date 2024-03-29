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
  /**  numero de procesos terminados */
  int n_finished;
  /**  numero de procesos en espera */
  int n_waiting;
  /**  numero de procesos en ready */
  int n_ready;
  /**  numero de procesos corriendo */
  int n_running;
};

/** Inicializa una cola vacía */
Queue *queue_init(int n_process);

/** Destruye una cola y sus procesos */
void free_queue(Queue *queue);

/** hace que los procesos chequeen sus estados y se actualicen */
void queue_process_checking(Queue *queue, int time);

/** Mueve un proceso de la cola source en la posición pos_source a la cola target */
void queue_move_process(Queue *queue, State source, int pos_source, State target);

/** Mueve un proceso de la cola source a la cola target. Recibe el puntero del proceso */
void queue_move_process_pointer(Queue *queue, State source, Process *process, State target);

/** Recorre la cola process y mueve todos los procesos que inician en el tiempo
actual y los mueve a la cola ready */
void queue_inactive_to_ready(Queue *queue, int tiempo);

/** Mueve los procesos que finalizan su cpu burst de la cola running a la cola waiting */
void queue_running_to_waiting(Queue *queue);

/** Mueve los procesos finalizados de la cola running a la cola finished */
void queue_running_to_finished(Queue *queue);

/** Mueve los procesos ready de la cola waiting a la cola ready */
void queue_waiting_to_ready(Queue *queue);

/* Encuentra el mejor proceso de la cola ready, se asume que hay mínimo 1 en la cola ready */
Process *queue_best_process(Queue *queue);

void queue_print(Process **cola, int n_cola);

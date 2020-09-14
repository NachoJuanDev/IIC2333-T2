#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "queue.h"

Queue *queue_init(int n_process)
{
  Queue *queue = malloc(sizeof(Queue));                 /* creamos el queue*/
  queue->process = calloc(n_process, sizeof(Process));  /*creamos lista de procesos sin iniciar*/
  queue->finished = calloc(n_process, sizeof(Process)); /*creamos lista de procesos terminados*/
  queue->waiting = calloc(n_process, sizeof(Process));  /*creamos lista de procesos en espera*/
  queue->ready = calloc(n_process, sizeof(Process));    /*creamos lista de procesos ready*/
  queue->running = calloc(n_process, sizeof(Process));  /*creamos lista de procesos corriendo*/
  queue->n_process = n_process;
  queue->n_finished = 0;
  queue->n_waiting = 0;
  queue->n_ready = 0;
  queue->n_running = 0;

  return queue;
}

/* hace que los procesos chequeen sus estados y se actualicen */
void queue_process_checking(Queue *queue)
{
  for (int i = 0; i < queue->n_process; i++)
  {
    process_check(queue->process[i]);
  }
}

void from_nothing_to_ready(Queue *queue, int tiempo)
{
  for (int i = 0; i < queue->n_process; i++)
  {
    if (queue->process[i]->start_time == tiempo)
    {
      queue->ready[queue->n_ready] = queue->process[i];
      queue->n_ready += 1;
    }
  }
}

void free_queue(Queue *queue)
{
  /** 
   * No se deben quitar nuna las referencias de la cola process,
   * solo se deben copiar a las demás colas y las demás colas pueden borrar y mover sus referencias
  */

  for (int i = 0; i < queue->n_process; i++)
  {
    free_process(queue->process[i]);
  }

  free(queue->process);
  free(queue->finished);
  free(queue->waiting);
  free(queue->ready);
  free(queue->running);
  free(queue);
}

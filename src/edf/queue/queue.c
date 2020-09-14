#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "queue.h"

Queue *queue_init(int n_process)
{
  Queue *queue = malloc(sizeof(Queue));                 /* creamos el queue */
  queue->process = calloc(n_process, sizeof(Process));  /* creamos lista de procesos sin iniciar */
  queue->finished = calloc(n_process, sizeof(Process)); /* creamos lista de procesos terminados */
  queue->waiting = calloc(n_process, sizeof(Process));  /* creamos lista de procesos en espera */
  queue->ready = calloc(n_process, sizeof(Process));    /* creamos lista de procesos ready */
  queue->running = calloc(n_process, sizeof(Process));  /* creamos lista de procesos corriendo */
  queue->n_process = n_process;
  queue->n_finished = 0;
  queue->n_waiting = 0;
  queue->n_ready = 0;
  queue->n_running = 0;

  return queue;
}

void queue_process_checking(Queue *queue)
{
  for (int i = 0; i < queue->n_process; i++)
  {
    process_check(queue->process[i]);
  }
}

void queue_move_process(Queue *queue, State source, int pos_source, State target)
{
  /** Para evitar espacios vacíos
   * Guardo en process el proceso en la posición pos_source de la lista source
   * Muevo el último proceso de la cola source a la posición pos_source
   * Resto 1 al contador de la cola source 
   * 
   * INACTIVE hace referencia a la cola process. Esta cola siempre debe tener todos los procesos
   * por lo que no se deben quitar procesos de su cola
  */

  Process *process;

  /** Busco la referencia al proceso */

  if (source == RUNNING)
  {
    process = queue->running[pos_source];
    queue->n_running--;
    queue->running[pos_source] = queue->running[queue->n_running];
    queue->running[queue->n_running] = NULL;
  }
  else if (source == READY)
  {
    process = queue->ready[pos_source];
    queue->n_ready--;
    queue->ready[pos_source] = queue->ready[queue->n_ready];
    queue->ready[queue->n_ready] = NULL;
  }
  else if (source == WAITING)
  {
    process = queue->waiting[pos_source];
    queue->n_waiting--;
    queue->waiting[pos_source] = queue->waiting[queue->n_waiting];
    queue->waiting[queue->n_waiting] = NULL;
  }
  else if (source == FINISHED)
  {
    process = queue->finished[pos_source];
    queue->n_finished--;
    queue->finished[pos_source] = queue->finished[queue->n_finished];
    queue->finished[queue->n_finished] = NULL;
  }
  else if (source == INACTIVE)
  {
    process = queue->process[pos_source];
  }

  /** Guardo la referencia al proceso */

  if (target == RUNNING)
  {
    queue->running[queue->n_running] = process;
    queue->n_running++;
  }
  else if (target == READY)
  {
    queue->ready[queue->n_ready] = process;
    queue->n_ready++;
  }
  else if (target == WAITING)
  {
    queue->waiting[queue->n_waiting] = process;
    queue->n_waiting++;
  }
  else if (target == FINISHED)
  {
    queue->finished[queue->n_finished] = process;
    queue->n_finished++;
  }
}

void queue_inactive_to_ready(Queue *queue, int tiempo)
{
  for (int i = 0; i < queue->n_process; i++)
  {
    if (queue->process[i]->start_time == tiempo)
    {
      queue_move_process(queue, INACTIVE, i, READY);
    }
  }
}

void queue_running_to_finished(Queue *queue)
{
  for (int i = 0; i < queue->n_running; i++)
  {
    if (queue->running[i]->state == FINISHED)
    {
      queue_move_process(queue, RUNNING, i, FINISHED);
    }
  }
}

void queue_waiting_to_ready(Queue *queue)
{
  for (int i = 0; i < queue->n_waiting; i++)
  {
    if (queue->waiting[i]->state == READY)
    {
      queue_move_process(queue, WAITING, i, READY);
    }
  }
}

void free_queue(Queue *queue)
{
  /** 
   * No se deben quitar nunca las referencias de la cola process,
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

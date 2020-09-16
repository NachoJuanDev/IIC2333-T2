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

void queue_process_checking(Queue *queue, int time)
{
  for (int i = 0; i < queue->n_process; i++)
  {
    process_check(queue->process[i], time);
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

void queue_move_process_pointer(Queue *queue, State source, Process *process, State target)
{
  if (source == RUNNING)
  {
    for (int i = 0; i < queue->n_running; i++)
    {
      if (queue->running[i]->pid == process->pid)
      {
        queue_move_process(queue, source, i, target);
        break;
      }
    }
  }
  else if (source == READY)
  {
    for (int i = 0; i < queue->n_ready; i++)
    {
      if (queue->ready[i]->pid == process->pid)
      {
        queue_move_process(queue, source, i, target);
        break;
      }
    }
  }
  else if (source == WAITING)
  {
    for (int i = 0; i < queue->n_waiting; i++)
    {
      if (queue->waiting[i]->pid == process->pid)
      {
        queue_move_process(queue, source, i, target);
        break;
      }
    }
  }
  else if (source == INACTIVE)
  {
    for (int i = 0; i < queue->n_process; i++)
    {
      if (queue->process[i]->pid == process->pid)
      {
        queue_move_process(queue, source, i, target);
        break;
      }
    }
  }
}

void queue_inactive_to_ready(Queue *queue, int tiempo)
{
  int i = 0;
  while (i < queue->n_process)
  {
    if (queue->process[i]->start_time == tiempo)
    {
      queue->process[i]->state = READY;
      queue_move_process(queue, INACTIVE, i, READY);
    }
    i++;
  }
}

void queue_running_to_waiting(Queue *queue)
{
  int i = 0;
  int max = queue->n_running;
  while (i < max)
  {
    if (queue->running[i]->state == WAITING)
    {
      max--;
      queue_move_process(queue, RUNNING, i, WAITING);
    }
    else
    {
      i++;
    }
  }
}

void queue_running_to_finished(Queue *queue)
{
  int i = 0;
  int max = queue->n_running;
  while (i < max)
  {
    if (queue->running[i]->state == FINISHED)
    {
      max--;
      queue_move_process(queue, RUNNING, i, FINISHED);
    }
    else
    {
      i++;
    }
  }
}

void queue_waiting_to_ready(Queue *queue)
{
  int i = 0;
  int max = queue->n_waiting;
  while (i < max)
  {
    if (queue->waiting[i]->state == READY)
    {
      max--;
      queue_move_process(queue, WAITING, i, READY);
    }
    else
    {
      i++;
    }
  }
}

Process *queue_best_process(Queue *queue)
{
  Process *proceso_aux = queue->ready[0];
  for (int i = 1; i < queue->n_ready; i++)
  {
    proceso_aux = process_compare(proceso_aux, queue->ready[i]);
  }
  return proceso_aux;
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

void queue_print(Process **cola, int n_cola)
{
  for (int i = 0; i < n_cola; i++)
  {
    printf("i: %i, Name: %s, State: %i, Rafaga: %i, Avance Rafaga: %i, n_Rafagas: %i \n",
           i, cola[i]->name, cola[i]->state, cola[i]->current_rafaga, cola[i]->avance_rafaga ,cola[i]->n_behavior);
  }
}
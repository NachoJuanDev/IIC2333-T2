#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <stdlib.h>
#include <sys/types.h>

#include <unistd.h>
#include <sys/wait.h>

#include <errno.h>
#include <signal.h>

#include <time.h>
#include <sys/time.h>

#include "queue.h"

Queue *queue_init(int n_process)
{
  Queue *queue = malloc(sizeof(Queue));                  /* creamos el queue*/
  queue->process = malloc(sizeof(Process) * n_process);  /*creamos lista de procesos sin iniciar*/
  queue->finished = malloc(sizeof(Process) * n_process); /*creamos lista de procesos terminados*/
  queue->waiting = malloc(sizeof(Process) * n_process);  /*creamos lista de procesos en espera*/
  queue->ready = malloc(sizeof(Process) * n_process);    /*creamos lista de procesos ready*/
  queue->running = malloc(sizeof(Process) * n_process);  /*creamos lista de procesos corriendo*/
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

void queue_destroy(Queue *queue)
{
  /* todo proceso a eliminar estará en finished, porque ahí terminará el programa */
  for (int i = 0; i < queue->n_finished; i++) // cambié el límite por otra variable para tener consistencia
  {
    free_process(queue->finished[i]);
  }

  /* para efectos de prueba, dejaré el código para borrar procesos generales*/
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

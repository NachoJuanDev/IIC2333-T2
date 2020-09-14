#include <stdlib.h>

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

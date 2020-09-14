#include <stdlib.h>

#include "queue.h"

Queue *queue_init(int n_process)
{
  Queue *queue = malloc(sizeof(Queue));
  queue->process = malloc(sizeof(Process) * n_process);
  queue->n_process = n_process;

  return queue;
}

void queue_destroy(Queue *queue)
{
  for (int i = 0; i < queue->n_process; i++)
  {
    free_process(queue->process[i]);
  }
  free(queue->process);
  free(queue);
}

#include <stdlib.h>

#include "queue.h"

Queue *queue_init()
{
  Queue *queue = malloc(sizeof(Queue));

  return queue;
}

void queue_destroy(Queue *queue)
{
  for (int i = 0; i < 255; i++)
  {
    free(queue->process[i]);
  }

  free(queue);
}

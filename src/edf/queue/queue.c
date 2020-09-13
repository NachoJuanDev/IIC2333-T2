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
    if (!queue->process[i])
    {
      break;
    }

    free_process(queue->process[i]);
  }

  free(queue);
}

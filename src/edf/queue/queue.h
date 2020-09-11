#pragma once

#include "../process/process.h"

struct queue;
typedef struct queue Queue;

struct queue
{
  /** Cola de procesos */
  Process *process[255];
};

/** Inicializa una cola vacía */
Queue *queue_init();

/** Destruye una cola y sus procesos */
void queue_destroy(Queue *queue);

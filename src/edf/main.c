
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#include "process/process.h"
#include "queue/queue.h"
#include "cpu/cpu.h"

Queue *populate_system(char *fileName)
{
  FILE *file = fopen(fileName, "r");
  char line[2048];

  int numero_procesos = atoi(fgets(line, sizeof(line), file)); /* El número de procesos */

  Queue *lista_queue = queue_init(numero_procesos);
  int proceso_actual = 0;

  while (fgets(line, sizeof(line), file))
  {
    char *token;

    /* obtenemos el nombre */
    token = strtok(line, " ");
    char nombre_proceso[255] = "hola";
    strcpy(nombre_proceso, token);

    /* obtenemos el PID */
    token = strtok(NULL, " ");
    pid_t pid_proceso = atoi(token);

    /* obtenemos el tiempo de inicio */
    token = strtok(NULL, " ");
    int tiempo_inicio = atoi(token);

    /* obtenemos el deadline */
    token = strtok(NULL, " ");
    int deadline = atoi(token);

    /* obtenemos el numero de rafagas */
    token = strtok(NULL, " ");
    int numero_rafagas = 2 * atoi(token) - 1;

    /* obtenemos las rafagas */
    int rafagas[255];
    for (int i = 0; i < numero_rafagas; i++)
    {
      token = strtok(NULL, " ");
      rafagas[i] = atoi(token);
    }

    /* creamos el proceso y lo poblamos con todos los datos de antes */
    Process *proceso = process_init(pid_proceso, nombre_proceso, deadline, tiempo_inicio, INACTIVE);

    process_add_behavior(proceso, numero_rafagas, rafagas);

    lista_queue->process[proceso_actual] = proceso;
    proceso_actual += 1;
  }

  fclose(file);

  return lista_queue;
}

void write_output(char *outputFile, Queue *queue)
{
  FILE *fp;
  fp = fopen(outputFile, "w");

  for (int i = 0; i < queue->n_process; i++)
  {
    Process *process = queue->process[i];
    process->turnaround = process->finish_time - process->start_time;
    process->response = process->first_time - process->start_time;

    fprintf(fp, "%s,%i,%i,%i,%i,%i,%i\n",
            process->name, process->n_veces_agregado, process->n_veces_interrumpido,
            process->turnaround, process->response, process->waiting, process->finished_correctly);
  }

  fclose(fp);
}

void run(char *fileName, char *outputFile, int n_cpu)
{

  Queue *queue = populate_system(fileName);
  CPU *cpu = cpu_init(n_cpu);

  bool run = true;
  int time = 0;
  while (run)
  {
    printf("Time: %i\n", time);
    /** Paso 0: Actualizo los estados de los procesos */
    queue_process_checking(queue, time);

    /** Paso 1: 
     * Revisar procesos que terminaron en la cpu
     * Liberar procesos finalizados de la cpu
     * Liberamos procesos en waiting de la cpu
     * Revisar condición de termino
     * */
    queue_running_to_finished(queue);
    queue_running_to_waiting(queue);
    core_free_finished_process(cpu);
    core_free_waiting_process(cpu);
    if (queue->n_finished == queue->n_process)
    {
      break;
    }

    /** Paso 2: Buscar procesos que comiencen en el tiempo actual */
    queue_inactive_to_ready(queue, time);

    /** Paso 3: Buscar procesos ready en la cola waiting */
    queue_waiting_to_ready(queue);

    /** Paso 4: Scheduler */
    bool run_scheduler = true;
    while (run_scheduler)
    {
      if (!queue->n_ready)
      {
        break;
      }
      /** Mejor proceso de la cola ready */
      Process *best_process = queue_best_process(queue);
      /** Busco un core vacio */
      int core_empty = cpu_core_empty(cpu);
      if (core_empty != -1)
      {
        /** Asigno al core vacio */
        cpu_add_process(cpu, core_empty, best_process, time);
        /** Muevo el proceso a la cola running */
        queue_move_process_pointer(queue, READY, best_process, RUNNING);
      }
      else
      {
        /** Peor proceso de la cpu */
        Process *worst_process = cpu_worst_process(cpu);
        /** Comparamos los dos procesos */
        Process *best_priority = process_compare(best_process, worst_process);

        if (best_process == best_priority)
        {
          int n_core = worst_process->cpu;
          /** Se interrumpe el peor y se asigna el mejor */
          cpu_interrumpt_process(cpu, n_core);
          /** Se agrega el mejor proceso a la cpu */
          cpu_add_process(cpu, n_core, best_process, time);
          /** Se mueve el peor a la cola ready */
          queue_move_process_pointer(queue, RUNNING, worst_process, READY);
          worst_process->state = READY;
          /** Se mueve el mejor a la cola running */
          queue_move_process_pointer(queue, READY, best_process, RUNNING);
          best_process->state = RUNNING;
        }
        else
        {
          run_scheduler = false;
        }
      }
    }

    printf("Ready: \n");

    queue_print(queue->ready, queue->n_ready);
    printf("\n\n");

    printf("Running: \n");

    queue_print(queue->running, queue->n_running);
    printf("\n\n");

    printf("Waiting: \n");

    queue_print(queue->waiting, queue->n_waiting);
    printf("\n\n");

    printf("Finished: \n");

    queue_print(queue->finished, queue->n_finished);
    printf("\n\n");

    /** Paso 5: Paso al siguiente ciclo */
    time++;
  }
  printf("Ready: \n");

  queue_print(queue->ready, queue->n_ready);
  printf("\n\n");

  printf("Running: \n");

  queue_print(queue->running, queue->n_running);
  printf("\n\n");

  printf("Waiting: \n");

  queue_print(queue->waiting, queue->n_waiting);
  printf("\n\n");

  printf("Finished: \n");

  queue_print(queue->finished, queue->n_finished);
  printf("\n\n");
  printf("Terminó\n");

  /** Paso 6: Imprimo el output y libero la memoria */
  write_output(outputFile, queue);
  free_cpu(cpu);
  free_queue(queue);
}

int main(int argc, char *argv[])
{
  int n_cpu;

  if (argc < 3) // Los argumentos no fueron pasado
  {
    printf("Número de parámetros incorrectos:\n\n");
    printf("./edf <file> <output> [<n>]\n");
    printf("- <file>:   Archivo de entrada\n");
    printf("- <output>: Ruta de sálida para un csv\n");
    printf("- <n>:      Cantidad de CPUs. Default=1\n");
    return 0;
  }

  if (argv[3]) // Manejar número de CPUs
  {
    n_cpu = atoi(argv[3]);
  }
  else
  {
    n_cpu = 1;
  }

  run(argv[1], argv[2], n_cpu);

  return 0;
}

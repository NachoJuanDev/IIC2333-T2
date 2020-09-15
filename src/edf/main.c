
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
    int numero_rafagas = atoi(token);

    /* obtenemos las rafagas */
    int rafagas[255];
    for (int i = 0; i < numero_rafagas; i++)
    {
      token = strtok(NULL, " ");
      rafagas[i] = atoi(token);
    }

    /* creamos el proceso y lo poblamos con todos los datos de antes */
    Process *proceso = process_init(pid_proceso, nombre_proceso, deadline, tiempo_inicio, READY);

    process_add_behavior(proceso, numero_rafagas, rafagas);

    lista_queue->process[proceso_actual] = proceso;
    proceso_actual += 1;
  }

  fclose(file);

  return lista_queue;
}

void write_output(char *outputFile)
{
  FILE *fp;
  int myInt = 5;
  fp = fopen(outputFile, "w");
  char *saludo = "HOLA,";
  fprintf(fp, "%s This is being written in the file. This is an int variable: %d", saludo, myInt);
  fclose(fp); // Hasta ésta línea hay código sacado de stackoverflow
}

void run(char *fileName, char *outputFile, int n_cpu)
{

  Queue *queue = populate_system(fileName);
  CPU *cpu = cpu_init(n_cpu);

  bool run = true;
  int time = 0;
  while (run)
  {
    /** Paso 0: Actualizo los estados de los procesos */
    queue_process_checking(queue);

    /** Paso 1: 
     * Revisar procesos que terminaron en la cpu
     * Liberar procesos finalizados de la cpu
     * Revisar condición de termino
     * */
    queue_running_to_finished(queue);
    core_free_finished_process(cpu);
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
        cpu_add_process(cpu, core_empty, best_process);
        /** Muevo el proceso a la cola running */
      }
      else
      {
        /** Peor proceso de la cpu */
        Process *worst_process = cpu_worst_process(cpu);
        /** Comparamos los dos procesos */
        Process *best_priority = process_compare(best_process, worst_process);

        if (best_process == best_priority)
        {
          /** Se interrumpe el peor y se asigna el mejor */
          /** Se mueve el peor a la cola ready */
          /** Se mueve el mejor a la cola running */
        }
        else
        {
          run_scheduler = false;
        }
      }
    }

    /** Paso 5: Paso al siguiente ciclo */
    time++;
  }

  /** Paso 6: Imprimo el output y libero la memoria */
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

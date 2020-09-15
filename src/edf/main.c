
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
  printf("file: %s output: %s cpu: %i \n", fileName, outputFile, n_cpu);

  Queue *lista_queue = populate_system(fileName);

  /* COMIENZO Testing */

  CPU *cpu_test;
  cpu_test = cpu_init(n_cpu);

  printf("/* Testeo comparaciónes */\n");
  Process *proceso_prueba;
  proceso_prueba = process_compare(lista_queue->process[2], lista_queue->process[1]);
  printf("%s \n", proceso_prueba->name);

  printf("/* Testeo mejor proceso de queue */\n");
  queue_move_process(lista_queue, INACTIVE, 0, READY);
  queue_move_process(lista_queue, INACTIVE, 1, READY);
  queue_move_process(lista_queue, INACTIVE, 2, READY);
  proceso_prueba = queue_best_process(lista_queue);
  printf("%s \n", proceso_prueba->name);

  printf("/* Testeo peor proceso de cpu */\n");
  //cpu_add_process(cpu_test, 0, lista_queue->process[0]);
  cpu_add_process(cpu_test, 1, lista_queue->process[1]);
  cpu_add_process(cpu_test, 2, lista_queue->process[2]);
  int resultado;
  resultado = cpu_worst_process(cpu_test);
  printf("%s \n", cpu_test->cores[resultado]->proceso_actual->name);

  /* FIN Testing */

  /* A continuacón se realiza el guardado y eliminado de todo*/

  free_cpu(cpu_test);
  free_queue(lista_queue);
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

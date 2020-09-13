

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

#include "process/process.h"
#include "queue/queue.h"

void run(char *fileName, char *outputFile, int n_cpu)
{
  printf("file: %s output: %s cpu: %i \n", fileName, outputFile, n_cpu);

  FILE *file = fopen(fileName, "r"); /* should check the result */
  char line[2048];
  int numero_procesos = atoi(fgets(line, sizeof(line), file)); /* El número de procesos */
  printf("el numero de procesos es %d \n", numero_procesos);
  /* Creamos lista de procesos */
  Queue *lista_queue = queue_init();
  int proceso_actual = 0;

  while (fgets(line, sizeof(line), file))
  {
    char *token; // de acá hasta donde se indique salió de tutorialspoint.
    /* obtenemos el nombre */
    token = strtok(line, " ");
    char nombre_proceso[255] = "hola";
    strcpy(nombre_proceso, token);
    printf("%s \n", nombre_proceso);

    /* obtenemos el PID */
    token = strtok(NULL, " ");
    pid_t pid_proceso = atoi(token);
    printf("%d \n", pid_proceso);

    /* obtenemos el tiempo de inicio */
    token = strtok(NULL, " ");
    int tiempo_inicio = atoi(token);
    printf("%d \n", tiempo_inicio);

    /* obtenemos el deadline */
    token = strtok(NULL, " ");
    int deadline = atoi(token);
    printf("%d \n", deadline);

    /* obtenemos el numero de rafagas */
    token = strtok(NULL, " ");
    int numero_rafagas = atoi(token);
    printf("%d \n", numero_rafagas);

    /* obtenemos las rafagas */
    int rafagas[255];
    for (int i = 0; i < numero_rafagas; i++)
    {
      token = strtok(NULL, " ");
      rafagas[i] = atoi(token);
      printf("%d \n", rafagas[i]);
    }

    /* creamos el proceso y lo poblamos con todos los datos de antes */
    Process *proceso = process_init(pid_proceso, nombre_proceso, deadline, tiempo_inicio, 1);
    printf("%s \n", proceso->name);
    printf("%d \n", proceso->pid);
    printf("%d \n", proceso->deadline);
    printf("%d \n", proceso->start_time);
    printf("%d \n", proceso->state);
    proceso->n_behavior = numero_rafagas;
    for (int i = 0; i < numero_rafagas; i++)
    {
      proceso->behaviors[i] = rafagas[i];
      printf("%d \n", proceso->behaviors[i]);
    }

    lista_queue->process[proceso_actual] = proceso;
    proceso_actual += 1;
  }

  queue_destroy(lista_queue);

  fclose(file); // Hasta ésta línea hay código sacado de stackoverflow

  FILE *fp; // Desde ésta línea hasta la marcada hay código sacado de stackoverflow
  int myInt = 5;
  fp = fopen(outputFile, "w");
  char *saludo = "HOLA,";
  fprintf(fp, "%s This is being written in the file. This is an int variable: %d", saludo, myInt);
  fclose(fp); // Hasta ésta línea hay código sacado de stackoverflow
}

int main(int argc, char *argv[]) // Desde ésta línea hasta la marcada hay código sacado de stackoverflow
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

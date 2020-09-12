#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "process/process.h"
#include "queue/queue.h"

void run(char *file, char *output, int n_cpu)
{
  printf("file: %s output: %s cpu: %i \n", file, output, n_cpu);

  Process *process = process_init(50, file, 1, RUNNING);
  Queue *queue = queue_init();

  queue->process[0] = process;

  printf("pid: %d, name: %s, priority: %i, state: %i\n",
         queue->process[0]->pid, queue->process[0]->name, queue->process[0]->priority, queue->process[0]->state);

  queue_destroy(queue);
}

int main(int argc, char *argv[]) // Desde ésta línea hasta la marcada hay código sacado de stackoverflow
{

  char const *const fileName = argv[1]; /* should check that argc > 1 */
  FILE *file = fopen(fileName, "r");    /* should check the result */
  char line[2048];

  while (fgets(line, sizeof(line), file))
  {

    char *token; // de acá hasta donde se indique salió de tutorialspoint.

    /* get the first token */
    token = strtok(line, " ");
    while (token != NULL)
    {
      printf(" %s\n", token);

      token = strtok(NULL, " "); // hasta acá salió de tutorialspoint.
    }
  }

  fclose(file); // Hasta ésta línea hay código sacado de stackoverflow

  FILE *fp; // Desde ésta línea hasta la marcada hay código sacado de stackoverflow
  int myInt = 5;
  fp = fopen("Output.csv", "w");
  char *saludo = "HOLA,";
  fprintf(fp, "%s This is being written in the file. This is an int variable: %d", saludo, myInt);
  fclose(fp); // Hasta ésta línea hay código sacado de stackoverflow

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

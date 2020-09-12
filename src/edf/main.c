#include <stdio.h>
#include <stdlib.h>

#include "process/process.h"
#include "queue/queue.h"

void run(char *file, char *output, int n_cpu)
{
  printf("file: %s output: %s cpu: %i \n", file, output, n_cpu);
}

int main(int argc, char **argv)
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

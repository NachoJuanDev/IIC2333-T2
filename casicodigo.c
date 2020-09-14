

void run(char *file, char *output, int n_cpu)
{
    // inicio ordenar procesos
    // final ordenar procesos

    // lo siguiente sólos e repite una vez para setear el primer while
    int tiempo = 0;
    int procesos = 0;
    for (int i = 0; i < numero_procesos; i++)
    {
        if (process->state == finished)
        {
            procesos += 1;
        }
    }

    // while del proceso completo
    int tiempo_actual = 0;
    while (procesos > 0)
    {
        // enconramos todos los procesos que se deben considerar
        process procesos_a_considerar[numero_procesos];
        int n = 0;
        for (int i = 0; i < numero_procesos; i++)
        {
            if (process->state == ready || process->inicio == tiempo_actual)
            {
                procesos_a_considerar[n] = process;
                n += 1;
            }
        }

        // comienza el while para iniciar el reemplazo
        bool reemplazo = true;
        int n = 0;
        while (reemplazo)
        {
            //consideramos el caso en que hay procesos y no hay
            if (procesos_a_considerar son todos null)
            {
                process proceso_postulante = procesos_a_considerar[n];
                // hay que considerar el caso en que hay cpu_cores desocupadas
                if (hay_cpu_vacía)
                {
                    add_process_to_cpu(process);
                    reemplazo = true;
                    n += 1
                }
                else
                {
                    // acá va todo lo que sucede si no hay cpus vacías y hay procesos

                    // primero encontramos la cpu que tiene el proceso con peor prioridad
                    int numero_cpu;
                    int prioridad;
                    for (int i = 0; i < numero_cpus; i++)
                    {
                        if (cpu->process->prioridad < prioridad)
                        {
                            prioridad = cpus->prioridad;
                            numero_cpu = i;
                        }
                        if (cpu->process->prioridad == prioridad)
                        {
                            //chequeo de ids
                        }
                    }

                    //encontramos el proceso con mejor prioridad en procesos_a_considerar
                    int numero_process;
                    int prioridad;
                    for (int i = 0; i < numero_procesos_a_considerar; i++)
                    {
                        if (process->prioridad < prioridad)
                        {
                            prioridad = cpus->prioridad;
                            numero_cpu = i;
                        }
                        if (process->prioridad == prioridad)
                        {
                            //chequeo de ids
                        }
                    }

                    //revisamos si la prioridad es peor o no.
                    if (procesos_a_considerar[numero_process] >= cpus[numero_cpu])
                    {

                        // Chequeo si las prioridades son exactamente iguales, si lo son, reviso pid
                        Falta;

                        //hago el reeemplazo correspondiente
                        cpus[numero_cpu] = procesos_a_considerar[numero_process] reemplazo = true;
                        n += 1;
                    }
                    else
                    {
                        reemplazo = false;
                    }
                }
            }
            else
            {
                //esto sucede si no habían procesos
                reemplazo = false;
            }
        }
    }
}
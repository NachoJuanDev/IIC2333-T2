# Algoritmo

## Paso 0

Actualizar estado de la simulación (Actualizar los distintos estados y 
variables de los procesos para simular el avance del tiempo de la simulación.)

* To Do:
    - QUEUE debe tener más colas (Para FINISHED, WAITING, READY, RUNNING) (completo)
    - CPU debe tener una referencia a los procesos (completo)
    - PROCESS Tiene que tener una variable que indique en que rafaga está y cuanto a avanzado de esa rafaga (completo)
    - PROCESS Tiene que tener un atributo que indique cuantas veces fue agregado a la cpu (completo)
    - **PROCESS debe tener un método para actualizar sus estados y variables según en que state esté** (falta sólo el interior de la función, es llegar y ver que atributos deberían cambiar, por eso lo dejé vacío)

    - QUEUE debe tener un método para recorrer la cola principal de todos los procesos y ejecutar el método anterior (completo)

* Implementación:
    - WAITING:
        - Sumar una unidad de tiempo a la rafaga actual
        - Sumar una unidad de tiempo waiting time
        - Revisar si termino la rafaga de I/O
            - Si termino, actualizar la rafaga actual y cambiar su estado a READY
    - READY:
        - Sumar una unidad de tiempo waiting time
    - RUNNING:
        - Sumar una unidad de tiempo a la rafaga actual
        - Revisar si termino la rafaga de CPU
            - Si termino, actualizar la rafaga actual y cambiar su estado a FINISHED/WAITING

## Paso 1

Revisar que procesos terminaron y quitarlos de la cpu y moverlos de la cola RUNNING a la cola FINISHED; 

* To Do: 
    - CPU debe tener un método para revisar si los procesos terminaron (hice un método que revisa si terminó el core N y lo vacía,

        para usarlo con todos los core hay que sólo iterar sobre todos los cores). (Completo)

    - QUEUE debe tener un método para mover procesos de una cola a otra 

    y dejar las colas ordenadas (sin espacios vacios) (Completo)

* Implementación:
    - queue_running_to_finished
    - Verificiar condición de termino: n_finished == n_process
        - Si termina, voy al paso 6
    - core_free_finished_process

## Paso 2

Buscar procesos que lleguen en el tiempo actual y moverlos a la cola READY; 

* To Do:
    - QUEUE debe tener un método para revisar su cola principal para buscar procesos que lleguen en un tiempo TIME y pasarlos a la cola READY (completo)

* Implementación:
    - queue_inactive_to_ready

## Paso 3

Buscar los procesos que pueden pasar a estado READY de la cola WAITING; 

* To Do:
    - QUEUE debe tener un método para pasar procesos de la cola WAITING a la cola READY (completo)

* Implementación:
    - REVISAR la lista de WAITING y pasarlo a READY (queue_waiting_to_ready)

## Paso 4

Elegiría los mejores procesos de la cola READY según cantidad de núcleos (como máximo); 

* To Do:
    - QUEUE debe tener un método que retorna el mejor proceso de la cola READY (DEBERIA FUNCIONAR)
    - CPU debe tener un método que retorna el peor proceso de sus cores (DEBERIA FUNCIONAR)
    - PROCESS debe tener un método que nos de la puntuación de un proceso o compare procesos entregando el mejor (DEBERIA FUNCIONAR)
    - **CPU debe tener un método para interrumpir un proceso**

* Implementación:
    - Obtenemos el mejor proceso de la cola READY, y vemos si lo podemos asiganr a un core
        - Se pudo asignar, se repite el paso 4 
        - Si no pudo asignar, lo comparamos con el peor proceso de la CPU
            - Si el mejor proceso tiene más prioridad que el peor de la cpu, se cambia (Interrupción, el proceso de la cpu se va a READY) y se repite el paso 4

            - Si no, se termina y se pasa al siguiente tick

## Paso 5

Paso al siguiente ciclo, vuelvo al paso 0; 

* To Do:

* Implementación:
    - Sumar uno al tiempo

## Paso 6

Imprimo el output.csv y libero la memoria

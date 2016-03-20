#include "solver.h"
#include <stdio.h>

/* Calcula la prioridad de una celda */
int priority(nDoku* doku, Cell* cell)
{
    int n = doku -> n;

    /* Mientras menos opciones tiene, mas prioridad tiene */
    int priority = (n * n - cell -> count) * peer_count;

    /* En caso de empate, el que tenga más vecinos asignados gana */
    priority += cell -> assigned_peers;

    return priority;
}

/* Chequea si es una jugada valida asignar el valor a esta celda */
bool local_is_safe(Cell* cell, int val)
{
    for(int i = 0; i < peer_count; i++)
    {
        /* Si uno de los compañeros ya tiene ese valor */
        if(cell -> peers[i] -> value == val)
        {
            /* Entonces shao */
            return false;
        }
    }
    return true;
}


/* Calcula todas las opciones de la celda. Retorna false si no tiene */
bool compute_options(nDoku* doku, Cell* cell)
{
    /* Reseteamos el contador */
    cell -> count = 0;
    int n = doku -> n;
    /* Recorremos los valores, de arriba hacia abajo */
    /* Ya que usaremos el arreglo como un stack */
    for(int val = n*n; val > 0; val--)
    {
        /* Revisa si la jugada es valida usando el arreglo de peers */
        if(local_is_safe(cell, val))
        {
            cell -> options[cell -> count++] = val;
        }
    }
    /* Retorna true si tenemos alguna opcion. False si no */
    return cell -> count > 0;
}

/* Asigna el siguiente valor en la lista de opciones de la celda */
/* Retorna false si el puzzle queda en un estado inresolvible */
bool assign_next(nDoku* doku, Cell* cell, Stack* stack, Heap* heap)
{
    /* Asignamos el valor */
    cell -> value = cell -> options[--cell -> count];
    stack_push(stack, cell);

    /* Si estamos haciendo seguimiento, comunicamos la jugada */
    if(step)
    {
        printf("%d %d %d\n", cell -> x, cell -> y, cell -> value);
    }

    /* Revisamos que el puzzle no sea inresolvible */
    for(int i = 0; i < peer_count; i++)
    {
        Cell* peer = cell -> peers[i];
        if(peer -> value == UNASSIGNED)
        {
            /* Si no tiene opciones entonces el puzzle es inresolvible */
            if(!compute_options(doku, peer)) return false;
        }
    }

    /* Ahora que sabemos que la decision es definitiva */
    for(int i = 0; i < peer_count; i++)
    {
        Cell* peer = cell -> peers[i];

        /* Le decimos a los compañeros que ahora tienen uno mas asignado */
        peer -> assigned_peers++;

        if(peer -> value == UNASSIGNED)
        {
            /* Calculamos su prioridad */
            int score = priority(doku,peer);

            /* Actualizamos su posicion en el heap */
            heap_update_key(heap, *(peer -> heapindex), score);
        }

    }


    return true;
}

Cell* choice_undo(nDoku* doku, Stack* stack, Heap* heap)
{
    Cell* cell = stack_pop(stack);
    cell -> value = UNASSIGNED;
    /* Si estamos haciendo seguimiento, comunicamos la jugada */
    if(step)
    {
        printf("%d %d %d\n", cell -> x, cell -> y, cell -> value);
    }


    for(int i = 0; i < peer_count; i++)
    {
        Cell* peer = cell -> peers[i];

        /* Le decimos a los compañeros que ahora tienen uno menos asignado */
        peer -> assigned_peers--;

        /* Recalculamos sus opciones */
        if(peer -> value == UNASSIGNED)
        {
            /* Ahora que tenemos los peers, precomputamos las opciones */
            compute_options(doku, peer);

            /* Calculamos su prioridad */
            int score = priority(doku,peer);

            /* Actualizamos su posicion en el heap */
            heap_update_key(heap, *(peer -> heapindex), score);
        }
    }
    undo_count++;
    return cell;
}

/* Inicializa el sistema de vecinos */
Heap* solver_init(nDoku* doku)
{
    int n = doku -> n;

    Heap* heap = heap_init(n*n*n*n);

    /* Cada celda tendra acceso directo a sus vecinos */
    /* Es decir, con las que comparte fila, cuadrante o columna */
    for(int i = 0; i < n*n; i++)
    {
        for(int j = 0; j < n*n; j++)
        {
            Cell* cell = doku -> grid[i][j];
            cell -> peers = malloc(sizeof(Cell*) * peer_count);

            int peer_count = 0;

            /* Agregamos los de la columna */
            for(int j = 0; j < n*n;j++)
            {
                /* No debe agregarse a si mismo */
                if(j == cell -> y) continue;

                Cell* peer = doku -> grid[cell -> x][j];

                cell -> peers[peer_count++] = peer;
            }
            /* Agregamos a los de la fila */
            for(int i = 0; i < n*n;i++)
            {
                /* No debe agregarse a si mismo */
                if(i == cell -> x) continue;

                Cell* peer = doku -> grid[i][cell -> y];

                cell -> peers[peer_count++] = peer;
            }
            /* Agregamos los del cuadrante */
            int boxi = cell -> x / n;
            int boxj = cell -> y / n;
            for (int i = 0; i < n; i++)
            {
                /* Los de la misma columa ya fueron agregados */
                if(boxi*n + i == cell -> x) continue;

                for (int j = 0; j < n; j++)
                {
                    /* Los de la misma fila ya fueron agregados */
                    if(boxj*n + j == cell -> y) continue;

                    Cell* peer = doku -> grid[boxi*n + i][boxj*n + j];

                    cell -> peers[peer_count++] = peer;
                }
            }

            if(cell -> value == UNASSIGNED)
            {
                /* Ahora que tenemos los peers, precomputamos las opciones */
                compute_options(doku, cell);

                /* Calculamos su prioridad */
                int score = priority(doku,cell);

                /* Y lo insertamos al heap segun su prioridad */
                /* Asegurandonos de almacenar el indice donde quedó */
                cell -> heapindex = heap_insert(heap, cell, score);
            }
        }
    }

    return heap;
}

/* Efectúa backtracking sobre el puzzle para encontrar la solucion */
bool solve_n_doku(nDoku* doku, Stack* stack)
{
    /* El heap va a tener las celdas ordenadas por prioridad */
    Heap* heap = solver_init(doku);

    do
    {
        /* Obtenemos la siguiente celda a asignar */
        Cell* next = heap_extract(heap);

        /* Si no quedan celdas por asignar, ya terminamos */
        if(!next)
        {
            heap_destroy(heap);
            return true;
        }
        else
        {
            /* Marcamos que ya no está en el heap */
            next -> heapindex = NULL;
        }

        /* Si la celda no tiene opciones o la opcion tomada no es valida */
        /* Aqui las opciones ya vienen calculadas */
        if(next -> count == 0 || !assign_next(doku, next, stack, heap))
        // if(!compute_options(doku, next) || !assign_next(doku, next, stack, heap))
        {
            while(true)
            {
                /* Recuperamos la celda que tomo la ultima desicion */
                while(true)
                {
                    /* Si no nos queda a donde volver */
                    if(stack_is_empty(stack))
                    {
                        heap_destroy(heap);
                        /* El puzzle no se puede resolver */
                        return false;
                    }
                    /* Volvemos al nodo anterior */
                    next = choice_undo(doku, stack, heap);

                    /* Si aun le quedan opciones, podemos probarlas */
                    if(next -> count > 0)
                    {
                        break;
                    }
                    /* Si a esa celda no le quedan opciones */
                    /* volvemos a retroceder */
                    else
                    {
                        /* Insertamos el nodo al final de la cola */
                        next -> heapindex = heap_insert(heap, next, 0);
                    }
                }

                /* Si es una asignacion valida, podemos pasar al siguiente */
                if(assign_next(doku, next, stack, heap))
                {
                    break;
                }
            }
        }

    /* Cuando el stack esté vacio, significa que ya probamos todo */
    } while(!stack_is_empty(stack));

    /* Si ya probamos todo, entonces no era resolvible */
    heap_destroy(heap);
    return false;
}

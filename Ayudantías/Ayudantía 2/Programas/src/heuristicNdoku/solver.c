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

/* Entrega la primera celda sin asignar que encuentre */
Cell* find_unnasigned_location(nDoku* doku)
{
    int n = doku -> n;

    Cell* chosen = NULL;

    int chosen_score = 0;

    for(int i = 0; i < n*n; i++)
    {
        for(int j = 0; j < n*n; j++)
        {
            /* Si la celda esta vacia es candidato */
            if(doku -> grid[i][j] -> value == UNASSIGNED)
            {
                if(chosen == NULL)
                {
                    chosen = doku -> grid[i][j];
                    chosen_score = priority(doku, chosen);
                }
                else
                {
                    int candidate_score = priority(doku, doku -> grid[i][j]);
                    if(candidate_score > chosen_score)
                    {
                        chosen = doku -> grid[i][j];
                        chosen_score = candidate_score;
                    }
                }
            }
        }
    }
    /* Si no quedan celdas vacias */
    return chosen;
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
        if(is_safe(doku, cell -> x, cell -> y, val))
        {
            cell -> options[cell -> count++] = val;
        }
    }
    /* Retorna true si tenemos alguna opcion. False si no */
    return cell -> count > 0;
}

/* Asigna el siguiente valor en la lista de opciones de la celda */
/* Retorna false si el puzzle queda en un estado inresolvible */
bool assign_next(nDoku* doku, Cell* cell, Stack* stack)
{
    /* Asignamos el valor */
    cell -> value = cell -> options[--cell -> count];
    stack_push(stack, cell);

    /* Si estamos haciendo seguimiento, comunicamos la jugada */
    if(step)
        printf("%d %d %d\n", cell -> x, cell -> y, cell -> value);

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
        /* Le decimos a los compañeros que ahora tienen uno mas asignado */
        cell -> peers[i] -> assigned_peers++;
    }


    return true;
}

Cell* choice_undo(Stack* stack)
{
    Cell* cell = stack_pop(stack);
    cell -> value = UNASSIGNED;
    /* Si estamos haciendo seguimiento, comunicamos la jugada */
    if(step)
        printf("%d %d %d\n", cell -> x, cell -> y, cell -> value);


    for(int i = 0; i < peer_count; i++)
    {
        /* Le decimos a los compañeros que ahora tienen uno mas asignado */
        cell -> peers[i] -> assigned_peers--;
    }
    undo_count++;
    return cell;
}

/* Inicializa el sistema de vecinos */
void solver_init(nDoku* doku)
{
    int n = doku -> n;

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
        }
    }
}

/* Efectúa backtracking sobre el puzzle para encontrar la solucion */
bool solve_n_doku(nDoku* doku, Stack* stack)
{
    solver_init(doku);

    do
    {
        /* Obtenemos la siguiente celda a asignar */
        Cell* next = find_unnasigned_location(doku);

        /* Si no quedan celdas por asignar, ya terminamos */
        if(!next) return true;

        /* Si la celda no tiene opciones o la opcion tomada no es valida */
        if(!compute_options(doku,next) ||
           !assign_next(doku, next, stack))
        {
            do
            {
                /* Recuperamos la celda que tomo la ultima desicion */
                do
                {
                    /* Si no nos queda a donde volver */
                    if(stack_is_empty(stack))
                    {
                        /* El puzzle no se puede resolver */
                        return false;
                    }
                    /* Volvemos al nodo anterior */
                    next = choice_undo(stack);

                /* Si a esa celda no le quedan opciones */
                /* volvemos a retroceder */
                } while(next -> count == 0);

            /* Retrocedemos hasta poder tomar una opcion valida */
            } while(!assign_next(doku, next, stack));
        }

    /* Cuando el stack esté vacio, significa que ya probamos todo */
    } while(!stack_is_empty(stack));

    /* Si ya probamos todo, entonces no era resolvible */
    return false;
}

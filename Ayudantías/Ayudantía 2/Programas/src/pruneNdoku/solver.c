#include "solver.h"
#include <stdio.h>

/* Entrega la primera celda sin asignar que encuentre */
Cell* find_unnasigned_location(nDoku* doku)
{
    int n = doku -> n;
    for(int i = 0; i < n*n; i++)
    {
        for(int j = 0; j < n*n; j++)
        {
            /* Si la celda esta vacia */
            if(doku -> grid[i][j] -> value == UNASSIGNED)
            {
                /* Toca esta entonces! */
                return doku -> grid[i][j];
            }
        }
    }
    /* Si no quedan celdas vacias */
    return NULL;
}


/* Calcula todas las opciones de la celda. Retorna false si no tiene */
bool compute_options(nDoku* doku, Cell* cell)
{
    /* Reseteamos el contador */
    cell -> count = 0;
    int n = doku -> n;

    /* Generamos el dominio para esa celda */
    int domain[n*n];
    for(int i = 0; i < n*n; i++) domain[i] = i + 1;
    shuffle(domain, n*n, cell);

    /* Recorremos los valores */
    for(int i = 0; i < n*n; i++)
    {
        if(is_safe(doku, cell -> x, cell -> y, domain[i]))
        {
            cell -> options[cell -> count++] = domain[i];
        }
    }
    /* Retorna true si tenemos alguna opcion. False si no */
    return cell -> count > 0;
}

/* Asigna el siguiente valor en la lista de opciones de la celda */
/* Retorna false si el puzzle queda en un estado inresolvible */
bool assign_next(nDoku* doku, Cell* cell, Stack* stack)
{
    int n = doku -> n;

    /* Asignamos el valor */
    cell -> value = cell -> options[--cell -> count];
    stack_push(stack, cell);

    /* Si estamos haciendo seguimiento, comunicamos la jugada */
    if(step)
        printf("%d %d %d\n", cell -> x, cell -> y, cell -> value);


    /* Revisamos la columna */
    for(int j = 0; j < n*n;j++)
    {
        Cell* pal = doku -> grid[cell -> x][j];
        /* Solo si no tiene valor aun */
        if(pal -> value == UNASSIGNED)
        {
            /* Si no tiene opciones, entonces el puzzle es inresolvible! */
            if(!compute_options(doku, pal)) return false;
        }
    }
    /* Revisamos la fila */
    for(int i = 0; i < n*n;i++)
    {
        Cell* pal = doku -> grid[i][cell -> y];
        /* Solo si no tiene valor aun */
        if(pal -> value == UNASSIGNED)
        {
            /* Si no tiene opciones, entonces el puzzle es inresolvible! */
            if(!compute_options(doku, pal)) return false;
        }
    }
    /* Revisamos el cuadrante */
    int boxi = cell -> x / n;
    int boxj = cell -> y / n;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            Cell* pal = doku -> grid[boxi*n + i][boxj*n + j];
            /* Solo si no tiene valor aun */
            if(pal -> value == UNASSIGNED)
            {
                /* Si no tiene opciones entonces el puzzle es inresolvible */
                if(!compute_options(doku, pal)) return false;
            }
        }
    }
    return true;
}

Cell* choice_undo(Stack* stack)
{
    Cell* ret = stack_pop(stack);
    ret -> value = UNASSIGNED;
    /* Si estamos haciendo seguimiento, comunicamos la jugada */
    if(step)
        printf("%d %d %d\n", ret -> x, ret -> y, ret -> value);

    undo_count++;
    return ret;
}

/* Efectúa backtracking sobre el puzzle para encontrar la solucion */
bool solve_n_doku(nDoku* doku, Stack* stack)
{
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

#include <stdio.h>
#include "solver.h"


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

/* Intenta darle valor a una celda y resolver el puzzle resultante */
/* Retorna true si el puzzle tiene solucion, false si no */
bool solve_n_doku(nDoku* doku)
{
    Cell* next = find_unnasigned_location(doku);
    /* Si no nos quedan celdas por asignar, ya terminamos el puzzle */
    if(!next)
    {
        return true;
    }

    /* Computamos los posibles valores que puede tener la celda */
    compute_options(doku, next);

    // /* Revisamos los posibles valores en orden */
    // for (int val = 1; val <= doku -> n * doku -> n; val++)
    /* Mientras le queden opciones */
    while(next -> count > 0)
    {
        /* Tomamos la siguiente */
        int val = next -> options[--next -> count];
        // /* Si es legal asignar ese valor */
        // if (is_safe(doku, next -> x, next -> y, val))
        // {
        /* Lo asignamos */
        next -> value = val;

        /* Si estamos haciendo seguimiento, comunicamos la jugada */
        if(step)
        {
            printf("%d %d %d\n", next -> x, next -> y, val);
        }

        /* Si el estado resultante puede ser resuelto, estamos listos! */
        if(solve_n_doku(doku))
        {
            return true;
        }

        /* Si no, debemos deshacer nuestra jugada */
        next -> value = UNASSIGNED;

        /* Registramos la devolución */
        undo_count++;

        /* Si estamos haciendo seguimiento, comunicamos la jugada */
        if(step)
        {
            printf("%d %d %d\n", next -> x, next -> y, UNASSIGNED);
        }

        // }
    }
    /* Ya probamos todos los valores y ninguno servia. */
    /* Entonces este estado no puede llegar a la solución */
    return false;
}

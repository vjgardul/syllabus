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

/* Intenta darle valor a una celda y resolver el puzzle resultante */
/* Retorna true si el puzzle tiene solucion, false si no */
bool solve_n_doku(nDoku* doku)
{
    Cell* next = find_unnasigned_location(doku);
    /* Si no nos quedan celdas por asignar, ya terminamos el puzzle */
    if(!next)
       return true;

    /* Revisamos los posibles valores en orden */
    for (int val = 1; val <= doku -> n * doku -> n; val++)
    {
        /* Si es legal asignar ese valor */
        if (is_safe(doku, next -> x, next -> y, val))
        {
            /* Lo asignamos */
            next -> value = val;

            /* Si estamos haciendo seguimiento, comunicamos la jugada */
            if(step)
                printf("%d %d %d\n", next -> x, next -> y, val);

            /* Si el estado resultante puede ser resuelto, estamos listos! */
            if(solve_n_doku(doku))
                return true;

            /* Si no, debemos deshacer nuestra jugada */
            next -> value = UNASSIGNED;

            /* Registramos la devolución */
            undo_count++;

            /* Si estamos haciendo seguimiento, comunicamos la jugada */
            if(step)
                printf("%d %d %d\n", next -> x, next -> y, UNASSIGNED);

        }
    }
    /* Ya probamos todos los valores y ninguno servia. */
    /* Entonces este estado no puede llegar a la solución */
    return false;
}

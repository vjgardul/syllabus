#include "doku.h"
#include <stdio.h>

/* Inicializa una celda de n-doku para un puzzle de orden n */
Cell* cell_init(int n, int x, int y)
{
    Cell* cell = malloc(sizeof(Cell));

    /* Inicialmente la celda está vacia */
    cell -> value = UNASSIGNED;

    /* Para un sudoku orden n, la grilla es de n² x n² */
    /* Por lo que una celda puede tener n² opciones */
    cell -> options = malloc(sizeof(int) * n * n);

    /* Inicialmente no hemos calculado las opciones de esta celda */
    cell -> count = 0;

    /* Inicialmente no estamos en un heap */
    cell -> heapindex = NULL;

    cell -> x = x;
    cell -> y = y;

    cell -> assigned_neighbours = 0;

    cell -> neighbours = NULL;

    return cell;
}

/* Libera los recursos asociados a esa celda */
void  cell_destroy (Cell* cell)
{
    free(cell -> neighbours);
    free(cell -> options);
    free(cell);
}

/** Inicializa una grilla vacia de n-doku para un puzzle de orden n */
nDoku*   n_doku_init_empty    (int n)
{
    nDoku* doku = malloc(sizeof(nDoku));

    /* Inicializa grilla de n² x n² */
    doku -> grid = malloc(sizeof(Cell**) * n * n);
    for(int i = 0; i < n*n; i++)
    {
        doku -> grid[i]  = malloc(sizeof(Cell*) * n * n);
        for(int j = 0; j < n*n; j++)
        {
            doku -> grid[i][j] = cell_init(n,i,j);
        }
    }

    /* Almacenamos el n */
    doku -> n = n;

    return doku;
}

/** Libera los recursos asociados a este n-doku */
void     n_doku_destroy       (nDoku* doku)
{
    int n = doku -> n;
    for(int i = 0; i < n*n; i++)
    {
        for(int j = 0; j < n*n; j++)
        {
            cell_destroy(doku -> grid[i][j]);
        }
        free(doku -> grid[i]);
    }
    free(doku -> grid);
    free(doku);
}

/* Imprime los valores de cada celda */
void n_doku_print(nDoku* doku)
{
    int n = doku -> n;
    for(int i = 0; i < n*n; i++)
    {
        for(int j = 0; j < n*n; j++)
        {
            /* Comunicamos el valor de la celda i,j */
            printf("%d %d %d\n", i, j, doku -> grid[i][j] -> value);
        }
    }
}

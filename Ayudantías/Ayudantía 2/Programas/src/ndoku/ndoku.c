/* Armado a partir del codigo de ejemplo en GeeksForGeeks:                 */
/*     http://www.geeksforgeeks.org/backtracking-set-7-suduku/             */

/* Importamos el módulo de Input / Output */
#include <stdio.h>
/* Importamos la librería estándar de C */
#include <stdlib.h>
/* Importamos el módulo de booleans */
#include <stdbool.h>
/* Importamos el módulo de strings */
#include <string.h>

/* Indica que una celda no tiene valor aún */
#define UNASSIGNED 0

/* El orden del n-doku. El tablero tendrá tamaño n² x n² */
int n;

/* Indica que se debe notificar cada paso del algoritmo */
bool step = false;

/* Recorre la grilla y retorna cuando encuentre la primera celda vacía */
bool FindUnassignedLocation(int** grid, int* row, int* col)
{
    /* Al retornar los punteros apuntaran a la celda correspondiente */
    for (*row = 0; *row < n*n; *row+=1)
        for (*col = 0; *col < n*n; *col+=1)
            if (grid[*row][*col] == UNASSIGNED)
                return true;
    /* Si no encuentra ninguna entonces retorna false */
    return false;
}

/* Indica si el número está o no presente en la fila especificada */
bool UsedInRow(int** grid, int row, int num)
{
    for (int col = 0; col < n*n; col++)
        if (grid[row][col] == num)
            return true;
    return false;
}

/* Indica si el número está o no presente en la columna especificada */
bool UsedInCol(int** grid, int col, int num)
{
    for (int row = 0; row < n*n; row++)
        if (grid[row][col] == num)
            return true;
    return false;
}

/* Indica si el número está o no presente en la caja nxn especificada */
bool UsedInBox(int** grid, int boxStartRow, int boxStartCol, int num)
{
    for (int row = 0; row < n; row++)
        for (int col = 0; col < n; col++)
            if (grid[row+boxStartRow][col+boxStartCol] == num)
                return true;
    return false;
}

/* Indica si es legal asignar el numero en la celda espeficada */
bool isSafe(int** grid, int row, int col, int num)
{
    return !UsedInRow(grid, row, num) &&
           !UsedInCol(grid, col, num) &&
           !UsedInBox(grid, row - row%n , col - col%n, num);
}

/* Intenta darle valor a una celda y resolver el puzzle resultante */
/* Retorna true si el puzzle tiene solucion, false si no */
bool SolveNDoku(int** grid)
{
    /* Los indices de la siguente celda a asignar */
    int row, col;

    /* Si no nos quedan celdas por asignar, ya terminamos el puzzle */
    if (!FindUnassignedLocation(grid, &row, &col))
       return true;

    /* Revisamos los posibles valores en orden */
    for (int val = 1; val <= n*n; val++)
    {
        /* Si es legal asignar ese valor */
        if (isSafe(grid, row, col, val))
        {
            /* Lo asignamos */
            grid[row][col] = val;

            /* Si estamos haciendo seguimiento, comunicamos la jugada */
            if(step)
                printf("%d %d %d\n", row, col, val);

            /* Si el estado resultante puede ser resuelto, estamos listos! */
            if(SolveNDoku(grid))
                return true;

            /* Si no, debemos deshacer nuestra jugada */
            grid[row][col] = UNASSIGNED;

            /* Si estamos haciendo seguimiento, comunicamos la jugada */
            if(step)
                printf("%d %d %d\n", row, col, UNASSIGNED);

        }
    }
    /* Ya probamos todos los valores y ninguno servia. */
    /* Entonces este estado no puede llegar a la solución */
    return false;
}

/* Libera toda la memoria de la grilla */
void grid_destroy(int** grid)
{
    for(int i = 0; i < n*n; i++)
    {
        free(grid[i]);
    }
    free(grid);
}

/* Imprime los valores de cada celda */
void grid_print(int** grid)
{
    for(int i = 0; i < n*n; i++)
    {
        for(int j = 0; j < n*n; j++)
        {
            /* Comunicamos el valor de la celda i,j */
            printf("%d %d %d\n", i, j, grid[i][j]);
        }
    }
}

/* Indica como usar el programa */
bool howto(char* program)
{
    printf("Faltan parámetros. Debes ejecutar %s n f, donde\n", program);
    printf("n es el orden del n-doku. n = 3 es sudoku tradicional\n");
    printf("f indica el modo de seguimiento:\n");
    printf("\tf = step para seguir al algoritmo paso a paso\n");
    printf("\tf = fast para mostrar solo la solución\n");
    return false;
}

/* Revisa que el programa haya sido ejecutado correctamente */
bool param_check(int argc, char** argv)
{
    /* Cantidad incorrecta de parametros */
    if(argc != 3)
    {
        return howto(argv[0]);
    }

    /* Orden */
    n = atoi(argv[1]);
    if(n <= 0)
    {
        printf("n inválido. Debe ser entero mayor a 0\n");
        printf("Valor entregado: %d\n", n);
        return false;
    }

    /* Seguimiento */
    if(!strcmp(argv[2], "step"))
    {
        step = true;
    }
    else if(!strcmp(argv[2], "fast"))
    {
        step = false;
    }
    else
    {
        printf("f inválido. Debe ser step o fast\n");
        printf("Valor entregado: %s\n", argv[2]);
        return false;
    }
    return true;
}

/* Inicializa el tablero de n-doku y lo resuelve */
int main(int argc, char** argv)
{
    /* Revisa que el programa haya sido ejecutado correctamente */
    if(!param_check(argc, argv)) return 2;

    /* Comunicamos el n escogido */
    printf("%d\n", n);

    /* Inicializamos la grilla. Inicialmente todas las celdas estan vacías */
    int** grid = malloc(sizeof(int*) * n*n);
    for(int i = 0; i < n*n; i++)
    {
        grid[i] = malloc(sizeof(int) * n*n);
        for(int j = 0; j < n*n; j++)
        {
            grid[i][j] = UNASSIGNED;
            /* Comunicamos el valor de la celda i,j */
            printf("%d %d %d\n", i, j, grid[i][j]);
        }
    }

    /* Intentamos resolver el n-doku */
    if(SolveNDoku(grid))
    {
        fprintf(stderr, "%s\n", "Solución encontrada");
        /* Notificamos el valor de cada celda en el estado solucion */
        grid_print(grid);
        /* Comunicamos que terminó el programa. El Watcher entenderá */
        printf("-1\n");
        /* Liberamos la memoria */
        grid_destroy(grid);
        /* El programa terminó satisfactoriamente */
        return 0;
    }
    else
    {
        /* Indicamos que no se pudo resolver */
        fprintf(stderr,"%s","No solution exists");
        /* Comunicamos que terminó el programa. El Watcher entenderá */
        printf("-1\n");
        /* Liberamos la memoria */
        grid_destroy(grid);
        /* El programa terminó con errores */
        return 1;
    }
}

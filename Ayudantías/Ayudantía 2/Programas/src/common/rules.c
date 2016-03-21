#include <stdbool.h>

#include "rules.h"

/* Indica si el número está o no presente en la fila especificada */
bool used_in_row(nDoku* doku, int j, int num)
{
    int n = doku -> n;
    for (int i = 0; i < n*n; i++)
    {
        if (doku -> grid[i][j] -> value == num) return true;
    }
    return false;
}

/* Indica si el número está o no presente en la columna especificada */
bool used_in_col(nDoku* doku, int i, int num)
{
    int n = doku -> n;
    for (int j = 0; j < n*n; j++)
    {
        if (doku -> grid[i][j] -> value == num) return true;
    }
    return false;
}

/* Indica si el número está o no presente en la caja nxn especificada */
bool used_in_box(nDoku* doku, int boxi, int boxj, int num)
{
    int n = doku -> n;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (doku -> grid[boxi*n + i][boxj*n + j] -> value == num)
            {
                return true;
            }
        }
    }
    return false;
}

/* Indica si es legal asignar el numero en la celda espeficada */
bool is_safe(nDoku* doku, int i, int j, int num)
{
    int n = doku -> n;
    return !used_in_row(doku, j, num) &&
           !used_in_col(doku, i, num) &&
           !used_in_box(doku, i/n, j/n, num);
}

/* Para darle randomicidad */
void shuffle(int *array, size_t n, Cell* cell)
{
    srand(cell -> x + cell -> y*n*n + 12);
    // srand(cell -> x + cell -> y*n*n + 10);
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

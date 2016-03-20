#ifndef A2_DOKU_H
#define A2_DOKU_H

#include <stdlib.h>
#include <stdint.h>

/* Se considera que una celda no esta asignada si vale 0 */
#define UNASSIGNED 0

/* Celda de n-doku */
struct doku_cell
{
    /** Valor actual de la celda */
    uint8_t value;

    /** Arreglo con los posibles valores */
    uint8_t* options;

    /** Cuantas opciones tiene */
    uint8_t count;

    /** Indice del heap donde está esta celda */
    size_t* heapindex;

    /** Indice de la columna en la que está esta celda */
    uint8_t x;

    /** Indice de la fila en la que está esta celda */
    uint8_t y;

    /* Cantidad de vecinos asignados. Utilizado para la heurística */
    uint16_t assigned_neighbours;

    /* Arreglo con los vecinos. Utilizado para acelerar las heurísticas */
    struct doku_cell* neighbours;
};
typedef struct doku_cell Cell;

/* Grilla de n-doku */
struct doku_grid
{
    /** La grilla en si */
    Cell*** grid;

    /** El orden del n-doku. El tablero tendrá tamaño n² x n² */
    uint8_t n;
};
typedef struct doku_grid nDoku;

/** Inicializa una grilla vacia de n-doku para un puzzle de orden n */
nDoku*   n_doku_init_empty    (int n);
/** Libera los recursos asociados a este n-doku */
void     n_doku_destroy       (nDoku* doku);
/** Imprime los valores de cada celda */
void     n_doku_print         (nDoku* doku);


#endif /* end of include guard: A2_DOKU_H */

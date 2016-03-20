#ifndef A2_ITESOL
#define A2_ITESOL

#include <stdbool.h>

#include "../common/stack.h"

#include "../common/rules.h"

#include "../common/heap.h"

/* Cuenta la cantidad de UNDOs que se han hecho */
unsigned int undo_count;

/* Indica que se debe notificar cada paso del algoritmo */
bool step;

/* La cantidad de compañeros de cada celda */
/* Es decir, con las que comparte fila, columna o cuadrante */
int peer_count;

/* Intenta darle valor a una celda y resolver el puzzle resultante */
/* Retorna true si el puzzle tiene solucion, false si no */
bool solve_n_doku(nDoku* doku, Stack* stack);


#endif /* end of include guard: A2_ITESOL */

#ifndef A2_RECSOL
#define A2_RECSOL

#include <stdbool.h>

#include "../common/rules.h"

/* Cuenta la cantidad de UNDOs que se han hecho */
unsigned int undo_count;

/* Indica que se debe notificar cada paso del algoritmo */
bool step;

/* Intenta darle valor a una celda y resolver el puzzle resultante */
/* Retorna true si el puzzle tiene solucion, false si no */
bool solve_n_doku(nDoku* doku);

#endif /* end of include guard: A2_RECSOL */

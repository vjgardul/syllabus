#ifndef A2_RULES
#define A2_RULES

/*#########################################################################*/
/* Las reglas del juego son las mismas para todos.                         */
/*#########################################################################*/

#include <stdbool.h>

#include "doku.h"

/* Indica si es legal asignar el numero en la celda espeficada */
bool is_safe(nDoku* doku, int row, int col, int num);

#endif /* end of include guard: A2_RULES */

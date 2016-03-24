#ifndef T1_LIB_DRAWER_H
#define T1_LIB_DRAWER_H

#include <gtk/gtk.h>
#include "../common/city.h"

/* Ancho y alto en píxeles de las celdas octagonales */
#define CELL_SIZE 96.0

/** Indica que se debe mostrar la estructura interior del problema */
bool draw_inner;

/** Indica el delay entre cada actualizacion de la ventana*/
int refreshInterval;

/** Indica que hay que guardar el ultimo estado de la ventana como .pdf */
bool pdf;

/** Abre una ventana para vizualizar el plano de la ciudad */
void view(Layout* layout, int refreshInterval);

#endif /* End of include guard: T1_LIB_VIEWER_H */

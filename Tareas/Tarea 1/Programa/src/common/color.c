#include <stdlib.h>
#include <math.h>
#include "color.h"
#include <stdio.h>
int color_selected = 1;

/* Super aleatorio */
Color color_random()
{
    Color ret = (Color) (color_selected % 7 + 1);

    color_selected++;
    return ret;
}

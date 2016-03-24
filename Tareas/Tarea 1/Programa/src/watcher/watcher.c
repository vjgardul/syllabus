#include "drawer.h"
#include <string.h>

void param_check(int argc, char** argv)
{
    refreshInterval = 0;
    draw_inner = false;

    /* Deberia ser un flag y el delay */
    if(argc == 3)
    {
        draw_inner = !strcmp(argv[1],"-u");
        /* Si el primero era el flag */
        if(draw_inner)
        {
            refreshInterval = atoi(argv[2]);
        }
        else
        {
            refreshInterval = atoi(argv[1]);
            draw_inner = !strcmp(argv[2],"-u");
        }
    }
    /* Uno solo de ambos */
    else if(argc == 2)
    {
        draw_inner = !strcmp(argv[1],"-u");
        if(!draw_inner)
        {
            refreshInterval = atoi(argv[1]);
        }
    }
    if(!refreshInterval) refreshInterval = 100;
}

/* Abre una ventana con la que se puede visualizar el puzzle */
int main(int argc, char** argv)
{
    pdf = true;

    param_check(argc,argv);

    Layout* layout = city_layout_read(stdin);

    view(layout, refreshInterval);

    city_layout_destroy(layout);

    return 0;
}

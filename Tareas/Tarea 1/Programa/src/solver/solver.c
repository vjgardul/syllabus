#include "../common/city.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    /* Leemos la ciudad del input */
    Layout* layout = city_layout_read(stdin);

    /* Lo imprimimos para darle el estado inicial al watcher / judge */
    city_layout_print(layout);

    /* TODO RESOLVER PROBLEMA */

    /* TODO IMPRIMIR DECISIONES */

    /* Indicamos al watcher y al judge que ya terminamos */    
    printf("END\n");

    /* Liberamos memoria */
    city_layout_destroy(layout);

    return 0;
}

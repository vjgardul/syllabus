/*#########################################################################*/
/*                               City  I/O                                 */
/*                                                                         */
/* Módulo encargado del Input / Output del mapa de la ciudad               */
/* Por Vicente Errázuriz                                                   */
/*#########################################################################*/

#include <string.h>
#include <stdio.h>
#include "city.h"

/*#########################################################################*/
/*                                 Output                                  */
/*#########################################################################*/


/** Comunica que se efectuó la conexion entre 2 nodos */
void city_client_link_print(Client* client1, Client* client2)
{
    size_t  i0 = client1 -> zone -> index;
    uint8_t i1 = client1 -> index;
    uint8_t i2 = client2 -> index;
    printf("LINK %zu %hhu %hhu\n", i0, i1, i2);
}

/** Comunica que se deshizo la última conexión */
void city_client_link_undo_print(Client* client1, Client* client2)
{
    printf("UNDO\n");
}
/*#########################################################################*/
/* TODO : Cambiar todos los nombres a ciudad / zona / edificio             */
/* en lugar de grafo / celda / nodo                                         */
/*#########################################################################*/

/** Imprime en consola las aristas de un nodo */
void  link_print(Building* node1)
{
    Zone* cell1 = node1 -> zone;

    for(int i = 0; i < node1 -> link_count; i++)
    {
        Building* node2 = node1 -> linked[i];
        Zone* cell2 = node2 -> zone;

        printf("LINK %s %zu %hhu %s %zu %hhu\n",
                cell1 -> core ? "CORE" : "CELL",
                cell1 -> index, node1 -> index ,
                cell2 -> core ? "CORE" : "CELL",
                cell2 -> index, node2 -> index);
    }
}

/* Imprime en consola la información de un nodo */
void node_print(Building* node)
{
    //index: Deducible
    //link_count: Siempre 0 al principio
    //linked: lo crea y llena el lector
    //parent: Deducible
    //desired
    printf("NODE   %hhu\n", node -> capacity);
    //color
    printf("COLOR  %u\n", node -> color);
    //direction
    printf("DIRECT %hhu\n", node -> direction);
    //x, y los usa el visor solamente
}

/** Imprime en consola una celda y sus nodos */
void cell_print(Zone* cell)
{
    //core: va en el nombre
    printf("%s %u\n", cell -> core ? "CORE  " : "CELL  ", cell -> sides);
    //sides: va junto con el nombre
    printf("INDEX  %zu\n", cell -> index);
    //x,y
    printf("X      %d\n", cell -> x);
    printf("Y      %d\n", cell -> y);
    //building_count
    printf("NODES  %d\n", cell -> building_count);
    for(int i = 0; i < cell -> building_count; i++)
    {
        node_print(cell -> buildings[i]);
    }
}

/** Imprime en consola el grafo completo */
void  city_layout_print(Layout* graph)
{
    printf("GRAPH\n");
    printf("WIDTH  %zu\n", graph -> width);
    printf("HEIGHT %zu\n", graph -> height);
    printf("CELLS  %zu\n", graph -> zone_count);
    for(int i = 0; i < graph -> zone_count; i++)
    {
        cell_print(graph -> zones[i]);
    }
    printf("CORES  %zu\n", graph -> core_count);
    for(int i = 0; i < graph -> core_count; i++)
    {
        cell_print(graph -> cores[i]);
    }
    for(int i = 0; i < graph -> zone_count; i++)
    {
        for(int j = 0; j < graph -> zones[i] -> building_count; j++)
        {
            link_print(graph -> zones[i] -> buildings[j]);
        }
    }

    for(int i = 0; i < graph -> core_count; i++)
    {
        for(int j = 0; j < graph -> cores[i] -> building_count; j++)
        {
            link_print(graph -> cores[i] -> buildings[j]);
        }
    }

    printf("EOF\n");
}

/*#########################################################################*/
/*                                Destroyers                               */
/*#########################################################################*/


/* Libera los recursos asociados a este nodo */
void node_destroy(void* pointer)
{
    /* Interpretamos el puntero como un nodo */
    Building* node = pointer;
    /* Liberar el arreglo de conexiones. Los nodos no se destruyen aqui */
    free(node -> linked);
    free(node);
}


/* Libera los recursos asociados a esta celda */
void cell_destroy(void* pointer)
{
    /* Interpretamos el puntero como una celda. Si no es celda se caerá */
    Zone* cell = pointer;

    /* Libera cada uno de los nodos */
    for(int i = 0; i < cell -> building_count; i++)
    {
        node_destroy(cell -> buildings[i]);
    }
    /* El arreglo de nodos */
    free(cell -> buildings);
    /* Y a si misma */
    free(cell);
}


void city_layout_destroy(Layout* layout)
{
    for(int i = 0; i < layout -> zone_count; i++)
    {
        cell_destroy(layout -> zones[i]);
    }
    for(int i = 0; i < layout -> core_count; i++)
    {
        cell_destroy(layout -> cores[i]);
    }
    free(layout -> zones);
    free(layout -> cores);
    free(layout);
}

void city_layout_destroy_v(void* ptr)
{
    city_layout_destroy(ptr);
}


/*#########################################################################*/
/*                                  Input                                  */
/*#########################################################################*/

void* read_error(void* stream, void* structure, void (* destroy)(void *))
{
    fprintf(stderr,"%s\n", "Error leyendo grafo");
    destroy(structure);
    fclose(stream);
    return NULL;
}

bool scan_for(void* stream, char* format, char* name, void* bucket)
{
    char buffer[256];
    if(!fscanf(stream, "%s", buffer) || strcmp(buffer,name))
    {
        return false;
    }
    if(!fscanf(stream, format, bucket))
    {
        return false;
    }
    return true;
}



/** Lee los datos de un nodo y sus conexiones a partir del stream */
bool link_read(void* stream, Layout* graph)
{
    /* Lee comandos del tipo A X N B Y M */
    /* A = CELL / CORE */
    /* X = Indice de A en su lista */
    /* N = Indice del nodo en A */
    /* B = CELL / CORE */
    /* Y = Indice de B en su lista */
    /* M = Indice del nodo en B */

    char buffer[256];
    Zone** array1;
    size_t size1;

    if(!fscanf(stream, "%s", buffer)) return false;
    if(!strcmp(buffer, "CELL"))
    {
        array1 = graph -> zones;
        size1 = graph -> zone_count;
    }
    else if(!strcmp(buffer, "CORE"))
    {
        array1 = graph -> cores;
        size1 = graph -> core_count;
    }
    else
    {
        return false;
    }

    size_t index1;
    if(!fscanf(stream, "%zu", &index1)) return false;
    if(index1 >= size1) return false;
    uint8_t node_index1;
    if(!fscanf(stream, "%hhu", &node_index1)) return false;
    if(node_index1 >= array1[index1] -> building_count) return false;

    Zone** array2;
    size_t size2;

    if(!fscanf(stream, "%s", buffer)) return false;
    if(!strcmp(buffer, "CELL"))
    {
        array2 = graph -> zones;
        size2 = graph -> zone_count;
    }
    else if(!strcmp(buffer, "CORE"))
    {
        array2 = graph -> cores;
        size2 = graph -> core_count;
    }
    else
    {
        return false;
    }

    size_t index2;
    if(!fscanf(stream, "%zu", &index2)) return false;
    if(index2 >= size2) return false;
    uint8_t node_index2;
    if(!fscanf(stream, "%hhu", &node_index2)) return false;
    if(node_index2 >= array2[index2] -> building_count) return false;

    Building* node1 = array1[index1] -> buildings[node_index1];
    Building* node2 = array2[index2] -> buildings[node_index2];

    node1 -> linked[node1 -> link_count++] = node2;
    return true;
}

Building* node_read(void* stream)
{
    Building* node = malloc(sizeof(Building));
    node -> color = none;
    node -> linked = NULL;
    node -> link_count = 0;

    if(!scan_for(stream, "%hhu","NODE",&node -> capacity))
    {
        return read_error(stream, node, node_destroy);
    }
    node -> linked = malloc(sizeof(Building*) * node -> capacity);
    for(int i = 0; i < node -> capacity; i++)
    {
        node -> linked[i] = NULL;
    }

    if(!scan_for(stream, "%u", "COLOR", &node -> color))
    {
        return read_error(stream, node, node_destroy);
    }

    if(!scan_for(stream, "%hhu", "DIRECT", &node -> direction))
    {
        return read_error(stream, node, node_destroy);
    }

    node -> drawn = 0;

    return node;
}

Zone* cell_read(void* stream)
{
    /** La celda a retornar */
    Zone* cell = malloc(sizeof(Zone));
    cell -> building_count = 0;
    /** Buffer de lectura. El largo de las palabras no deberia superar 6 */
    char buffer[6];

    /* Scan for Cell */
    if(!fscanf(stream, "%s", buffer))
    {
        return read_error(stream, cell, cell_destroy);
    }

    /* Se espera a que la palabra sea CELL o CORE */
    if(!strcmp(buffer, "CELL")) cell -> core = false;
    else if(!strcmp(buffer, "CORE")) cell -> core = true;
    else return read_error(stream, cell, cell_destroy);

    /* Scan for sides */
    if(!fscanf(stream, "%hhu" , &cell -> sides))
    {
        return read_error(stream, cell, cell_destroy);
    }

    /* Scan for index */
    if(!scan_for(stream, "%zu", "INDEX", &cell -> index))
    {
        return read_error(stream, cell, cell_destroy);
    }

    /* Scan for X */
    if(!scan_for(stream, "%d", "X", &cell -> x))
    {
        return read_error(stream, cell, cell_destroy);
    }

    /* Scan for Y */
    if(!scan_for(stream, "%d", "Y", &cell -> y))
    {
        return read_error(stream, cell, cell_destroy);
    }

    /* Scan for building_count */
    if(!scan_for(stream, "%hhu", "NODES", &cell -> building_count))
    {
        return read_error(stream, cell, cell_destroy);
    }

    /* Scan for Nodes, n times */
    cell -> buildings = malloc(sizeof(Building*) * cell -> building_count);
    for(uint8_t i = 0; i < cell -> building_count; i++)
    {
        Building* node = node_read(stream);
        if(!node)
        {
            return read_error(stream, cell, cell_destroy);
        }
        node -> index = i;
        node -> zone = cell;

        cell -> buildings[i] = node;
    }

    return cell;
}

Layout* city_layout_read(void* stream)
{
    /** El grafo a retornar */
    Layout* graph = malloc(sizeof(Layout));
    graph -> zone_count = 0;
    graph -> core_count = 0;
    graph -> zones = NULL;
    graph -> cores = NULL;

    /** Buffer de lectura. El largo de las palabras no deberia superar 6 */
    char buffer[6];

    /* Scan for Graph */
    if(!fscanf(stream, "%s", buffer) || strcmp(buffer, "GRAPH"))
    {
        return read_error(stream, graph, city_layout_destroy_v);
    }

    /* Scan for Width */
    if(!scan_for(stream, "%zu", "WIDTH", &graph -> width))
    {
        return read_error(stream, graph, city_layout_destroy_v);
    }

    /* Scan for Height */
    if(!scan_for(stream, "%zu", "HEIGHT", &graph -> height))
    {
        return read_error(stream, graph, city_layout_destroy_v);
    }

    /* Initialize matrix */
    int x = 2 * graph -> width - 1;
    int y = 2 * graph -> height - 1;

    /** Variable auxiliar para guardar el largo de los arreglos */
    size_t n = 0;

    /* Scan for Cells */
    if(!scan_for(stream, "%zu", "CELLS", &n))
    {
        return read_error(stream, graph, city_layout_destroy_v);
    }

    graph -> zones = malloc(sizeof(Zone*) * n);
    /* Scan for zones, n times */
    while(graph -> zone_count < n)
    {
        Zone* cell = cell_read(stream);
        if(!cell || cell -> x >= x || cell -> y >= y)
        {
            return read_error(stream, graph, city_layout_destroy_v);
        }
        graph -> zones[graph -> zone_count++] = cell;
    }

    /* Scan for Cores */
    if(!scan_for(stream, "%zu", "CORES", &n))
    {
        return read_error(stream, graph, city_layout_destroy_v);
    }
    graph -> cores = malloc(sizeof(Zone*) * n);
    /* Scan for cores, n times */
    while(graph -> core_count < n)
    {
        Zone* core = cell_read(stream);
        if(!core || core -> x >= x || core -> y >= y)
        {
            return read_error(stream, graph, city_layout_destroy_v);
        }
        graph -> cores[graph -> core_count++] = core;
    }

    /* Scan for links till End of File */
    while(!fscanf(stream, "%s", buffer) || strcmp(buffer, "EOF"))
    {
        if(strcmp(buffer,"LINK") || !link_read(stream, graph))
        {
            return read_error(stream, graph, city_layout_destroy_v);
        }
    }

    return graph;
}

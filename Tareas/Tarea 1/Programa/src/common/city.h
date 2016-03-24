/*#########################################################################*/
/*                              City Layout                                */
/*                                                                         */
/* Módulo encargado de la representación de los elementos de la ciudad     */
/* Por Vicente Errázuriz                                                   */
/*#########################################################################*/

#ifndef T1_LIB_LAYOUT_H
#define T1_LIB_LAYOUT_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "color.h"

/*######################   Structs : Declaración   ########################*/

/* Representa el plano de la ciudad */
struct city_layout;
/* Representa una zona de la ciudad */
struct city_zone;
/* Representa un edificio de la ciudad */
struct city_building;

/*#############################   Aliases   ###############################*/

/** Representa el plano de la ciudad */
typedef struct city_layout Layout;
/** Representa una zona de la ciudad, puede ser una zona civil o un núcleo */
typedef struct city_zone Zone;
/** Representa una zona núcleo de la ciudad */
typedef struct city_zone Core;
/** Representa un edificio de la ciudad, puede ser cliente o central */
typedef struct city_building Building;
/** Representa un cliente de la ciudad */
typedef struct city_building Client;
/** Representa una central de abastecimiento de la ciudad */
typedef struct city_building Central;

/*#######################   Structs : Definición   ########################*/

/* Representa un edificio de la ciudad */
struct city_building
{
    /** La zona a la cual pertenece este este edificio */
    Zone*        zone;
    /** Indice del edificio dentro de su zona */
    uint8_t      index;
    /** El color actual del edificio */
    Color        color;
    /** Cantidad de edificios a los que está conectado actualmente */
    uint8_t      link_count;
    /** Arreglo de nodos vecinos */
    Building**   linked;

    /*#####################################################################*/
    /* Variables de uso interno. No las toques o puede fallar todo         */
    /*#####################################################################*/

    /** La cantidad de vecinos que necesita para estar completo */
    uint8_t      capacity;
    /** Dirección en la que está este edificio */
    uint8_t      direction;
    /** Posición en X de este edificio */
    double       x;
    /** Posición en Y de este edificio */
    double       y;
    /** Indica que este edificio y sus aristas ya fue dibujado */
    uint8_t      drawn;
    /** Indica que este edificio esta conectado a un core */
    bool         cored;
};

/* Representa una zona de la ciudad */
struct city_zone
{
    /** Cuantos edificios tiene esta zona */
    uint8_t      building_count;
    /** Arreglo con los edificios de esta celda. (largo = building_count)*/
    Building**   buildings;
    /** Indice de esta zona en su respectivo arreglo en el plano */
    size_t       index;

    /*#####################################################################*/
    /* Variables de uso interno. No las toques o puede fallar todo         */
    /*#####################################################################*/

    /** Indica que esta zona es un núcleo y no necesita ser procesada */
    bool         core;
    /** Cuantos lados tiene esta celda. Puede ser 8 o 4. */
    uint8_t      sides;
    /** Coordenada X de esta celda */
    int          x;
    /** Coordenada Y de esta celda */
    int          y;
};

/* Representa el plano de la ciudad */
struct city_layout
{
    /** Arreglo de zonas civiles de la ciudad. (largo = cell_count) */
    Zone**       zones;
    /** Cuantas zonas civiles hay en total */
    size_t       zone_count;
    /** Arreglo de las zonas núcleo. (largo = core_count) */
    Core**       cores;
    /** Cuantas zonas núcleo posee este plano */
    size_t       core_count;

    /*#####################################################################*/
    /* Variables de uso interno. No las toques o puede fallar todo         */
    /*#####################################################################*/

    /** Ancho del plano */
    size_t       width;
    /** Alto del plano */
    size_t       height;

};

/*#########################################################################*/
/*                             Manejo global                               */
/*#########################################################################*/

/** Inicializa una ciudad a partir de información en un stream */
Layout*   city_layout_read             (void* stream);
/** Libera todos los recursos asociados a la ciudad */
void      city_layout_destroy          (Layout* layout);
/** Imprime toda la información de la ciudad en consola */
void      city_layout_print            (Layout* layout);

/*#########################################################################*/
/*                     Información y toma de desiciones                    */
/*                                                                         */
/* Con estas funciones puedes obtener la información necesaria para saber  */
/* qué decisiones tomar. También están las funciones que efectúan la toma  */
/* de decisiones en si.                                                    */
/*#########################################################################*/

/** Entrega la cantidad de puertos del núcleo */
uint8_t   city_core_get_capacity       (Core* core);
/** Entrega el color de un núcleo */
Color     city_core_get_color          (Core* core);

/** Indica que un cliente ya cumple con todo lo que tiene que cumplir */
bool      city_client_is_ready         (Client* client);
/** Indica que un cliente ya tiene ambas parejas asignadas */
bool      city_client_is_taken         (Client* client);
/** Indica que un ciente no tiene color / no forma parte de una ruta */
bool      city_client_is_blank         (Client* client);

/** Asocia dos clientes. Asume que ambos pueden. No propaga colores */
void      city_client_link             (Client* client1, Client* client2);
/** Deshace la conexion entre dos clientes. Asume que es la última */
void      city_client_link_undo        (Client* client1, Client* client2);

/*#########################################################################*/
/*                          Output del programa                            */
/*                                                                         */
/* Estas funciones son para comunicarse con el watcher y el judge          */
/*                                                                         */
/* El watcher construye el nuevo estado a partir de lo que le comuniques   */
/* en particular te mostrara los edificios que DEBERÍAN ESTAR PINTADOS     */
/* luego de una conexión, siendo que quizás tu no los pintaste             */
/*                                                                         */
/* El judge funciona en modo corrección, por lo que solo acepta decisiones */
/* correctas. Si le imprimes un UNDO considerará que fallaste              */
/*#########################################################################*/

/** Comunica que se efectuó la conexion entre 2 nodos */
void      city_client_link_print       (Client* client1, Client* client2);
/** Comunica que se deshizo la última conexión */
void      city_client_link_undo_print  (Client* client1, Client* client2);


#endif /* End of include guard: T1_LIB_LAYOUT_H */

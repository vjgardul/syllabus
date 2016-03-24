#include "city.h"

/** Entrega la cantidad de puertos del núcleo */
uint8_t city_core_get_capacity(Core* core)
{
    return core -> buildings[0] -> capacity;
}
/** Entrega el color de un núcleo */
Color city_core_get_color(Core* core)
{
    return core -> buildings[0] -> color;
}

/** Asocia dos clientes. Asume que ambos pueden */
void city_client_link(Client* client1, Client* client2)
{
    client1 -> linked[client1 -> link_count++] = client2;
    client2 -> linked[client2 -> link_count++] = client1;
}

/** Deshace la conexion entre dos clientes. Asume que es la última */
void city_client_link_undo(Client* client1, Client* client2)
{
    client1 -> linked[--client1 -> link_count] = NULL;
    client2 -> linked[--client2 -> link_count] = NULL;
}

/** Indica que un cliente ya tiene ambas parejas asignadas */
bool city_client_is_taken(Client* client)
{
    return (client -> link_count == client -> capacity);
}
/** Indica que un ciente no tiene color / no forma parte de una ruta */
bool city_client_is_blank(Client* client)
{
    return (client -> color == none);
}

/** Indica que un cliente ya cumple con todo lo que tiene que cumplir */
bool city_client_is_ready(Client* client)
{

    return !city_client_is_blank(client) && city_client_is_taken(client);
}

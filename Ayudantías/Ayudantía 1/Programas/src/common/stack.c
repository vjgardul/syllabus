#include <stdlib.h>
#include "stack.h"

/** Crea un nuevo stack vacio */
Stack* stack_init()
{
    /* Solicitamos memoria para el stack */
    Stack* stack = malloc(sizeof(Stack));
    /* Inicialmente está vacio */
    stack -> top = NULL;
    /* La cantidad de elementos es 0 */
    stack -> count = 0;
    /* Entregamos el stack listo */
    return stack;
}

/** Libera los recursos asociados al stack. Llamar solo si está vacio */
void stack_destroy(Stack* stack)
{
    /* Libera la memoria del stack */
    free(stack);
}

/** Introduce un puntero en el stack */
void stack_push(Stack* stack, void* pointer)
{
    stack -> count++;
    /* Creamos un nodo para guardar el puntero */
    struct stack_node* node = malloc(sizeof(struct stack_node));
    /* Guardamos el puntero en el nodo */
    node -> content = pointer;
    /* El nodo siguiente va a ser el que lo era antes */
    node -> next = stack -> top;
    /* Hacemos que el stack apunte a este nodo ahora */
    stack -> top = node;
}

/** Obtiene el ultimo puntero que se le entregó */
void* stack_pop(Stack* stack)
{
    stack -> count--;
    /* Tomamos el nodo que esta en la punta del stack */
    struct stack_node* node = stack -> top;
    /* Obtenemos el puntero que estaba guardado en el */
    void* pointer = node -> content;
    /* Hacemos que el stack apunte al siguiente nodo */
    stack -> top = node -> next;
    /* Eliminamos el nodo que ahora quedó vacio */
    free(node);
    /* Entregamos el puntero sano y salvo */
    return pointer;
}

/** Indica si el stack está vacio o no */
bool stack_is_empty(Stack* stack)
{
    /* Cuando el stack esté vacio no habrá ningun nodo */
    return (stack -> top == NULL);
}

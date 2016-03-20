#include <stdlib.h>
#include "stack.h"

/** Crea un nuevo stack vacio */
Stack* stack_init(size_t size)
{
    /* Solicitamos memoria para el stack */
    Stack* stack = malloc(sizeof(Stack));
    /* Inicializamos el arreglo */
    stack -> array = malloc(sizeof(void*) * size);
    /* La cantidad de elementos es 0 */
    stack -> count = 0;
    /* Asignamos el tamaño del stack */
    stack -> size = size;
    /* Entregamos el stack listo */
    return stack;
}

/** Libera los recursos asociados al stack. Llamar solo si está vacio */
void stack_destroy(Stack* stack)
{
    /* Libera la memoria del stack */
    free(stack -> array);
    free(stack);
}

/** Introduce un puntero en el stack */
void stack_push(Stack* stack, void* pointer)
{
    /* Asignamos el puntero en la siguiente celda disponible */
    stack -> array[stack -> count++] = pointer;
}

/** Obtiene el ultimo puntero que se le entregó */
void* stack_pop(Stack* stack)
{
    return stack -> array[--stack -> count];
}

/** Indica si el stack está vacio o no */
bool stack_is_empty(Stack* stack)
{
    /* Cuando el stack esté vacio no habrá ningun nodo */
    return (stack -> count == 0);
}

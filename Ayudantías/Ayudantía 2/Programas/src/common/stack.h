#ifndef T1_LIB_STACK_H
#define T1_LIB_STACK_H

#include <stdbool.h>
#include <stdlib.h>

/* Stack genérico como arreglo de punteros */
/* Almacena objetos en orden LIFO */
struct stack
{
    /** El arreglo que será usado como stack */
    void** array;
    /** Lleva la cuenta de cuantos elementos tiene el stack */
    int count;
    /** Indica el tamaño del stack */
    size_t size;
};
/** Stack en base a nodos que almacena objetos en orden LIFO */
typedef struct stack Stack;

/** Crea un nuevo stack vacio */
Stack* stack_init     (size_t size);
/** Libera los recursos asociados al stack. Llamar solo si está vacio */
void   stack_destroy  (Stack* stack);
/** Introduce un puntero en el stack */
void   stack_push     (Stack* stack, void* pointer);
/** Obtiene el ultimo puntero que se le entregó */
void*  stack_pop      (Stack* stack);
/** Indica si el stack está vacio o no */
bool   stack_is_empty (Stack* stack);

#endif /* End of include guard: T1_LIB_STACK_H */

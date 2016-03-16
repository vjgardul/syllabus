#ifndef T1_LIB_STACK_H
#define T1_LIB_STACK_H

#include <stdbool.h>

/* Stack simple y generico. Está lejos de ser la mejor implementación */
/* Si quieres eficiencia construye tu propio stack ad-hoc para problema */

/* Nodo para el stack */
struct stack_node
{
    /** Puntero almacenado en este nodo */
    void* content;
    /** Siguiente nodo del stack */
    struct stack_node* next;
};

/* Stack en base a nodos que almacena objetos en orden LIFO */
struct stack
{
    /** El nodo que está más arriba del stack */
    struct stack_node* top;
    /** Lleva la cuenta de cuantos elementos tiene el stack */
    int count;
};
/** Stack en base a nodos que almacena objetos en orden LIFO */
typedef struct stack Stack;

/** Crea un nuevo stack vacio */
Stack* stack_init     ();
/** Libera los recursos asociados al stack. Llamar solo si está vacio */
void   stack_destroy  (Stack* stack);
/** Introduce un puntero en el stack */
void   stack_push     (Stack* stack, void* pointer);
/** Obtiene el ultimo puntero que se le entregó */
void*  stack_pop      (Stack* stack);
/** Indica si el stack está vacio o no */
bool   stack_is_empty (Stack* stack);

#endif /* End of include guard: T1_LIB_STACK_H */

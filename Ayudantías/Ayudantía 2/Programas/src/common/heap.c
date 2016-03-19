#include "heap.h"
#include <stdio.h>
#include <limits.h>

/*#########################################################################*/
/*                          Operaciones Internas                           */
/*#########################################################################*/

/* Macro para obtener el indice del padre del nodo con indice i */
#define heap_parent_of(i)      (((i)-(1))/(2))
/* Macro para obtener el indice del hijo izquierdo del nodo con indice i */
#define heap_left_child_of(i)  (((2)*(i))+(1))
/* Macro para obtener el indice del hijo derecho del nodo con indice i */
#define heap_right_child_of(i) (((2)*(i))+(2))

/** Intercambia dos elementos en el arreglo */
void heap_swap(Heap* heap, size_t i, size_t j)
{
    HeapNode aux = heap -> array[i];
    heap -> array[i] = heap -> array[j];
    heap -> array[j] = aux;

    *heap -> array[i].index = i;
    *heap -> array[j].index = j;
}

/** Hace bajar el nodo en index hasta la posicion que le corresponde */
void heap_sift_down(Heap* heap, size_t index)
{
    /** El índice actual del nodo */
    size_t node_i = index;

    /* Repetimos la operación hasta que se indique lo contrario */
    while(true)
    {
        /* Los indices de ambos hijos del nodo */
        size_t left_i = heap_left_child_of(node_i);
        size_t right_i = heap_right_child_of(node_i);

        /* Si no tiene hijo izquierdo, no tiene a donde bajar */
        if(left_i >= heap -> count) break;

        /* El nodo solo tiene hijo izquierdo */
        if(right_i == heap -> count)
        {
            /* Si el hijo es mayor que el padre */
            if(heap -> array[left_i].key > heap -> array[node_i].key)
            {
                /* Se intercambian */
                heap_swap(heap, node_i, left_i);
            }
            /* Como no tenía hijo derecho, no tiene a donde más bajar */
            break;
        }
        /* El nodo tiene dos hijos */
        else
        {
            /* Las key de los hijos */
            int left_key = heap -> array[left_i].key;
            int right_key = heap -> array[right_i].key;

            /* El indice del hijo con la key mas alta */
            int max_i = left_key > right_key ? left_i : right_i;

            /* Si el hijo más grande es mayor que el padre */
            if(heap -> array[max_i].key > heap -> array[node_i].key)
            {
                /* Se intercambian */
                heap_swap(heap, node_i, max_i);

                /* Indicamos que el indice cambio y seguimos bajandolo */
                node_i = max_i;
            }
            /* Si no, no hay nada más que hacer */
            else break;
        }
    }
}

/** Hace subir el nodo en index hasta la posicion que le corresponde */
void heap_sift_up(Heap* heap, size_t index)
{
    /* El índice actual del nodo */
    size_t node_i = index;

    /* Repetimos la operacion hasta que se indique lo contrario */
    while(true)
    {
        /* Si el indice es 0, no tiene a donde más subir */
        if(!node_i) break;

        /* Obtenemos el indice del padre de este nodo */
        size_t parent_i = heap_parent_of(node_i);

        /* Si el hijo es mayor que el padre */
        if(heap -> array[node_i].key > heap -> array[parent_i].key)
        {
            /* Se intercambian */
            heap_swap(heap, node_i, parent_i);

            /* Indicamos que cambió el indice y seguimos subiendolo */
            node_i = parent_i;
        }
        /* Si son iguales no hay nada mas que hacer */
        else break;
    }
}

/*#########################################################################*/
/*                          Operaciones Públicas                           */
/*#########################################################################*/

/** Inicializa un heap del tamaño dado */
Heap* heap_init(size_t size)
{
    /* Solicitamos memoria para el heap */
    Heap* heap = malloc(sizeof(Heap));
    /* Inicializar el arreglo de nodos */
    heap -> array = malloc(sizeof(HeapNode) * size);
    /* Se asigna el indice correspondiente a cada nodo */
    for(size_t i = 0; i < size; i++)
    {
        heap -> array[i].index = malloc(sizeof(size_t));
        *heap -> array[i].index = i;
    }
    /* Inicialmente no tiene ningun elemento */
    heap -> count = 0;
    /* Asignamos el largo */
    heap -> size = size;
    return heap;
}

/** Libera todos los recursos asociados a este heap */
void heap_destroy(Heap* heap)
{
    for(int i = 0; i < heap -> size; i++)
    {
        free(heap -> array[i].index);
    }
    free(heap -> array);
    free(heap);
}

/** Inserta al heap un puntero con key dada. Retorna el nodo donde quedó */
size_t* heap_insert(Heap* heap, void* pointer, int key)
{
    /* Almacenamos la informacion en el nodo */
    heap -> array[heap -> count].content = pointer;
    heap -> array[heap -> count].key = key;
    /* Obtenemos el puntero de su indice */
    size_t* index = heap -> array[heap -> count].index;
    /* Marcamos como que hay un elemento más */
    heap -> count++;
    /* Lo hacemos subir a donde le corresponde */
    heap_sift_up(heap, *index);
    /* Retornamos el indice del nodo */
    return index;
}

/** Extrae el elemento de mayor key del heap. Retorna NULL si esta vacio */
void* heap_extract(Heap* heap)
{
    /* Si no tiene elementos, retorna NULL */
    if(!heap -> count) return NULL;
    /* Obtenemos el puntero a retornar */
    void* max = heap -> array[0].content;
    /* Actualizamos el contador */
    heap -> count--;
    /* Ponemos el último elemento en la cabeza del heap */
    heap_swap(heap, 0, heap -> count);
    /* Lo hacemos bajar a donde le correspnde */
    heap_sift_down(heap, 0);
    /* Retornamos el puntero con mayor key */
    return max;
}

/** Actualiza la key y posicion del nodo en index */
void heap_update_key(Heap* heap, size_t index, int key)
{
    /* Si la key es la misma, no hay nada que hacer */
    if(heap -> array[index].key == key) return;

    /* Si la nueva es mayor a la antigua */
    if(key > heap -> array[index].key)
    {
        heap -> array[index].key = key;
        heap_sift_up(heap, index);
    }
    /* Si la nueva es menor */
    else
    {
        heap -> array[index].key = key;
        heap_sift_down(heap, index);
    }
}

/** Vacía el heap */
void heap_clear(Heap* heap)
{
    /* "Vacía" pero para efectos prácticos es lo mismo */
    heap -> count = 0;
}

/* Indica si el heap esta vacio */
bool heap_is_empty(Heap* heap)
{
    return !heap -> count;
}

/** Elimina del heap el elemento situado en index */
void heap_remove_at(Heap* heap, size_t index)
{
    /* Hacemos subir al nodo a la cabeza del heap */
    heap_update_key(heap, index, INT_MAX);
    /* Ponemos el último elemento en la cabeza del heap */
    heap_swap(heap, 0, --heap -> count);
    /* Lo hacemos bajar a donde le correspnde */
    heap_sift_down(heap, 0);
}

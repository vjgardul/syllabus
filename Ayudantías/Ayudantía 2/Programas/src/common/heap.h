#ifndef T1_LIB_MAXBINHEAP_H
#define T1_LIB_MAXBINHEAP_H

/*#########################################################################*/
/*                            Max-heap Binario                             */
/*                                                                         */
/* Módulo encargado del Input / Output del grafo                           */
/* Por Vicente Errázuriz                                                   */
/*#########################################################################*/

#include <stdlib.h>
#include <stdbool.h>


/** El nodo del heap, contiene al elemento e indica su posicion en el heap */
struct heap_node
{
    /** Puntero a un elemento cualquiera */
    void* content;

    /** Valor segun el cual se ordenan los elementos del heap */
    int key;

    /** Puntero que indica el indice de este nodo en el heap */
    size_t* index;
};
/** El nodo del heap. contiene al elemento, su key y su índice */
typedef struct heap_node HeapNode;



/** Representa un max-heap binario */
struct max_bin_heap
{
    /* Arreglo de punteros a nodos de heap que corresponde al heap */
    HeapNode* array;

    /* Cuantos elementos tiene el heap actualmente */
    size_t count;

    /* Tamaño del arreglo del heap */
    size_t size;
};
/** Representa un max-heap binario */
typedef struct max_bin_heap Heap;



/** Inicializa un heap del tamaño dado */
Heap*   heap_init       (size_t size);
/** Libera todos los recursos asociados al heap */
void    heap_destroy    (Heap* heap);
/** Inserta al heap un puntero con key dada. Retorna el puntero a su indice */
size_t* heap_insert     (Heap* heap, void* pointer, int key);
/** Extrae el elemento de mayor key del heap. Retorna NULL si está vacío */
void*   heap_extract    (Heap* heap);
/** Actualiza la key y posición del nodo situado en index */
void    heap_update_key (Heap* heap, size_t index, int key);
/** Vacía el heap */
void    heap_clear      (Heap* heap);
/** Indica si el heap esta vacio */
bool    heap_is_empty   (Heap* heap);
/** Elimina del heap el elemento situado en index */
void    heap_remove_at  (Heap* heap, size_t index);

#endif /* End of include guard: T1_LIB_MAXBINHEAP_H */

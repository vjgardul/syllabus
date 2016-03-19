#include "heap_test.h"
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdio.h>

/* Macro para obtener el indice del padre del nodo con indice i */
#define heap_parent_of(i)      (((i)-(1))/(2))

/* Tamaño del heap */
size_t size;

/* El max - heap */
Heap* heap;

/* Arreglo de ints para probar el heap */
int* pointers;

/* Arreglo de indices que indica en que parte del heap se esta */
size_t** indices;

void print_heap(Heap* heap)
{
    printf("HEAP\n");
    for(int i = 0; i < heap -> count; i++)
    {
        HeapNode node = heap -> array[i];
        printf("POINTER %p KEY %d\n", node.content, node.key);
    }
}

/* Indica si el heap es un heap */
bool heap_is_heap(Heap* heap)
{
    for(int i = heap -> count - 1; i > 0; i--)
    {
        int Key = heap -> array[i].key;
        int PKey = heap -> array[heap_parent_of(i)].key;
        if(Key > PKey) return false;
    }
    return true;
}

/* Indica que los indices están funcionando correctamente */
bool indices_work(Heap* heap)
{
    for(int i = 0; i < size; i++)
    {
        if(!indices[i]) continue;

        void* ptr = heap -> array[*indices[i]].content;
        void* real = &pointers[i];
        if(ptr != real) return false;
    }
    return true;
}

/* Indica que el heap contiene al puntero */
bool heap_contains(Heap* heap, void* ptr)
{
    for(int i = 0; i < heap -> count; i++)
    {
        void* content = heap -> array[i].content;
        if(ptr == content) return true;
    }
    return false;
}

bool somos_los_que_estamos(Heap* heap)
{
    for(int i = 0; i < size; i++)
    {
        if(indices[i])
        {
            if(!heap_contains(heap, &pointers[i])) return false;
        }
        else
        {
            if(heap_contains(heap, &pointers[i])) return false;
        }
    }
    return true;
}

/* Prueba que al insertar un nuevo elemento, se cumpla la propiedad de heap */
void test_insert(Heap* heap)
{
    for(int i = 0; i < size; i++)
    {
        pointers[i] = i;
        int key = rand() % size;
        printf("INSERT %d KEY %d\n", i, key);
        indices[i] = heap_insert(heap, &pointers[i], key);
        assert(somos_los_que_estamos(heap));
        assert(heap_is_heap(heap));
        assert(indices_work(heap));
    }
}

/* Prueba que al extraer un elemento este sea el mas grande */
void test_extract(Heap* heap)
{
    int prevKey = INT_MAX;
    while(!heap_is_empty(heap))
    {
        int key = heap -> array[0].key;
        void* ptr = heap -> array[0].content;
        size_t* index = heap -> array[0].index;
        void* ext = heap_extract(heap);
        assert(ptr == ext);
        assert(key <= prevKey);
        for(int i = 0; i < size; i++)
        {
            if(indices[i] == index)
            {
                indices[i] = NULL;
                break;
            }
        }
        assert(somos_los_que_estamos(heap));
        assert(heap_is_heap(heap));
        assert(indices_work(heap));
        prevKey = key;
    }
}

/* Verifica que los elementos se eliminen correctamente */
void test_remove_at(Heap* heap)
{
    int index;
    for(int i = 0; i < size / 2; i++)
    {
        do
        {
            index = rand() % size;
        } while(!indices[index]);
        printf("REMOVE %d\n", index);
        heap_remove_at(heap, *indices[index]);
        indices[index] = NULL;
        assert(somos_los_que_estamos(heap));
        assert(heap_is_heap(heap));
        assert(indices_work(heap));
    }
}

/* Verifica que las key se actualicen correctamente */
void test_update_key(Heap* heap)
{
    int index;
    for(int i = 0; i < size / 2; i++)
    {
        do
        {
            index = rand() % size;
        } while(!indices[index]);
        int key = rand() % size;
        printf("UPDATE %zu KEY %d\n", *indices[index], key);
        heap_update_key(heap, *indices[index], key);
        assert(somos_los_que_estamos(heap));
        assert(heap_is_heap(heap));
        assert(indices_work(heap));
        print_heap(heap);

    }
}

/* Rellena los espacios vacios del heap */
void reinsert_and_test(Heap* heap)
{
    for(int i = 0; i < size; i++)
    {
        if(!indices[i])
        {
            int key = rand() % size;
            printf("INSERT %d KEY %d\n", i, key);
            indices[i] = heap_insert(heap, &pointers[i], key);
            assert(somos_los_que_estamos(heap));
            assert(heap_is_heap(heap));
            assert(indices_work(heap));
        }
    }
}

/* Prueba a fondo que los heap funcionen */
void test_heap(size_t heap_size)
{
    fclose(stdout);

    size = heap_size;

    /* Inicializamos el heap */
    heap = heap_init(size);

    /* Inicializamos los punteros */
    pointers = malloc(sizeof(int) * size);

    /* Inicializamos los indices */
    indices = malloc(sizeof(size_t*) * size);
    for(int i = 0; i < size; i++)
    {
        indices[i] = NULL;
    }

    /* Probamos que la insercion esté bien */
    test_insert(heap);

    for(int i = 0; i < size; i++)
    {
        printf("PASS #%d\n", i);

        /* Probamos que la actualización esté bien */
        test_update_key(heap);

        print_heap(heap);


        /* Probamos que la eliminacion esté bien */
        test_remove_at(heap);

        /* Rellenamos los espacios que quedaron vacios */
        reinsert_and_test(heap);

        print_heap(heap);
    }

    /* Probamos que la extracion esté bien */
    test_extract(heap);

    heap_destroy(heap);
    free(pointers);
    free(indices);
}

/* Importamos el módulo de Input / Output */
#include <stdio.h>
/* Importamos la librería estándar de C */
#include <stdlib.h>
/* Importamos el módulo de booleans */
#include <stdbool.h>
/* Importamos el módulo de strings */
#include <string.h>
/* Importamos el módulo de viaje en el tiempo */
#include <time.h>

/* El modulo encargado de resolver el puzzle */
#include "solver.h"

/** El orden del n-doku. El tablero tendrá tamaño n² x n² */
int n;

/* Indica como usar el programa */
bool how_to(char* program)
{
    printf("Faltan parámetros. Debes ejecutar %s n f, donde\n", program);
    printf("n es el orden del n-doku. n = 3 es sudoku tradicional\n");
    printf("f indica el modo de seguimiento:\n");
    printf("\tf = step para seguir al algoritmo paso a paso\n");
    printf("\tf = fast para mostrar solo la solución\n");
    return false;
}

/* Revisa que el programa haya sido ejecutado correctamente */
bool param_check(int argc, char** argv)
{
    /* Cantidad incorrecta de parametros */
    if(argc != 3)
    {
        return how_to(argv[0]);
    }

    /* Orden */
    n = atoi(argv[1]);
    if(n <= 0)
    {
        printf("n inválido. Debe ser entero mayor a 0\n");
        printf("Valor entregado: %d\n", n);
        return false;
    }

    /* Seguimiento */
    if(!strcmp(argv[2], "step"))
    {
        step = true;
    }
    else if(!strcmp(argv[2], "fast"))
    {
        step = false;
    }
    else
    {
        printf("f inválido. Debe ser step o fast\n");
        printf("Valor entregado: %s\n", argv[2]);
        return false;
    }
    return true;
}

/* Inicializa el tablero de n-doku y lo resuelve */
int main(int argc, char** argv)
{
    /* Revisa que el programa haya sido ejecutado correctamente */
    if(!param_check(argc, argv)) return 2;

    /* Comunicamos el n escogido */
    printf("%d\n", n);

    /* Inicializamos la grilla. Inicialmente todas las celdas estan vacías */
    nDoku* doku = n_doku_init_empty(n);

    /* Comunicamos el estado inicial del problema */
    n_doku_print(doku);

    /* Seteamos el contador en 0 */
    undo_count = 0;

    /* Medimos el tiempo */
    clock_t start = clock();

    /* El stack que guardará los pasos */
    Stack* steps = stack_init(n*n*n*n);

    /* Intentamos resolver el n-doku */
    if(solve_n_doku(doku, steps))
    {
        double time_used = ((double) (clock() - start)) / CLOCKS_PER_SEC;
        /* Imprimimos las estadisticas */
        fprintf(stderr, "%s ha resuelto ", argv[0]);
        fprintf(stderr, "%d-doku en %lf segundos,", n, time_used);
        fprintf(stderr, " volviendo %u veces\n", undo_count);
        /* Comunicamos el valor de cada celda en el estado solucion */
        while(!stack_is_empty(steps))
        {
            Cell* cell = stack_pop(steps);
            printf("%d %d %d\n", cell -> x, cell -> y, cell -> value);
        }
        /* Comunicamos que terminó el programa. El Watcher entenderá */
        printf("-1\n");
        /* Liberamos la memoria */
        n_doku_destroy(doku);
        /* No nos interesan los pasos más que el estado final */
        stack_destroy(steps);
        /* El programa terminó satisfactoriamente */
        return 0;
    }
    else
    {
        /* Indicamos que no se pudo resolver */
        fprintf(stderr,"%s","No solution exists");
        /* Comunicamos que terminó el programa. El Watcher entenderá */
        printf("-1\n");
        /* Liberamos la memoria */
        n_doku_destroy(doku);
        stack_destroy(steps);
        /* El programa terminó con errores */
        return 1;
    }
}

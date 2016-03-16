/* Importamos GTK+ */
#include <gtk/gtk.h>
/* Importamos el modulo de Input / Output */
#include <stdio.h>
/* Importamos la libreria estandar de C */
#include <stdlib.h>
/* Importamos el módulo de threads (POSIX) */
#include <pthread.h>
/* Importamos el modulo quee nos permite usar bools */
#include <stdbool.h>
/* Importamos el modulo de strings */
#include <string.h>
/* Definimos la constante SIZE que valdra 512 */
#define SIZE 64

/** Tamaño del tablero */
int board_size;

/** El estado del puzzle */
char* state;

char* buffer;


/** Thread que va modificando el offset cada 0.001 segundos */
int animate(void* canvas)
{
    if(!buffer)
      buffer = malloc(sizeof(char)*board_size);

    /* Repetimos hasta que: */
    if(
      fscanf(stdin, "%s", buffer) // El stream se cierre, o
      && strcmp(buffer,"END") != 0) // El bknd nos dé la instrucción de terminar.
    {

        for(int i = 0; i < board_size; i++)
        {
            state[i] = buffer[i];
        }

        //printf("Leído: %s\n", state);

        /* Indicamos a GTK que vuelva a dibujar la ventana */
        gtk_widget_queue_draw(canvas);

        return 1;
    }

    else
    {
      if(buffer)
        free(buffer);

      return 0;
    }

      //free(buffer);
}

/** Dibuja n arcos en el canvas */
static gboolean draw(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    /* Seleccionamos color negro*/
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.2);
    /* Pintamos el fondo de ese color */
    cairo_paint(cr);

    /* Indicamos que queremos lineas de 2 pixeles de grosor */
    cairo_set_line_width(cr,0);

    for (int i = 0; i < board_size; i++)
    {
        cairo_pattern_t* pat = cairo_pattern_create_radial (SIZE/2 + i * SIZE - SIZE/8, SIZE/2 - SIZE/6, 0,
                                   SIZE/2 + i * SIZE, SIZE/2, SIZE/2);
        cairo_pattern_add_color_stop_rgba (pat, 0, 1, 1, 1, 1);

        if(state[i] == 'L')
        {
            cairo_pattern_add_color_stop_rgba (pat, 1, 0, 1, 1, 1);
        }
        else if (state[i] == 'R')
        {
          cairo_pattern_add_color_stop_rgba (pat, 1, 1, 0, 0, 1);
        }
        else
        {
            continue;
        }
        cairo_set_source (cr, pat);
        /* Calculamos el centro del circulo */
        double cx = SIZE / 2 + i * SIZE;
        double cy = SIZE / 2;

        cairo_arc(cr,cx,cy,SIZE/2,0,2*G_PI);
        cairo_fill(cr);

        cairo_pattern_destroy(pat);
    }

    /* Indicamos que todo salió bien */
    return TRUE;
}

/* El main del programa. Esta es la función que se llamará al ejecutarlo */
int main (int argc, char *argv[])
{
    int refreshInterval = 100;

    if(argc > 1)
      refreshInterval = atoi(argv[1]);

    fscanf(stdin, "%d", &board_size);

    state = malloc(sizeof(char) * board_size);

    /* Cerramos el canal de errores ya que GTK mete mucho ruido por ahi */
    //fclose(stderr);

    /* Inicializamos GTK */
    gtk_init (0, NULL);

    /* Inicializamos la ventana */
    GtkWidget* window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    /* Linkeamos los eventos de la ventana */
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Inicializamos el canvas */
    GtkWidget* drawingArea = gtk_drawing_area_new();
    /* Linkeamos los eventos al canvas */
    g_signal_connect(drawingArea, "draw", G_CALLBACK(draw),  NULL);

    /* Indicamos que queremos un canvas de SIZE * board_size x SIZE */
    gtk_widget_set_size_request (drawingArea, SIZE * board_size, SIZE);

    /* Conectamos el canvas a la ventana */
    gtk_container_add (GTK_CONTAINER (window), drawingArea);
    gtk_widget_show(drawingArea);
    gtk_widget_show(window);

    // This worked but is from GTK2
    //g_timeout_add(100, animate, drawingArea);

    // This is the alternative, from GTK3. It seems.
    gdk_threads_add_timeout(refreshInterval, animate, drawingArea);

    /* Comenzamos el GTK Main Loop */
    gtk_main();

    free(state);

    /* Indicamos que todo salio de perillas */
    return 0;
}

/* Importamos GTK+ */
#include <gtk/gtk.h>
/* Importamos el modulo de Input / Output */
#include <stdio.h>
/* Importamos la libreria estandar de C */
#include <stdlib.h>
/* Importamos el módulo de threads (POSIX) */
#include <pthread.h>
/* Importamos el modulo que nos permite usar bools */
#include <stdbool.h>

/* Definimos la constante SIZE que valdra 512 */
#define SIZE 512

/** El thread encargado de la animación */
pthread_t* animation_thread;

/* Indicamos que vamos a querer dibujar 10 arcos */
double n = 10;

/* El offset de cada arco estará dado por alpha, el cual irá cambiando */
double alpha = 0;

/** Thread que va modificando el offset cada 0.001 segundos */
void* animate(void* canvas)
{
    /* Por el resto de la vida */
    while(true)
    {
        /* Aumentamos el offset en 0.001 pixeles */
        alpha += 0.001;
        /* Dormimos el thread por 1000 microsegundos */
        usleep(1000);
        /* Indicamos a GTK que vuelva a dibujar la ventana */
        gtk_widget_queue_draw(canvas);
    }

    /* Indicamos que terminó el thread */
    pthread_exit(NULL);
}

/** Inicializa el thread que animará el programa */
void spawn_animator(GtkWidget *widget, gpointer user_data)
{
    /* Inicializamosel thread */
    animation_thread = malloc(sizeof(pthread_t));
    /* Y lo lanzamos */
    pthread_create(animation_thread, NULL, animate, widget);
}

/** Dibuja n arcos en el canvas */
static gboolean draw(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    /* Seleccionamos color negro*/
    cairo_set_source_rgb(cr, 0, 0, 0);
    /* Pintamos el fondo de ese color */
    cairo_paint(cr);

    /* Seleccionamos un color verde lindo */
    cairo_set_source_rgb(cr, 0.17, 0.63, 0.12);
    /* Indicamos que queremos lineas de 2 pixeles de grosor */
    cairo_set_line_width(cr,2);

    /* Calculamos el centro de la ventana */
    double cx = SIZE / 2;
    double cy = SIZE / 2;

    /* Calculamos el radio de la circunferencia inscrita en el canvas */
    double er = SIZE / 2;

    /* Cada arco medirá PI, medio circulo */
    double delta = -G_PI;

    /* Repetimos el proceso n veces */
    for (double i = 0; i < n; i++)
    {
        /* El radio del arco actual depende de cual sea */
        double r = er * i / n;
        /* Recorremos un arco con un offset de acuerdo a que arco es */
        cairo_arc_negative(cr, cx, cy, r, alpha * i, alpha * i + delta);
        /* Marcamos ese camino con tinta */
        cairo_stroke(cr);
    }

    /* Indicamos que todo salió bien */
    return TRUE;
}

/* El main del programa. Esta es la función que se llamará al ejecutarlo */
int main (int argc, char *argv[])
{
    /* Cerramos el canal de errores ya que GTK mete mucho ruido por ahi */
    fclose(stderr);

    /* Saludamos al mundo por la consola */
    printf("Hello world! I'm a C & GTK+ program with a threaded animation :D\n");

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
    g_signal_connect(drawingArea, "realize", G_CALLBACK(spawn_animator), NULL);

    /* Indicamos que queremos un canvas de SIZE x SIZE */
    gtk_widget_set_size_request (drawingArea, SIZE, SIZE);

    /* Conectamos el canvas a la ventana */
    gtk_container_add (GTK_CONTAINER (window), drawingArea);
    gtk_widget_show(drawingArea);
    gtk_widget_show(window);

    /* Comenzamos el GTK Main Loop */
    gtk_main();

    /* Indicamos que todo salio de perillas */
    return 0;
}

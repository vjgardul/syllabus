/* Importamos GTK+ */
#include <gtk/gtk.h>
/* Imprtamos la libreria de I/O */
#include <stdio.h>

/* Definimos la constante SIZE que valdra 512 */
#define SIZE 512

/* Indicamos que vamos a querer dibujar 10 arcos */
double n = 10;

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

    /* El offset de cada arco estará dado por alpha */
    double alpha = -2*G_PI / n;

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
    printf("Hello world! I'm a C & GTK+ program :P\n");

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

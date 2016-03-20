/* Importamos GTK+ */
#include <gtk/gtk.h>
/* Importamos el modulo de Input / Output */
#include <stdio.h>
/* Importamos la libreria estandar de C */
#include <stdlib.h>
/* Importamos el modulo quee nos permite usar bools */
#include <stdbool.h>
/* Importamos el modulo de strings */
#include <string.h>
#include <stdint.h>

/* Tamaño de la ventana */
#define WINDOW 512.0


/* Se considera que una celda no esta asignada si vale 0 */
#define UNASSIGNED 0

/* Indica el tamaño de las celdas */
static double cell_size = 1;

/* El tamaño de la fuente */
double SIZE;

/** El tablero será de (nxn)x(nxn) */
static int n;

/* La grilla */
static uint8_t** grid;

/** Funcion que lee la siguiente jugada y actualiza el tablero segun eso */
int update(void* canvas)
{
    int i,j,v;
    /* Si alguno de los numeros es negativo o falla, dejamos de llamar */
    if(!fscanf(stdin,"%d",&i) || i < 0) return 0;
    if(!fscanf(stdin,"%d",&j) || j < 0) return 0;
    if(!fscanf(stdin,"%d",&v) || v < 0) return 0;

    grid[i][j] = v;

    gtk_widget_queue_draw(canvas);

    /* All is well, please continue */
    return 1;
}

/** Dibuja n arcos en el canvas */
static gboolean draw(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    /* Seleccionamos color blanco */
    cairo_set_source_rgb(cr, 1, 1, 1);
    /* Pintamos el fondo de ese color */
    cairo_paint(cr);

    /* Dibujamos lineas claras y delgadas */
    cairo_set_line_width(cr,SIZE/32);
    cairo_set_source_rgb(cr,0.6,0.6,0.6);

    /* Dibuja la grilla clara del sudoku */
    for (int i = 0; i < n * n; i++)
    {
        /* Si i no es un multiplo de n */
        if(i % n)
        {
            /* Lineas verticales */
            cairo_move_to(cr, i * cell_size, 0);
            cairo_rel_line_to(cr, 0,cell_size * n * n);
            cairo_stroke(cr);
            /* Lineas horizontales */
            cairo_move_to(cr, 0, i * cell_size);
            cairo_rel_line_to(cr, cell_size * n * n, 0);
            cairo_stroke(cr);
        }
    }

    /* Dibujamos lineas grueasa y oscuras */
    cairo_set_line_width(cr, SIZE/16);
    cairo_set_source_rgb(cr,0,0,0);

    for (int i = 0; i < n; i++)
    {
        /* Lineas verticales */
        cairo_move_to(cr, i * cell_size * n, 0);
        cairo_rel_line_to(cr, 0, cell_size * n * n);
        cairo_stroke(cr);
        /* Lineas horizontales */
        cairo_move_to(cr, 0, i * cell_size * n);
        cairo_rel_line_to(cr, cell_size * n * n, 0);
        cairo_stroke(cr);
    }


    cairo_text_extents_t te;
    cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
    cairo_select_font_face (cr, "monospace",
        CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size (cr, SIZE);
    char text[n];

    for(int i = 0; i < n*n; i++)
    {
        for(int j = 0; j < n*n; j++)
        {
            if(!grid[i][j]) continue;

            sprintf(text,"%hhu",grid[i][j]);

            cairo_text_extents (cr, text, &te);
            cairo_move_to (cr, cell_size / 2 + cell_size * i - te.width / 2 - te.x_bearing,
                cell_size / 2 + cell_size * j - te.height / 2 - te.y_bearing);
            cairo_show_text (cr, text);
        }
    }

    /* Indicamos que todo salió bien */
    return TRUE;
}

/* El main del programa. Esta es la función que se llamará al ejecutarlo */
int main (int argc, char *argv[])
{
    /* Leemos el tamaño del tablero */
    fscanf(stdin, "%d", &n);

    grid = malloc(sizeof(uint8_t*) * n * n);
    for(int i = 0; i < n*n; i++)
    {
        grid[i] = malloc(sizeof(uint8_t) * n * n);
    }

    int i,j,v;

    /* Leemos el estado inicial del tablero */
    for(int k = 0; k < n*n*n*n; k++)
    {
        /* Si alguno de los numeros es negativo o falla, dejamos de llamar */
        if(!fscanf(stdin,"%d",&i) || i < 0) return 1;
        if(!fscanf(stdin,"%d",&j) || j < 0) return 1;
        if(!fscanf(stdin,"%d",&v) || v < 0) return 1;

        grid[i][j] = v;
    }

    /* Indicamos que hay que actualizar cada cierta cantidad de ticks */
    int refreshInterval = argc > 1 ? atoi(argv[1]) : 10;

    /* Cerramos el canal de errores ya que GTK mete mucho ruido por ahi */
    fclose(stderr);

    /* Inicializamos GTK */
    gtk_init (0, NULL);

    /* Inicializamos la ventana */
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    /* Linkeamos los eventos de la ventana */
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    /* Asignamos el nombre de la ventana */
    char windowname[32];
    sprintf(windowname, "WATCH %d-DOKU %d X %d",n,n*n,n*n);
    gtk_window_set_title(GTK_WINDOW(window),windowname);

    /* Inicializamos el canvas */
    GtkWidget* drawingArea = gtk_drawing_area_new();
    /* Linkeamos los eventos al canvas */
    g_signal_connect(drawingArea, "draw", G_CALLBACK(draw),  NULL);

    int max = n*n;
    double x = 1;
    while(max > 10)
    {
        x *= 1.5;
        max /= 10;
    }
    // cell_size = SIZE * x;

    cell_size = WINDOW / (n*n);

    SIZE = cell_size / x;

    /* Indicamos el tamaño deseado para el canvas */
    // gtk_widget_set_size_request (drawingArea, cell_size*n*n, cell_size*n*n);
    gtk_widget_set_size_request (drawingArea, WINDOW, WINDOW);

    /* Conectamos el canvas a la ventana */
    gtk_container_add(GTK_CONTAINER(window), drawingArea);
    gtk_widget_show(drawingArea);
    gtk_widget_show(window);

    /* Indicamos a GTK que debe llamar a esta funcion constantemente */
    gdk_threads_add_timeout(refreshInterval, update, drawingArea);

    /* Comenzamos el GTK Main Loop */
    gtk_main();

    for(int i = 0; i < n*n; i++)
    {
        free(grid[i]);
    }
    free(grid);

    /* Indicamos que todo salio de perillas */
    return 0;
}

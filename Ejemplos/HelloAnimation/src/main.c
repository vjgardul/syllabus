#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define WINDOW_SIZE  512

double deltissimo = 0;

/** El thread, porque es bueno tenerlo */
pthread_t* animation_thread;

static gboolean draw(GtkWidget *widget, cairo_t *cr, gpointer data)
{

    /* Set color for background */
    cairo_set_source_rgb(cr, 0, 0, 0);
    /* fill in the background color*/
    cairo_paint(cr);

    /* draw circle */
    cairo_set_source_rgb(cr, 0.17, 0.63, 0.12);
    cairo_set_line_width(cr,2);

    double cx = WINDOW_SIZE / 2;
    double cy = WINDOW_SIZE / 2;

    double er = WINDOW_SIZE / 2;

    double n = 10;

    double alpha = -2*G_PI / n;
    double delta = -G_PI;


    for (double i = 0; i < n; i++)
    {
        double r = er * i / n;
        cairo_arc_negative(cr, cx, cy, r, alpha * i + i * deltissimo, alpha * i + delta + i * deltissimo);
        cairo_stroke(cr);
    }

    return TRUE;
}

void* animate(void* canvas)
{
    struct timespec pause;
    pause.tv_sec = 0;
    pause.tv_nsec = 999999;

    while(1)
    {
        deltissimo += 0.001;
        nanosleep(&pause,NULL);
        gtk_widget_queue_draw(canvas);
    }

    pthread_exit(NULL);
}

void spawn_animator(GtkWidget *widget, gpointer user_data)
{
    animation_thread = malloc(sizeof(pthread_t));
    pthread_create(animation_thread, NULL, animate, widget);
}


int main (int argc, char *argv[])
{
    fclose(stderr);

    printf("Hello world! I'm a C & GTK+ program with a threaded animation :D\n");

    GtkWidget *window;
    GtkWidget *drawingArea;

    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    drawingArea = gtk_drawing_area_new();
    gtk_widget_set_size_request (drawingArea, WINDOW_SIZE, WINDOW_SIZE);
    g_signal_connect(drawingArea, "realize", G_CALLBACK(spawn_animator), NULL);
    g_signal_connect(drawingArea, "draw", G_CALLBACK(draw),  NULL);

    gtk_container_add (GTK_CONTAINER (window), drawingArea);
    gtk_widget_show(drawingArea);
    gtk_widget_show(window);

    gtk_main ();

    return 0;
}

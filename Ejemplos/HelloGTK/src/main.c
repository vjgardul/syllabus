#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_SIZE  512

static gboolean draw_cb(GtkWidget *widget, cairo_t *cr, gpointer data)
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
        cairo_arc_negative(cr, cx, cy, r, alpha * i, alpha * i + delta);
        cairo_stroke(cr);
    }

    return TRUE;
}

int main (int argc, char *argv[])
{
    fclose(stderr);

    printf("Hello world! I'm a C & GTK+ program :P\n");

    GtkWidget *window;
    GtkWidget *da;

    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    da = gtk_drawing_area_new();
    gtk_widget_set_size_request (da, WINDOW_SIZE, WINDOW_SIZE);
    g_signal_connect(da, "draw", G_CALLBACK(draw_cb),  NULL);

    gtk_container_add (GTK_CONTAINER (window), da);
    gtk_widget_show(da);
    gtk_widget_show(window);

    gtk_main ();

    return 0;
}

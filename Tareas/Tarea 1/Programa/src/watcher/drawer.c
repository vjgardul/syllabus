#include "drawer.h"
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <cairo.h>
#include <cairo-pdf.h>

/** El plano de la ciudad que estamos dibujando */
Layout* the_layout = NULL;

/** Define el ancho de la linea relativo al tamaño de las celdas */
#define LINE_SIZE (CELL_SIZE / 128.0)

#define BORDER_SIZE (LINE_SIZE * 3.5)

#define CURVE_SIZE (LINE_SIZE * 6)

/** Mantiene registro de la pasada en la que va el render */
static uint8_t draw_count;

/** Indica cuanto se debe rotar el centro del arco para un par dado */
const int center_rotation[8][8] =
{
    {-1,0,0,0,0,5,6,7},
    {0,-1,1,1,1,1,6,7},
    {0,1,-1,2,2,2,2,7},
    {0,1,2,-1,3,3,3,3},
    {0,1,2,3,-1,4,4,4},
    {5,1,2,3,4,-1,5,5},
    {6,6,2,3,4,5,-1,6},
    {7,7,7,3,4,5,6,-1},
};

/** Indica el angulo de inicio de los arcos para un par dado */
const int starting_angles[8][8] =
{
    {-1,3,4,5,-1,2,2,2},
    {3,-1,4,5,6,-1,3,3},
    {4,4,-1,5,6,7,-1,4},
    {5,5,5,-1,6,7,0,-1},
    {-1,6,6,6,-1,7,0,1},
    {2,-1,7,7,7,-1,0,1},
    {2,3,-1,0,0,0,-1,1},
    {2,3,4,-1,1,1,1,-1},
};

/** Rota el vector (x,y) en 'theta' grados en sentido antihorario */
static void point_rotate(double* x, double* y, double theta)
{
    double aux = (*x) * cos(theta) - (*y) * sin(theta);
    *y = (*x) * sin(theta) + (*y) * cos(theta);
    *x = aux;
}

/** Obtiene el valor RGB de un color dado */
static void color_dip(cairo_t* cr, Color color)
{
    switch (color)
    {
        case red:    cairo_set_source_rgb(cr,235.0/255.0,52.0/255.0,65.0/255.0); break;
        case yellow: cairo_set_source_rgb(cr,249.0/255.0,202.0/255.0,30.0/255.0); break;
        case green:  cairo_set_source_rgb(cr,25.0/255.0,171.0/255.0,101.0/255.0); break;
        case blue:   cairo_set_source_rgb(cr,70.0/255.0,79.0/255.0,171.0/255.0); break;
        case purple: cairo_set_source_rgb(cr,183.0/255.0,90.0/255.0,171.0/255.0); break;
        case orange: cairo_set_source_rgb(cr,252.0/255.0,98.0/255.0,50.0/255.0); break;
        case teal: cairo_set_source_rgb(cr,20.0/255.0,171.0/255.0,191.0/255.0); break;
        // case 8: cairo_set_source_rgb(cr,0.5,0.5,0.5); break;
        default: cairo_set_source_rgb(cr,0.7,0.7,0.7); break;

    }
}

/** Traza una linea del borde de un circulo al del otro */
static void link_circles(cairo_t* cr, double x1, double y1, double r1,
                                      double x2, double y2, double r2)
{
    /* Direccion */
    double dirx = x2 - x1;
    double diry = y2 - y1;

    /* Magnitud de la direccion */
    double mag = sqrt(dirx*dirx + diry*diry);

    /* Direccion unitaria */
    double dirxu = dirx / mag;
    double diryu = diry / mag;

    /* El punto de partida de la recta */
    double startx = x1 + dirxu * r1;
    double starty = y1 + diryu * r1;

    /** Distancia que recorre la recta */
    double dist = mag - r1 - r2;

    /* Trazamos la linea */
    cairo_move_to(cr, startx-dirxu * LINE_SIZE, starty - diryu*LINE_SIZE);
    cairo_line_to(cr, startx + dirxu * (dist+LINE_SIZE), starty + diryu * (dist+LINE_SIZE));
}

/** Dibuja un arco de radio 'r', centro (cx,cy), desde 's' a 'f' */
static void
draw_arc(cairo_t* cr, double cx, double cy, double r, double s, double f)
{
    double delta = G_PI / 180.0;
    cairo_arc_negative(cr,cx,cy,r,-(s - delta),-(f + delta));
}

/** Dibuja un circulo de radio 'r' centrado en (cx,cy) */
static void draw_circle(cairo_t* cr, double cx, double cy, double r)
{
    draw_arc(cr, cx, cy, r, 0, 2*G_PI);
}

/** Obtiene el radio de un core */
static double core_radius(Core* core)
{
    uint8_t n = core -> sides;

    /** TODO Ancho? */
    double width = n == 8 ? CELL_SIZE : CELL_SIZE / 2;
    /** Radio de la circunferencia circunscrita a esta celda */
    double arc = n == 8 ? width / 2 : width * tan(G_PI/8);

    /* Radio del core */
    double radius = n == 8 ? width / (cos(G_PI/8) * 2) : arc * sqrt(2);

    return radius / 3;
}

/** Dibuja un nucleo */
static void draw_core(cairo_t* cr, Core* core)
{
    /** Obtenemos el radio de este core */
    double r = core_radius(core);
    /* Centro de la celda en espacio de imagen */
    double cx = CELL_SIZE / 2 + core -> x * CELL_SIZE/2;
    double cy = CELL_SIZE / 2 + core -> y * CELL_SIZE/2;

    /* Marcamos la posicion de este core para el que la necesite */
    core -> buildings[0] -> x = cx;
    core -> buildings[0] -> y = cy;


    draw_circle(cr, cx, cy, r + (BORDER_SIZE + CURVE_SIZE / 2)/2);

    // cairo_set_source_rgb(cr,120.0/255.0,120.0/255.0,120.0/255.0);
    cairo_set_source_rgb(cr,0,0,0);
    cairo_fill_preserve(cr);
    cairo_set_line_width(cr,BORDER_SIZE + CURVE_SIZE / 2);
    cairo_stroke(cr);




    color_dip(cr, core -> buildings[0] -> color);
    double mult = 1;
    while(mult > 0.1)
    {
        cairo_set_line_width(cr, mult * CURVE_SIZE);
        draw_circle(cr, cx, cy, mult * r);
        cairo_stroke(cr);
        if(core -> sides == 8)
        {
            mult *= 0.5;
        }
        else
        {
            mult *= 0.4;

        }
    }


    /* Se marca como que se dibujó en la pasada anterior */
    /* Asi siempre estará marcado como que no se ha dibujado */
    core -> buildings[0] -> drawn = draw_count - 1;
}

/** Dibuja la orilla de una celda, ya sea de 8 o 4 lados */
static void draw_cell_border(cairo_t* cr, Zone* cell)
{
    /* Vemos que tipo de celda es */
    uint8_t n = cell -> sides;
    /* Segun eso definimos sus parametros geometricos */

    /** TODO La mitad del ancho de una celda de 8 */
    double width = CELL_SIZE / 2;
    /** El tamaño de los lados de las celdas */
    double side = width * tan(G_PI/8);
    /** Radio de la circunferencia circunscrita a la celda */
    double radius = n == 8 ? width / cos(G_PI/8) : side * sqrt(2);
    /** Centro de la celda en espacio de imagen */
    double cx = CELL_SIZE / 2 + cell -> x * CELL_SIZE/2;
    double cy = CELL_SIZE / 2 + cell -> y * CELL_SIZE/2;

    /** Angulo entre punto y punto */
    double alpha = n == 8 ? G_PI / 4 : G_PI / 2;
    /** Angulo del punto de partida */
    double delta = n == 8 ? G_PI / 8 : 0;

    /* Primer vertice de la celda */
    cairo_move_to(cr, cx + radius * cos(delta), cy - radius * sin(delta));

    /* Se avanza a lo largo de la circunferencia en busca de los vertices */
    for(int i = 1; i < n; i++)
    {
        double x = cx + radius * cos(delta + alpha * i);
        double y = cy - radius * sin(delta + alpha * i);
        /* Se lanza una linea al siguiente vertice */
        cairo_line_to(cr,x,y);
    }

    /* Se cierra el polígono */
    cairo_close_path(cr);

    /* Se dibuja en el lienzo */
    cairo_set_source_rgb(cr, 47.0/255.0, 47.0/255.0, 67.0/255.0);
    cairo_fill_preserve(cr);
    cairo_set_source_rgb(cr,0.5,0.5,0.5);
    cairo_set_line_width(cr,1 * LINE_SIZE);
    cairo_stroke(cr);
}

/** Dibuja los nodos de una celda */
static void draw_cell_nodes(cairo_t* cr, Zone* cell)
{
    /* Vemos que tipo de celda es */
    uint8_t n = cell -> sides;
    /* Segun eso definimos sus parametros geometricos */

    /** TODO Ancho? */
    double width = n == 8 ? CELL_SIZE : CELL_SIZE / 2;
    /** Radio de la circunferencia circunscrita a esta celda */
    double arc = n == 8 ? width / 2 : width * tan(G_PI/8);

    /* Centro de la celda en espacio de imagen */
    double cx = CELL_SIZE / 2 + cell -> x * CELL_SIZE/2;
    double cy = CELL_SIZE / 2 + cell -> y * CELL_SIZE/2;

    /** Angulo entre nodo y nodo */
    double alpha = n == 8 ? G_PI / 4 : G_PI / 2;
    /** Angulo del nodo inicial */
    double delta = n == 8 ? 0 : G_PI / 4;

    /** Radio de un nodo */
    double r = CELL_SIZE / 20;

    /* Por cada nodo en la celda */
    for(int i = 0; i < cell -> building_count; i++)
    {
        /* Tomamos la orientacion de ese nodo */
        uint8_t dir = cell -> buildings[i] -> direction;
        /* Segun eso definimos su posicion */
        double x = cx + (arc - 2*r) * cos(delta + alpha * dir);
        double y = cy - (arc - 2*r) * sin(delta + alpha * dir);

        /* Guardamos la posicion para dibujar las aristas entre nodos */
        cell -> buildings[i] -> x = x;
        cell -> buildings[i] -> y = y;

        /* Dibujamos la circunferencia */
        draw_circle(cr, x, y, r);

        /* La pintamos en el lienzo */
        color_dip(cr, cell -> buildings[i] -> color);
        cairo_set_line_width(cr,1 * LINE_SIZE);

        if(cell -> buildings[i] -> color)
        {
            /* Los nodos que tengan color se rellenan */
            cairo_fill(cr);
        }
        else
        {
            /* Los que no, se dibuja solo el borde */
            cairo_stroke(cr);
        }
    }

}

/** Dibuja una arista entre 2 nodos */
static void draw_edge(cairo_t* cr, Building* start, Building* end)
{
    /* Si ya se dibujó en esta pasada, nada que hacer aqui */
    if(end -> drawn == draw_count) return;

    /* Radio de los nodos */
    double r1 = CELL_SIZE / 20;
    double r2 = CELL_SIZE / 20;

    /* Si uno es core */
    if(start -> zone -> core) r1 = core_radius(start -> zone);
    if(end -> zone -> core) r2 = core_radius(end -> zone);

    /* Dibujamos la arista */
    link_circles(cr, start -> x, start -> y, r1, end -> x, end -> y, r2);
}

/** Dibuja una arista muy linda de un nodo a otro */
static void draw_fancy_edge(cairo_t* cr, Building* start, Building* end)
{
    /* Si ya se dibujó en esta pasada, nada que hacer aqui */
    if(end -> drawn == draw_count) return;

    /* El nodo inicial manda */
    Zone* cell = start -> zone;
    /* El centro de la celda */
    double cx = CELL_SIZE / 2 + cell -> x * CELL_SIZE/2;
    double cy = CELL_SIZE / 2 + cell -> y * CELL_SIZE/2;

    /* Es una arista dentro del la misma zona */
    if(start -> zone == end -> zone)
    {

        /* Los indices de los nodos */
        uint8_t i1 = start -> direction;
        uint8_t i2 = end -> direction;
        /* Numero para identificar que clase de arista es */
        uint8_t code = abs(i2 - i1);

        /* TODO agregar el delta para que no se vea feo */
        /* Se trata de una recta que cruza la celda*/
        if(code == cell -> sides / 2)
        {
            /** Coordenadas de la recta */
            double xi,yi,theta;
            /* Celda de 4 */
            if(cell -> sides == 4)
            {
                double side =  CELL_SIZE * tan(G_PI/8);
                /* Punto inicial */
                xi = side / 2 / sqrt(2);
                yi = side / 2 / sqrt(2);
                /* Rotacion de la recta */
                theta = center_rotation[i1 * 2][i2 * 2] * G_PI / 4;
            }
            /* Celda de 8 */
            else
            {
                /* Punto inicial */
                xi = CELL_SIZE / 2;
                yi = 0;
                /* Rotacion de la recta */
                theta = center_rotation[i1][i2] * G_PI / 4;
            }
            /* Punto final */
            double xf = -xi;
            double yf = -yi;
            /* Rotar la recta */
            point_rotate(&xi, &yi, theta);
            point_rotate(&xf, &yf, theta);
            /* Trazarla */
            cairo_move_to(cr,cx + xi,cy - yi);
            cairo_line_to(cr,cx + xf,cy - yf);
        }
        /* Una hermosa curva que va de un lado a otro */
        else
        {
            /* Parametros del arco */
            double arcx, arcy, radius, arci, arcf, center_rotate;

            if(cell -> sides == 4)
            {
                double side =  CELL_SIZE * tan(G_PI/8);
                arcx = 0;
                arcy = sqrt(2) * side / 2;
                radius = side / 2;

                center_rotate = center_rotation[i1 * 2][i2 * 2] / 2;
                center_rotate *= G_PI / 2;

                arci = starting_angles[i1 * 2][i2 * 2] * G_PI / 4;
                arci += G_PI / 4;

                arcf = arci + G_PI / 2;
            }
            /* Arista en una celda de 8 */
            else
            {
                uint8_t delta;
                /* Arista que va del nodo i al i + 3 */
                if(code == 3 || code == 5)
                {
                    arcx = CELL_SIZE / 2;
                    arcy = CELL_SIZE * (tan(G_PI/4) + tan(G_PI/8) / 2);
                    delta = 1;
                }
                /* Arista que va del nodo i al i + 2 */
                else if(code == 2 || code == 6)
                {
                    arcx = CELL_SIZE / 2;
                    arcy = CELL_SIZE / 2;
                    delta = 2;
                }
                /* Solo queda una opcion: arista que va del nodo i al i + 1 */
                else
                {
                    arcx = CELL_SIZE / 2;
                    arcy = arcx * tan(G_PI / 8);
                    delta = 3;
                }

                radius = arcy;

                center_rotate = center_rotation[i1][i2];
                center_rotate *= G_PI / 4;

                arci = starting_angles[i1][i2] * G_PI / 4;
                arcf = arci + delta * G_PI / 4;
            }

            point_rotate(&arcx, &arcy, center_rotate);
            draw_arc(cr, cx + arcx, cy - arcy, radius, arci, arcf);
        }
    }
    /* El otro nodo es un core */
    else if(end -> zone -> core)
    {
        if(start -> zone -> core)
        {
            double r1 = core_radius(start -> zone) + CURVE_SIZE / 2;
            double r2 = core_radius(end -> zone) + CURVE_SIZE / 2;

            link_circles(cr, start -> x, start -> y, r1, end -> x, end -> y, r2);
        }
        else
        {
            /* Obtenemos el radio de ese core */
            double r = core_radius(end -> zone) + CURVE_SIZE / 2;

            /* Coordenadas correspondientes al nodo */
            double startx, starty, theta;
            if(start -> zone -> sides == 8)
            {
                startx = CELL_SIZE / 2;
                starty = 0;

                theta = start -> direction * G_PI / 4;
            }
            else
            {
                double side =  CELL_SIZE * tan(G_PI/8);
                startx = side / 2 / sqrt(2);
                starty = side / 2 / sqrt(2);

                theta = start -> direction * G_PI / 2;
            }
            point_rotate(&startx, &starty, theta);

            /* Se conectan ambos */
            link_circles(cr, cx + startx, cy - starty, 0, end -> x, end -> y, r);
        }
    }
}

/* Dibuja las aristas para cada nodo de esta celda */
static void draw_fancy_links(cairo_t* cr, Zone* cell)
{
    /* Por cada nodo de la celda */
    for(int i = 0; i < cell -> building_count; i++)
    {
        /* La arista toma el color del nodo */
        for(int j = 0; j < cell -> buildings[i] -> link_count; j++)
        {
            draw_fancy_edge(cr, cell-> buildings[i], cell-> buildings[i]-> linked[j]);

            // cairo_set_source_rgb(cr,220.0/255.0,220.0/255.0,220.0/255.0);
            // cairo_set_line_width(cr, CURVE_SIZE + 2 * BORDER_SIZE + 3 * LINE_SIZE);
            // cairo_stroke_preserve(cr);

            cairo_set_source_rgb(cr,0,0,0);
            cairo_set_line_width(cr, CURVE_SIZE + 2* BORDER_SIZE);
            cairo_stroke_preserve(cr);

            cairo_set_line_cap(cr,CAIRO_LINE_CAP_ROUND);

            color_dip(cr, cell -> buildings[i] -> color);
            cairo_set_line_width(cr, CURVE_SIZE);
            cairo_stroke(cr);

            cairo_set_line_cap(cr,CAIRO_LINE_CAP_BUTT);

        }
        /* Marcamos que ya pasamos por este nodo en esta pasada */
        cell -> buildings[i] -> drawn = draw_count;
    }
}

/* Dibuja las aristas para cada nodo de esta celda */
static void draw_raw_links(cairo_t* cr, Zone* cell)
{
    cairo_set_line_width(cr, 2 * LINE_SIZE);
    /* Por cada nodo de la celda */
    for(int i = 0; i < cell -> building_count; i++)
    {
        /* La arista toma el color del nodo */
        color_dip(cr, cell -> buildings[i] -> color);
        for(int j = 0; j < cell -> buildings[i] -> link_count; j++)
        {
            draw_edge(cr, cell-> buildings[i], cell-> buildings[i]-> linked[j]);

            /* La dibujamos en el lienzo */
            color_dip(cr, cell-> buildings[i] -> color);
            cairo_set_line_width(cr,2 * LINE_SIZE);
            cairo_stroke(cr);
        }
        /* Marcamos que ya pasamos por este nodo en esta pasada */
        cell -> buildings[i] -> drawn = draw_count;
    }
}

static gboolean draw(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    /* TODO dibujar solo lo que ha cambiado */

    /* Si es que es nulo no nos sirve */
    if(!the_layout) return FALSE;

    /* Se cambia el numero de la pasada en la que vamos */
    draw_count++;

    /* Set color for background */
    cairo_set_source_rgb(cr, 27.0/255.0, 27.0/255.0, 47.0/255.0);


    /* Fill in the background color*/
    cairo_paint(cr);


    /* Siempre se deben dibujar las celdas */
    for(int i = 0; i < the_layout -> zone_count; i++)
    {
        draw_cell_border(cr, the_layout -> zones[i]);
    }
    for(int i = 0; i < the_layout -> core_count; i++)
    {
        draw_cell_border(cr, the_layout -> cores[i]);
    }

    /* Esto se debe dibujar siempre */
    for(int i = 0; i < the_layout -> core_count; i++)
    {
        draw_core(cr, the_layout -> cores[i]);
    }

    /* Si se quiere dibujar el lado oculto del problema */
    if(draw_inner)
    {
        for(int i = 0; i < the_layout -> zone_count; i++)
        {
            /* Necesariamente se van a dibujar los nodos */
            draw_cell_nodes(cr, the_layout -> zones[i]);
            draw_raw_links(cr, the_layout -> zones[i]);
        }
        for(int i = 0; i < the_layout -> core_count; i++)
        {
            draw_raw_links(cr, the_layout -> cores[i]);
        }
    }
    /* Si se quiere dibujar el lado lindo del problema */
    else
    {
        for(int i = 0; i < the_layout -> zone_count; i++)
        {
            draw_fancy_links(cr, the_layout -> zones[i]);
        }
        for(int i = 0; i < the_layout -> core_count; i++)
        {
            draw_fancy_links(cr, the_layout -> cores[i]);
        }
    }

    return TRUE;
}

void client_paint(Client* target, Color color)
{
    if(target -> color != color)
    {
        target -> color = color;
        for(int i = 0; i < target -> link_count; i++)
        {
            client_paint(target -> linked[i], color);
        }
    }
}

struct choice
{
    Client* choser;
    Client* chosee;
    Client* tinted;
};


/* Las desiciones que hemos tomado */
struct choice* decisions;
size_t decision_count;

int update(void* canvas)
{
    char buffer[6];
    size_t cindex;
    uint8_t nindex;

    Client* node1;
    Client* node2;

    if(fscanf(stdin, "%s", buffer) && strcmp(buffer,"END"))
    {
        if(!strcmp(buffer, "LINK"))
        {
            if(!fscanf(stdin,"%zu", &cindex))
            {
                free(decisions);
                return 0;
            }
            if(!fscanf(stdin,"%hhu",&nindex))
            {
                free(decisions);
                return 0;
            }
            node1 = the_layout -> zones[cindex] -> buildings[nindex];
            if(!fscanf(stdin,"%hhu",&nindex))
            {
                free(decisions);
                return 0;
            }
            node2 = the_layout -> zones[cindex] -> buildings[nindex];

            struct choice c;
            c.choser = node1;
            c.chosee = node2;
            c.tinted = NULL;

            city_client_link(node1, node2);
            // node_link_print(node1, node2);
            if(node1 -> color != node2 -> color)
            {
                if(node1 -> color)
                {
                    client_paint(node2, node1 -> color);
                    c.tinted = node2;
                }
                else
                {
                    client_paint(node1, node2 -> color);
                    c.tinted = node1;
                }
            }
            decisions[decision_count++] = c;
        }
        else if(!strcmp(buffer,"UNDO"))
        {
            struct choice c = decisions[--decision_count];
            city_client_link_undo(c.choser, c.chosee);

            if(c.tinted)
            {
                client_paint(c.tinted, none);
            }
        }
        else
        {
            free(decisions);
            return 0;
        }

        /* TODO mandar a actualizar solo lo que ha cambiado */
        gtk_widget_queue_draw(canvas);

    }
    else
    {
        free(decisions);
        return 0;
    }
    return 1;
}

void view(Layout* layout, int refreshInterval)
{
    the_layout = layout;

    /* Se cierra el canal para errores para que GTK no moleste */
    fclose(stderr);

    /* Inicialmente no se ha dibujado ninguna vez */
    draw_count = 0;

    /* Inicializar GTK */
    gtk_init(0, NULL);

    /* Inicializar ventana */
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Inicializar canvas */
    GtkWidget* drawingArea = gtk_drawing_area_new();
    /* Dimensiones del canvas */
    size_t width = CELL_SIZE * the_layout -> width;
    size_t height = CELL_SIZE * the_layout -> height;
    gtk_widget_set_size_request(drawingArea, width, height);
    /* Ligar eventos */
    g_signal_connect(drawingArea, "draw", G_CALLBACK(draw), NULL);

    /* Meter canvas a la ventana */
    gtk_container_add(GTK_CONTAINER(window), drawingArea);

    /* Mostrar todo */
    gtk_widget_show(drawingArea);
    gtk_widget_show(window);

    /* Cuantas decisiones debemos tomar */
    decision_count = 0;
    for(int i = 0; i < the_layout -> zone_count; i++)
    {
        decision_count += the_layout -> zones[i] -> building_count / 2;
    }
    /* El stack de decisiones del actualizador*/
    decisions = malloc(sizeof(struct choice) * decision_count);
    decision_count = 0;

    /* Registramos la funcion actualizadora */
    gdk_threads_add_timeout(refreshInterval, update, drawingArea);

    /* Comenzamos la ejecucion de GTK */
    gtk_main();

    if(pdf)
    {
        cairo_surface_t* surface;
        cairo_t *cr;

        surface = cairo_pdf_surface_create ("watcher.pdf",width, height);
        cr = cairo_create(surface);

        /* Dibuja el estado actual */
        draw(NULL, cr, NULL);

        cairo_surface_destroy(surface);
        cairo_destroy(cr);
    }
}

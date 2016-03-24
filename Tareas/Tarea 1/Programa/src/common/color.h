#ifndef T1_LIB_COLOR_H
#define T1_LIB_COLOR_H

/* Especifica un color */
enum color {none, red, orange, yellow, green, teal, blue, purple};
typedef enum color Color;

/** Entrega un color aleatorio */
Color color_random();

#endif /* end of include guard: T1_LIB_COLOR_H */

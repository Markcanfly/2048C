#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

typedef struct tabla {
    int **row;
    int *column;
    int size_x;
    int size_y;
} tabla;

*tabla create (int size_x, int size_z);

void swipe(tabla *tabla, char direction);

void free_tabla(tabla to_free);

void print_tabla(tabla to_print);

// add SDL render call

#endif // GAME_H_INCLUDED

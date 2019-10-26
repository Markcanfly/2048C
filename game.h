#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

typedef struct tabla {
    int **dynarr;
    int size_x;
    int size_y;
} tabla;

typedef struct coord {
    int x;
    int y;
} coord;

tabla *create_tabla(int size_x, int size_y, int start_tiles);

void swipe(tabla *tabla, char direction);

void free_tabla(tabla *to_free);

void print_tabla(tabla *to_print);

void add_random(tabla *to_add);

int my_rand(int from, int to);

// add SDL render call

#endif // GAME_H_INCLUDED

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <SDL2/SDL.h>

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

void free_tabla(tabla *to_free);

void print_tabla(tabla *to_print);

void draw_tiles(SDL_Renderer *renderer, tabla *to_draw, int x0, int y0, int x1, int y1);

#endif // GAME_H_INCLUDED

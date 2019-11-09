#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "style.h"

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

void draw_tiles(const struct render_params render_data, tabla *to_draw);

bool lost(tabla *to_check);

#endif // GAME_H_INCLUDED

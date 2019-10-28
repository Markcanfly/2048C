#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "push_tiles.h"

tabla *create_tabla(int size_x, int size_y, int start_tiles) {
    int **nums = (int **) malloc(size_y * sizeof(int));
    nums[0] = (int *) calloc(size_x * size_y, sizeof(int)); // filled with 0s
    for (int y = 1; y < size_y; y++) {
        nums[y] = nums[0] + y*size_x;
    }

    tabla* new_tabla = malloc(sizeof(tabla));
    new_tabla -> dynarr = nums;
    new_tabla -> size_x = size_x;
    new_tabla -> size_y = size_y;

    for (int i = 0; i < start_tiles; i++) {
        add_random(new_tabla);
    }

    return new_tabla;
}

void free_tabla(tabla *to_free) {
    free(to_free -> dynarr[0]);
    free(to_free -> dynarr);
    free(to_free);
}

void print_tabla(tabla *to_print) {
    int x = to_print -> size_x;
    int y = to_print -> size_y;

    for (int y_i = 0; y_i < y; y_i++) {
        for (int x_i = 0; x_i < x; x_i++) {
            printf("%d ", to_print -> dynarr[y_i][x_i]);
        }
        printf("\n");
    }
}

void draw_tiles(SDL_Renderer *renderer, tabla *to_draw, int x0, int y0, int x1, int y1) {
    int size_x = to_draw -> size_x;
    int size_y = to_draw -> size_y;
    int **fields = to_draw -> dynarr;

    const int width = x1 - x0;
    const int height = y1 - y0;

    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {
            if (fields[y][x] != 0) {
                boxColor(renderer,
                         x0 + (width / size_x) * x,
                         y0 + (height / size_y) * y,
                         x0 + (height / size_x) * (x + 1),
                         y0 + (height / size_y) * (y + 1),
                         0xEEE4DAFF);
            }
        }
    }




}

// TODO add renderer
// TODO add gamestate-checker

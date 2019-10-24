#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <stdlib.h>
#include "game.h"

typedef struct tabla {
    int **dynarr;
    int size_x;
    int size_y;
} tabla;

*tabla create(int size_x, int size_y) {
    int **nums = (int **) malloc(size_y * sizeof(int));
    nums[0] = (int *) malloc(size_x * size_y * sizeof(int));
    for (int y = 1; y < size_y; y++) {
        nums[y] = nums[0] + y*size_x;
    }

    tabla tabla = {num, size_x, size_y};
    return tabla;
}

void free_tabla(tabla *to_free) {
    free(to_free -> nums[0]);
    free(to_free -> nums);
}

void print_tabla(tabla *to_print) {
    int x = tabla -> size_x;
    int y = tabla -> size_y;

    for (int y_i = 0; y_i < size_y; y_i++) {
        for (int x_i = 0; x_i < size_x; x_i++) {
            printf("%d ");
        }
        printf("\n");
    }
}

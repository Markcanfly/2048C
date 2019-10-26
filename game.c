#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "game.h"

tabla create_tabla(int size_x, int size_y) {
    int **nums = (int **) malloc(size_y * sizeof(int));
    nums[0] = (int *) calloc(size_x * size_y, sizeof(int)); // filled with 0s
    for (int y = 1; y < size_y; y++) {
        nums[y] = nums[0] + y*size_x;
    }

    tabla tabla = {nums, size_x, size_y};
    return tabla;
}

void free_tabla(tabla *to_free) {
    free(to_free -> dynarr[0]);
    free(to_free -> dynarr);
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

void swipe(tabla *tabla, char direction){
    switch (direction) {
        // Set respective loop parameters
        int size_x = tabla -> size_x;
        int size_y = tabla -> size_y;
        int **fields = tabla -> dynarr;

        case 'U':
            break;
        case 'D':
            break;
        case 'L':
            for (int sor = 0; sor < size_y; sor++) {
                    // check each row
                int i = 1; // Don't even check 0
                while (i < size_x) {
                    if (fields[sor][i] == 0) {
                        // if at wall or is empty (0), skip
                        i++;
                    } else if (fields[sor][i-1] == 0) {
                        // Leftmost is 0
                        fields[sor][i-1] = fields[sor][i];
                        fields[sor][i] = 0;
                        // don't increment i => check again
                    } else if (fields[sor][i-1] == fields[sor][i]) {
                        // Leftmost is the same num => add
                        fields[sor][i-1] += fields[sor][i];
                        fields[sor][i] = 0;
                        // don't increment i => check again
                    }

                }
            }
            break;
        case 'R':
            for (int sor = 0; sor < size_y; sor++) {
                    // check each row
                int i = size_x - 2; // Don't even check the last one
                while (i > 0) {
                    if (fields[sor][i] == 0) {
                        // if at wall or is empty (0), skip
                        i++;
                    } else if (fields[sor][i-1] == 0) {
                        // Leftmost is 0
                        fields[sor][i-1] = fields[sor][i];
                        fields[sor][i] = 0;
                        // don't increment i => check again
                    } else if (fields[sor][i-1] == fields[sor][i]) {
                        // Leftmost is the same num => add
                        fields[sor][i-1] += fields[sor][i];
                        fields[sor][i] = 0;
                        // don't increment i => check again
                    }
                }
            }
            break;
        default:
            // throw error
            break;
    }
    // TODO: Add random block adder
}

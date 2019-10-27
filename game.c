#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
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

void swipe(tabla *to_swipe, char direction){
    int size_x = to_swipe -> size_x;
    int size_y = to_swipe -> size_y;
    int **fields = to_swipe -> dynarr;

    switch (direction) {
        // Set respective loop parameters
        case 'U':
            for (int oszlop = 0; oszlop < size_x; oszlop++) {
                int i = 1; // Don't check the first
                while (i >= 0 && i <= size_y-1) {
                    int *num = &fields[i][oszlop];

                    if (*num != 0) {
                        int *near_num = &fields[i-1][oszlop];
                        if (i == 0 || *num == 0) {
                            i++;
                        } else if (*near_num == 0) {
                            *near_num = *num;
                            *num = 0;
                            i--;
                            // Decrement, check last num
                        } else if (*near_num == *num) {
                            *near_num += *num;
                            *num = 0;
                            i++;
                        } else {
                            // Different numbers => skip
                            i++;
                        }
                    } else {
                        // Skip if num is 0
                        i++;
                    }
                }
            }
            break;
        case 'D':
            for (int oszlop = 0; oszlop < size_x; oszlop++) {
                int i = size_y - 2; // Don't check the first
                while (i >= 0 && i <= size_y-1) {
                    int *num = &fields[i][oszlop];

                    if (*num != 0) {
                        int *near_num = &fields[i+1][oszlop];
                        if (i == 0) {
                            i--;
                        } else if (*near_num == 0) {
                            *near_num = *num;
                            *num = 0;
                            i++;
                            // Decrement, check last num
                        } else if (*near_num == *num) {
                            *near_num += *num;
                            *num = 0;
                            i--;
                        } else {
                            // Different numbers => skip
                            i--;
                        }
                    } else {
                        // Skip if num is 0
                        i--;
                    }
                }
            }
            break;
        case 'L':
            for (int sor = 0; sor < size_y; sor++) {
                int i = 1; // Don't check the first
                while (i >= 0 && i <= size_x-1) {
                    int *num = &fields[sor][i];

                    if (*num != 0) {
                        int *near_num = &fields[sor][i-1];
                        if (i == 0 || *num == 0) {
                            // At wall, or is empty => skip
                            i++;
                        } else if (*near_num == 0) {
                            // Is 0, just push
                            *near_num = *num;
                            *num = 0;
                            i--;
                        } else if (*near_num == *num) {
                            // The same number => add
                            *near_num += *num;
                            *num = 0;
                            i++;
                        } else {
                            // Different numbers => skip
                            i++;
                        }
                    } else {
                        // Skip if num is 0
                        i++;
                    }
                }
            }
            break;
        case 'R':
            for (int sor = 0; sor < size_y; sor++) {
                int i = size_x - 2; // Don't check the last one
                while (i >= 0 && i <= size_x-1) {
                    int *num = &fields[sor][i];

                    if (*num != 0) {
                        int *near_num = &fields[sor][i+1];
                        if (i == 0 || *num == 0) {
                            // At wall, or is empty => skip
                            i--;
                        } else if (*near_num == 0) {
                            // Is 0, just push
                            *near_num = *num;
                            *num = 0;
                            i++;
                        } else if (*near_num == *num) {
                            // The same number => add
                            *near_num += *num;
                            *num = 0;
                            i--;
                        } else {
                            // Different numbers => skip
                            i--;
                        }
                    } else {
                        // Skip if num is 0
                        i--;
                    }
                }
            }
            break;
        default:
            // throw error
            break;
    }

    add_random(to_swipe);
}

// TODO add renderer
// TODO add gamestate-checker

#include "filehandler.h"
#include "game.h"

tabla *load_save() {
    /*
    Expects a tabla file in the following format:

    Score
    SIZE_X
    SIZE_Y

    `SIZE_Y` Rows and `SIZE_X` columns of numbers, separated with a space
    */


    FILE *save_file = fopen("mentes/tabla.txt","r");

    int score, size_x, size_y;
    fscanf(save_file,"%d", &score);
    fscanf(save_file, "%d", &size_x);
    fscanf(save_file, "%d", &size_y);

    // Create new tabla obj

    int **nums = (int **) malloc(size_y * sizeof(int));
    nums[0] = (int *) calloc(size_x * size_y, sizeof(int)); // filled with 0s
    for (int y = 1; y < size_y; y++) {
        nums[y] = nums[0] + y*size_x;
    }

    for (int y = 0; y < size_x; y++) {
        for (int x = 0; x < size_x; x++) {
            fscanf(save_file, "%d", &nums[y][x]);
        }
    }

    tabla* new_tabla = malloc(sizeof(tabla));
    new_tabla -> dynarr = nums;
    new_tabla -> size_x = size_x;
    new_tabla -> size_y = size_y;

    return new_tabla;
}

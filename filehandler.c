#include <string.h>
#include "filehandler.h"
#include "game.h"
#include "debugmalloc.h"
#include "highscores.h"
#include <stdio.h>

/**
* \brief Load the saved gamestate from a static location
* Load the saved gamestate from *%workingdir%/mentes/tabla.txt*
* Expects the following format:
* - Name
* - Score
* - SIZE_X
* - SIZE_Y
* - `SIZE_Y` Rows and `SIZE_X` columns of numbers, separated with a space
*/
tabla *load_save() {

    FILE *save_file = fopen("mentes/tabla.txt", "r");

    if (save_file == NULL) {
        printf("Error! Couldn't open save file for reading.\n");
        return create_tabla("Default", 4, 4, 3);
    }
    char name[51];
    int score, size_x, size_y;
    fgets(name, 51, save_file);
    fscanf(save_file,"%d", &score);
    fscanf(save_file, "%d", &size_x);
    fscanf(save_file, "%d", &size_y);

    // Create new tabla obj

    int **nums = (int **) malloc(size_y * sizeof(int));
    nums[0] = (int *) calloc(size_x * size_y, sizeof(int)); // filled with 0s
    for (int y = 1; y < size_y; y++) {
        nums[y] = nums[0] + y*size_x;
    }

    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {
            fscanf(save_file, "%d", &nums[y][x]);
        }
    }
    fclose(save_file);

    tabla* new_tabla = malloc(sizeof(tabla));

    strcpy(new_tabla -> name, name);
    new_tabla -> score = score;
    new_tabla -> dynarr = nums;
    new_tabla -> size_x = size_x;
    new_tabla -> size_y = size_y;

    return new_tabla;
}

/**
* \brief Store a save of the current gamestate to a static location
* Stores the save at *%workingdir%/mentes/tabla.txt*
* \param to_store pointer to a tabla object
*/
void store_save(const tabla *to_store) {
    /*
    Takes a tabla object and writes its data to file.
    */
    FILE *save_file = fopen("mentes/tabla.txt", "w");

    if (save_file == NULL) {
        printf("Error! Couldn't open save file for writing.");
        return;
    }


    // Write out metadata
    fprintf(save_file, "%s\n%d\n%d\n%d\n", to_store -> name, to_store -> score, to_store -> size_x, to_store -> size_y);
    // Write gamestate to file

    for (int y = 0; y < to_store -> size_y; y++) {
        fprintf(save_file, "%d", to_store -> dynarr[y][0]); // First elem
        for (int x = 1; x < to_store -> size_x; x++) {
            fprintf(save_file, " %d", to_store -> dynarr[y][x]);
        }
        fprintf(save_file, "\n");
    }

    fclose(save_file);

}

/**
* \brief Load the saved highscores from a static location
* Load the saved highscores from *%workingdir%/mentes/highscores.txt*
* Expects the following format on each line:
* {score}-{field_size}-{name}
*/
HS_Node *load_highscores() {
    FILE *hs_file = fopen("mentes/highscores.txt", "r");

    if (hs_file == NULL) {
        printf("Error! Couldn't open highscore file for reading.\n");
        return NULL;
    }

    char line[255]; //254 character long buffer
    int score, field_size;
    char *name;
    HS_Node *first = NULL;
    while (fgets(line, 255, hs_file) != NULL) {
        // Get data from each line
        score = atoi(strtok(line, "-"));
        field_size = atoi(strtok(NULL, "-"));
        name = strtok(strtok(NULL, "-"), "\n"); // Tokenize then strip newline
        if (name != NULL && field_size != 0)
            add_highscore(&first, name, field_size, score);
    }

    return first;
}

/**
* \brief Store the saved highscores to a static location
* Write the saved highscores to *%workingdir%/mentes/highscores.txt*
* Uses the following format on each line:
* {score}-{field_size}-{name}
*/
void store_highscores(HS_Node *to_store) {
    FILE *hs_file = fopen("mentes/highscores.txt", "w");

    if (hs_file == NULL) {
        printf("Error! Couldn't open highscore file for writing.\n");
        return NULL;
    }

    for (HS_Node *c = to_store; c != NULL; c = c -> next) {
        fprintf(hs_file, "%d-%d-%s\n", c -> score, c -> field_size, c -> name);
    }

    fclose(hs_file);

}

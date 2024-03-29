#include <string.h>
#include "filehandler.h"
#include "game.h"
#include "debugmalloc.h"
#include "highscores.h"
#include <stdio.h>

/**
* \brief Load the saved gamestate from a static location
* Load the saved gamestate from *%workingdir%/data/mentes.txt*
* Expects the following format:
* - Name
* - Score
* - SIZE_X
* - SIZE_Y
* - `SIZE_Y` Rows and `SIZE_X` columns of numbers, separated with a space
* - Previous score (for undo)
* - `SIZE_Y` Rows and `SIZE_X` columns of numbers, separated with a space or '-' if nonexistent (previous state for undo)
*/
tabla *load_save() {

    FILE *save_file = fopen("data/mentes.txt", "r");

    if (save_file == NULL) {
        printf("Error! Couldn't open save file for reading.\n");
        return create_tabla("Default", 4, 4, 3);
    }
    char name[51];
    int score, previous_score, size_x, size_y;
    fgets(name, 51, save_file);
    strtok(name, "\n");
    fscanf(save_file,"%d", &score);
    fscanf(save_file, "%d", &size_x);
    fscanf(save_file, "%d", &size_y);

    /* Load field values */

    // Create new dynamic array for the field

    int **nums = (int **) malloc(size_y * sizeof(int));
    if (nums == NULL)
        return create_tabla("Default", 4, 4, 3);
    nums[0] = (int *) malloc(size_x * size_y * sizeof(int)); // filled with 0s
    if (nums[0] == NULL)
        return create_tabla("Default", 4, 4, 3);
    for (int y = 1; y < size_y; y++) {
        nums[y] = nums[0] + y*size_x;
    }
    // Assign values
    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {
            fscanf(save_file, "%d", &nums[y][x]);
        }
    }

    /* Load previous field if present */

    // Load previous score
    fscanf(save_file, "%d", &previous_score);
    int **prev_nums;

    if (previous_score > 0) { /* Read previous state */
        // Create new dynamic array for the field

        prev_nums = (int **) malloc(size_y * sizeof(int));
        if (prev_nums == NULL)
            return create_tabla("Default", 4, 4, 3);

        prev_nums[0] = (int *) malloc(size_x * size_y * sizeof(int));
        if (prev_nums[0] == NULL)
            return create_tabla("Default", 4, 4, 3);

        for (int y = 1; y < size_y; y++) {
            prev_nums[y] = prev_nums[0] + y*size_x;
        }
        // Assign values
        for (int y = 0; y < size_y; y++) {
            for (int x = 0; x < size_x; x++) {
                fscanf(save_file, "%d", &prev_nums[y][x]);
            }
        }
    } else { /* No previous state present, don't even read */
        prev_nums = NULL;
    }

    fclose(save_file);

    // Create new tabla obj
    tabla* new_tabla = malloc(sizeof(tabla));

    if (new_tabla == NULL)
        return create_tabla("Default", 4, 4, 3);

    strcpy(new_tabla -> name, name);
    new_tabla -> score = score;
    new_tabla -> previous_score = previous_score;
    new_tabla -> current_field = nums;
    new_tabla -> previous_field = prev_nums;
    new_tabla -> size_x = size_x;
    new_tabla -> size_y = size_y;

    return new_tabla;
}

/**
* \brief Store a save of the current gamestate to a static location
* Stores the save at *%workingdir%/data/mentes.txt*
* \param to_store pointer to a tabla object
*/
void store_save(const tabla *to_store) {
    /*
    Takes a tabla object and writes its data to file.
    */
    FILE *save_file = fopen("data/mentes.txt", "w");

    if (save_file == NULL) {
        printf("Error! Couldn't open save file for writing.");
        return;
    }

    // Write out generic metadata
    fprintf(save_file, "%s\n%d\n%d\n%d\n", to_store -> name, to_store -> score, to_store -> size_x, to_store -> size_y);

    // Write gamestate to file
    for (int y = 0; y < to_store -> size_y; y++) {
        fprintf(save_file, "%d", to_store -> current_field[y][0]); // First elem
        for (int x = 1; x < to_store -> size_x; x++) {
            fprintf(save_file, " %d", to_store -> current_field[y][x]);
        }
        fprintf(save_file, "\n");
    }

    // Write previous score
    fprintf(save_file, "%d\n", to_store -> previous_score);

    // Write previous gamestate if such exists, otherwise a '-'
    if (to_store -> previous_field != NULL) {
        for (int y = 0; y < to_store -> size_y; y++) {
            fprintf(save_file, "%d", to_store -> previous_field[y][0]); // First elem
            for (int x = 1; x < to_store -> size_x; x++) {
                fprintf(save_file, " %d", to_store -> previous_field[y][x]);
            }
            fprintf(save_file, "\n");
        }
    } else {
        fprintf(save_file, "-\n");
    }

    fclose(save_file);

}

/**
* \brief Load the saved highscores from a static location
* Load the saved highscores from *%workingdir%/data/toplista.txt*
* Expects the following format on each line:
* {name}-{field_size}-{score}
*/
HS_Node *load_highscores() {
    FILE *hs_file = fopen("data/toplista.txt", "r");

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
        name = strtok(line, "-");
        field_size = atoi(strtok(NULL, "-"));
        score = atoi(strtok(strtok(NULL, "-"), "\n"));

         // Tokenize then strip newline
        if (name != NULL && field_size != 0)
            add_checked_highscore(&first, name, field_size, score);
    }

    return first;
}

/**
* \brief Store the saved highscores to a static location
* Write the saved highscores to *%workingdir%/data/toplista.txt*
* Uses the following format on each line:
* {name}-{field_size}-{score}
*/
void store_highscores(HS_Node *to_store) {
    FILE *hs_file = fopen("data/toplista.txt", "w");

    if (hs_file == NULL) {
        printf("Error! Couldn't open highscore file for writing.\n");
        return NULL;
    }

    for (HS_Node *c = to_store; c != NULL; c = c -> next) {
        fprintf(hs_file, "%s-%d-%d\n", c -> name, c -> field_size, c -> score);
    }

    fclose(hs_file);

}

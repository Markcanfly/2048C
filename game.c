#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "style.h"
#include "game.h"
#include "push_tiles.h"
#include "debugmalloc.h"
#include "filehandler.h"

/**
* \brief Pseudo-constructor for a tabla object
* \param name The name of the player
* \param size_x The horizontal size of the tabla
* \param size_y The vertical size of the tabla
* \param start_tiles The number of random start tiles to add to the field.
* Dynamically create a clean new tabla object and return a pointer to it.
* Uses the parameter start_tiles to determine how many initial random tiles to add.
* \return new tabla pointer
*/
tabla *create_tabla(char *name, int size_x, int size_y, int start_tiles) {
    int **nums = (int **) malloc(size_y * sizeof(int));
    if (nums == NULL)
        return NULL;

    nums[0] = (int *) calloc(size_x * size_y, sizeof(int)); // filled with 0s
    if (nums[0] == NULL)
        return NULL;

    for (int y = 1; y < size_y; y++) {
        nums[y] = nums[0] + y*size_x;
    }

    tabla* new_tabla = malloc(sizeof(tabla));
    if (new_tabla == NULL)
        return NULL;

    strcpy(new_tabla -> name, name);
    new_tabla -> score = 0;
    new_tabla -> previous_score = 0;
    new_tabla -> current_field = nums;
    new_tabla -> previous_field = NULL;
    new_tabla -> size_x = size_x;
    new_tabla -> size_y = size_y;

    for (int i = 0; i < start_tiles; i++) {
        add_random(new_tabla);
    }

    return new_tabla;
}

/**
* \brief Make copy of the tabla
* \param
* \return
*/
tabla *copy_tabla(tabla *to_copy) {
    // Allocate memory for the new array
    int **nums = (int **) malloc((to_copy -> size_y) * sizeof(int));
    if (nums == NULL)
        return NULL;

    nums[0] = (int *) malloc(sizeof(int) * (to_copy -> size_x) * (to_copy -> size_y));
    if (nums[0] == NULL)
        return NULL;

    for (int y = 1; y < to_copy -> size_y; y++) {
        nums[y] = nums[0] + y * to_copy -> size_x;
    }

    // Copy values to new array
    for (int y = 0; y < to_copy -> size_y; y++) {
        for (int x = 0; x < to_copy -> size_x; x++) {
            nums[y][x] = to_copy -> current_field[y][x];
        }
    }

    tabla *new_tabla = malloc(sizeof(tabla));
    if (new_tabla == NULL)
        return NULL;

    // Copy values over
    strcpy(new_tabla -> name, to_copy -> name);
    new_tabla -> score = to_copy -> score;
    new_tabla -> current_field = nums;
    new_tabla -> previous_field = NULL;
    new_tabla -> previous_score = 0;
    new_tabla -> size_x = to_copy -> size_x;
    new_tabla -> size_y = to_copy -> size_y;

    return new_tabla;
}

/**
* \brief Free a tabla object
* \param to_free pointer to a tabla object
*/
void free_tabla(tabla *to_free) {
    free(to_free -> current_field[0]);
    free(to_free -> current_field);
    if (to_free -> previous_field != NULL) {
        free(to_free -> previous_field[0]);
        free(to_free -> previous_field);
    }
    free(to_free);
}

/**
* \brief Free a tabla object, except for the current state
* Free a tabla object, excluding the current_state 2D array, which can be reused
* Primarily used for the undo method, the state storing of which requires
* the previous state to be reachable, and to avoid having to reallocate an identical array
* we can reuse it.
*/
void free_tabla_ex_field(tabla *to_free) {
    if (to_free -> previous_field != NULL) {
        free(to_free -> previous_field[0]);
        free(to_free -> previous_field);
    }
    free(to_free);
}

/**
* \brief Print tabla object to console
* \param to_print pointer to a tabla object
* Takes a tabla object and prints a representation to stdout,
* including the current score and numbers contained.
* For debugging purposes only.
*/
void print_tabla(tabla *to_print) {
    int x = to_print -> size_x;
    int y = to_print -> size_y;

    printf("Score: %d\n", to_print -> score);

    for (int y_i = 0; y_i < y; y_i++) {
        for (int x_i = 0; x_i < x; x_i++) {
            printf("%d ", to_print -> current_field[y_i][x_i]);
        }
        printf("\n");
    }
}

/**
* \brief Game move handler
* \param to_move pointer to a tabla object
* \param dir The direction to move, ['L', 'R', 'U', 'D']
*/
int game_move(tabla *to_move, HS_Node **hs_node, char dir) {

    // Create temporary save of current state
    tabla *temp_fieldcopy = copy_tabla(to_move);

    int move_state; // -1: game won | 0: valid move | 1: invalid move
    switch (dir) {
        case 'L':
            move_state = push_left(to_move);
            break;
        case 'R':
            move_state = push_right(to_move);
            break;
        case 'U':
            move_state = push_up(to_move);
            break;
        case 'D':
            move_state = push_down(to_move);
            break;
    }

    switch (move_state) {
        case -1: /* Game won */
            add_checked_highscore(hs_node, to_move -> name, to_move -> size_x, to_move -> score);
            store_save(to_move);
            return 1;
            break;

        case 0: /* Valid move */
            add_random(to_move);
            // Save state
            store_save(to_move);
            add_checked_highscore(hs_node, to_move -> name, to_move -> size_x, to_move -> score);

            /* SAVE THE PREVIOUS STATE */
            // Save score
            to_move -> previous_score = temp_fieldcopy -> score;

            // Free the previous 2D array
            if (to_move -> previous_field != NULL) {
                free(to_move -> previous_field[0]);
                free(to_move -> previous_field);
            }
            // Set the pointers
            to_move -> previous_field = temp_fieldcopy -> current_field;
            to_move -> previous_field[0] = temp_fieldcopy -> current_field[0];

            // Free the temporary variable
            free_tabla_ex_field(temp_fieldcopy);

            break;
        case 1: /* Invalid move */
            free_tabla(temp_fieldcopy);
            break;
    }

    if (lost(to_move))
        return -1;

    // If we got here, all is well, return 0
    return 0;
}

/**
* \brief Perform an undo on the object
* In case there's an available previous state, replace the current state with that
*/
void game_undo(tabla *to_undo) {
    if (to_undo -> previous_field != NULL) {
        /* Reset score */
        to_undo -> score = to_undo -> previous_score;
        to_undo -> previous_score = 0; // Flag for the save
        /* Free current state */
        free(to_undo -> current_field[0]);
        free(to_undo -> current_field);
        /* Replace current state with previous */
        to_undo -> current_field = to_undo -> previous_field;
        to_undo -> current_field[0] = to_undo -> previous_field[0];
        /* Set previous state to unavailable */
        to_undo -> previous_field = NULL;
    } else {
        printf("Undo unavailable.\n");
    }
}

/**
* \brief Draws all game elements
* \param render_data rendering hyperparameters
* \param to_draw pointer to a tabla object
* This function separates the renderer
* into the playing field, which is the bottom square
* (the height of which is the width of the renderer)
* and a rectangle above which fills the remaining space.
* Then uses the precise functions to draw the score,
* and the playing field respectively.
*/
void draw_game(const struct render_params render_data, tabla *to_draw) {
    /**
    * \brief Draws all game elements
    * \param render_data rendering hyperparameters
    * \param to_draw pointer to a tabla object
    * This function separates the renderer
    * into the playing field, which is the bottom square
    * (the height of which is the width of the renderer)
    * and a rectangle above which fills the remaining space.
    * Then uses the precise functions to draw the score,
    * and the playing field respectively.
    */
    struct render_params game_rp = render_data;
    game_rp.y0 = game_rp.y1 - (game_rp.x1 - game_rp.x0); // height=width and fill bottom
    struct render_params controls_rp = render_data;
    controls_rp.y1 = game_rp.y0;

    draw_game_controls(controls_rp, to_draw);
    draw_tiles(game_rp, to_draw);

}

/**
* \brief Draw score to render loc
* \param render_data rendering hyperparameters
* \param to_draw pointer to a tabla object
* Draws the current score to the specified render location
*/
void draw_game_controls(const struct render_params render_data, tabla *to_draw) {

    char score_char[33]; // large buffer to handle large score

    const SDL_Color text_color = {119, 110, 101, 255};
    const Uint32 background_color = 0xEEE4DAFF;

    boxColor(render_data.renderer, render_data.x0, render_data.y0, render_data.x1, render_data.y1, background_color);

    itoa(to_draw -> score, score_char, 10);
    draw_text_to_center(render_data.renderer, render_data.x0, render_data.y0, render_data.x1, render_data.y1, score_char, render_data.font, text_color);

}

/**
* \brief Draw the tiles into a square frame
* \param render_data rendering hyperparameters
* \param to_draw pointer to a tabla object
* Takes a tabla object and draws each tile into a frame
* specified by the render_params struct.
*/
void draw_tiles(const struct render_params render_data, tabla *to_draw) {

    int size_x = to_draw -> size_x;
    int size_y = to_draw -> size_y;
    int **fields = to_draw -> current_field;

    const int width = render_data.x1 - render_data.x0;
    const int height = render_data.y1 - render_data.y0;

    char num_char[5]; // Size 5 buffer (max val is 8192)

    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {
            if (fields[y][x] != 0) {
                // Create frame coords
                int x0_ = render_data.x0 + (width / size_x) * x;
                int y0_ = render_data.y0 + (height / size_y) * y;
                int x1_ = render_data.x0 + (height / size_x) * (x + 1);
                int y1_ = render_data.y0 + (height / size_y) * (y + 1);

                // Get colors and create box
                int number = fields[y][x];

                rect_style this_tile = getTile(number);

                roundedBoxColor(render_data.renderer, x0_, y0_, x1_, y1_, 10, this_tile.backgroundColor);

                itoa(number, num_char, 10); // Int to char array

                // Text rendering
                draw_text_to_center(render_data.renderer, x0_, y0_, x1_, y1_, num_char, render_data.font, this_tile.textColor);
            }
        }
    }

}

/**
* \brief Check if game is lost
* \param to_check pointer to a tabla object
* Checks the whole field if there are any possible valid moves left.
*/
bool lost(tabla* to_check) {

    int size_x = to_check -> size_x;
    int size_y = to_check -> size_y;
    int **fields = to_check -> current_field;

    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {
            int* current = &fields[y][x];
            if (*current == 0)
                return false;

            if (y != size_y - 1) {
                // Not checking last row
                int* next_v = &fields[y+1][x];
                if (*next_v == 0)
                    return false;
                if (*current == *next_v)
                    return false;
            }

            if (x != size_x - 1) {
                // Not checking last column
                int *next_h = &fields[y][x+1];
                if (*next_h == 0)
                    return false;
                if (*next_h == *current)
                    return false;
            }
        }
    }

    return true;

}

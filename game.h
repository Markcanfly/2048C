#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "highscores.h"
#include "style.h"

/**
* \brief The structure used to store a gamestate
*/
typedef struct tabla {
    char name[51];
    int score;
    int **dynarr;
    int size_x;
    int size_y;
} tabla;

typedef struct coord {
    int x;
    int y;
} coord;

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
tabla *create_tabla(char *name, int size_x, int size_y, int start_tiles);

/**
* \brief Free a tabla object
* \param to_free pointer to a tabla object
*/
void free_tabla(tabla *to_free);

/**
* \brief Print tabla object to console
* \param to_print pointer to a tabla object
* Takes a tabla object and prints a representation to stdout,
* including the current score and numbers contained.
* For debugging purposes only.
*/
void print_tabla(tabla *to_print);

/**
* \brief Game move handler
* \param to_move pointer to a tabla object
* \param dir The direction to move, ['L', 'R', 'U', 'D']
*/
int move_game(tabla *to_move, HS_Node **hs_node, char dir);

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
void draw_game(const struct render_params render_data, tabla *to_draw);

/**
* \brief Draw score to render loc
* \param render_data rendering hyperparameters
* \param to_draw pointer to a tabla object
* Draws the current score to the specified render location
*/
void draw_game_controls(const struct render_params render_data, tabla *to_draw);

/**
* \brief Draw the tiles into a square frame
* \param render_data rendering hyperparameters
* \param to_draw pointer to a tabla object
* Takes a tabla object and draws each tile into a frame
* specified by the render_params struct.
*/
void draw_tiles(const struct render_params render_data, tabla *to_draw);

/**
* \brief Check if game is lost
* \param to_check pointer to a tabla object
* Checks the whole field if there are any possible valid moves left.
*/
bool lost(tabla *to_check);

#endif // GAME_H_INCLUDED

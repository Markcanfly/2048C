#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "style.h"
#include "game.h"
#include "push_tiles.h"


/**
* \brief Pseudo-constructor for a tabla object
* \param x and y sizes of tabla, number of random start tiles
* Dynamically create a clean new tabla object and return a pointer to it.
* Uses the parameter start_tiles to determine how many initial random tiles to add.
* \return new tabla pointer
*/
tabla *create_tabla(int size_x, int size_y, int start_tiles) {
    int **nums = (int **) malloc(size_y * sizeof(int));
    nums[0] = (int *) calloc(size_x * size_y, sizeof(int)); // filled with 0s
    for (int y = 1; y < size_y; y++) {
        nums[y] = nums[0] + y*size_x;
    }

    tabla* new_tabla = malloc(sizeof(tabla));
    new_tabla -> score = 0;
    new_tabla -> dynarr = nums;
    new_tabla -> size_x = size_x;
    new_tabla -> size_y = size_y;

    for (int i = 0; i < start_tiles; i++) {
        add_random(new_tabla);
    }

    return new_tabla;
}

/**
* \brief Free a tabla object
* Takes a pointer to a tabla object and
* frees the inner dynamic 2D array, then the object itself
*/
void free_tabla(tabla *to_free) {
    free(to_free -> dynarr[0]);
    free(to_free -> dynarr);
    free(to_free);
}

/**
* \brief Print tabla object to console
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
            printf("%d ", to_print -> dynarr[y_i][x_i]);
        }
        printf("\n");
    }
}

void draw_game(const struct render_params render_data, tabla *to_draw) {
    /**
    * \brief Draws all game elements
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

void draw_game_controls(const struct render_params render_data, tabla *to_draw) {
    /**
    * \brief Draws the current score to the specified render location
    */
    char score_char[33]; // large buffer to handle large score

    const SDL_Color text_color = {119, 110, 101, 255};
    const Uint32 background_color = 0xEEE4DAFF;

    boxColor(render_data.renderer, render_data.x0, render_data.y0, render_data.x1, render_data.y1, background_color);

    itoa(to_draw -> score, score_char, 10);
    draw_text_to_center(render_data.renderer, render_data.x0, render_data.y0, render_data.x1, render_data.y1, score_char, render_data.font, text_color);

}

void draw_tiles(const struct render_params render_data, tabla *to_draw) {
    /**
    * \brief Takes a tabla object and draws the tiles in it into a square frame.
    * \param render parameters, tabla
    */
    int size_x = to_draw -> size_x;
    int size_y = to_draw -> size_y;
    int **fields = to_draw -> dynarr;

    const int width = render_data.x1 - render_data.x0;
    const int height = render_data.y1 - render_data.y0;

    // Declare variables needed for Text rendering
    SDL_Surface *num_s;
    SDL_Texture *num_t;
    char num_char[5]; // Size 5 buffer (max val is 8192)
    SDL_Rect loc = { 0, 0, 0, 0 };


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

                // Text rendering

                itoa(number, num_char, 10); // Int to char array
                num_s = TTF_RenderUTF8_Blended(render_data.font, num_char, this_tile.textColor);
                num_t = SDL_CreateTextureFromSurface(render_data.renderer, num_s);

                loc.x = (x1_ + x0_) / 2 - (num_s -> w / 2);
                loc.y = (y1_ + y0_) / 2 - (num_s -> h / 2);
                loc.w = num_s -> w;
                loc.h = num_s -> h;

                SDL_RenderCopy(render_data.renderer, num_t, NULL, &loc);
            }
        }
    }

    SDL_FreeSurface(num_s);
    SDL_DestroyTexture(num_t);



}

bool lost(tabla* to_check) {
    int size_x = to_check -> size_x;
    int size_y = to_check -> size_y;
    int **fields = to_check -> dynarr;

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
// TODO add renderer
// TODO add gamestate-checker

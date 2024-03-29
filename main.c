#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "game.h"
#include "push_tiles.h"
#include "menu.h"
#include "filehandler.h"
#include "debugmalloc.h"
#include "highscores.h"


/*  -- TODO list - ordered by priority --
    TODO add headers to highscore display
*/

// Hyperparameters

const int WINSIZE_X = 400;
const int WINSIZE_Y = 500;

void sdl_init(int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("2048", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }

    SDL_RenderClear(renderer);
    *pwindow = window;
    *prenderer = renderer;
}

int main(int argc, char *argv[]) {

    /* Initialize shared variables */

    /* Highscore data */
    HS_Node *hs_first = load_highscores();

    /* Tabla data */
    tabla *uj_tabla = load_save();

    /* SDL data */
    SDL_Window *window;
    SDL_Renderer *renderer;

    sdl_init(WINSIZE_X, WINSIZE_Y, &window, &renderer);

    /* TrueTypeFont data */
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("arial.ttf", 32);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    /* Render data */
    const struct render_params render_data = {
        .renderer = renderer,
        .x0 = 0,
        .y0 = 0,
        .x1 = WINSIZE_X,
        .y1 = WINSIZE_Y,
        font = font
    };

    /* Menu controller data */
    bool quit = false;
    bool quit_game = true;
    bool quit_highscores = true;
    bool quit_play_select = true;
    bool create_newgame = false;

    // ENTER Event Loop Controlled Menu
    while (!quit) {

        // ENTER Event Loop Controlled Selector
        while (!quit_play_select) {

            // Game state controller variable
            int game_status = 0; // 0: nothing | 1: won | -1: lost
            // ENTER Game
            while (!quit_game) {

                // Draw tiles
                draw_game(render_data, uj_tabla);

                // Game event handling
                switch (game_status) {
                    case 1:
                        draw_win_splash(render_data);
                        break;
                    case -1:
                        draw_lose_splash(render_data);
                        quit_game = true;
                        break;
                }

                SDL_RenderPresent(renderer);

                SDL_Event game_event;
                SDL_WaitEvent(&game_event);

                // Get user input
                switch (game_event.type) {
                    case SDL_KEYDOWN:
                        switch (game_event.key.keysym.sym) {
                            case SDLK_LEFT: game_status = game_move(uj_tabla, &hs_first, 'L'); break;
                            case SDLK_RIGHT: game_status = game_move(uj_tabla, &hs_first, 'R'); break;
                            case SDLK_UP: game_status = game_move(uj_tabla, &hs_first, 'U'); break;
                            case SDLK_DOWN: game_status = game_move(uj_tabla, &hs_first, 'D'); break;
                            case SDLK_ESCAPE:
                                quit_game = true;
                                // Show previous after quitting game
                                draw_menu_play(render_data, 0, 0, false);
                                SDL_RenderPresent(renderer);
                                break;
                            case SDLK_SPACE:
                                game_undo(uj_tabla);
                                break;
                        }
                        break;

                    case SDL_QUIT:
                        quit_game = true;
                        quit = true;
                        break;
                }

            }

            // ENTER Play menu
            int choice = -1; // Store the menu option choice here
            choice = handle_menu_interaction(render_data, &quit_play_select, &draw_menu_play);
            switch (choice) {
                case 0:
                    quit_game = false;
                    break;
                case 1: // New game
                    create_newgame = true;
                    break;
                case 2:
                    quit_play_select = true;
                    draw_menu_main(render_data, 0, 0, false);
                    break;
            }

            if (create_newgame) {
                bool successful = handle_menu_newgame_interaction(render_data, &uj_tabla);
                if (successful)
                    quit_game = false;

                create_newgame = false; // avoid looping back here without user choice
            }

            SDL_RenderPresent(renderer);
        }

        // ENTER High Scores screen
        while (!quit_highscores) {
            bool exit = handle_menu_hs_interaction(render_data, &quit_highscores, hs_first);
            if (exit) {
                quit_highscores = true;
                draw_menu_main(render_data, 0, 0, false);
                SDL_RenderPresent(renderer);
                break;
            }
        }

        int choice = -1; // Store the menu option choice here
        choice = handle_menu_interaction(render_data, &quit, &draw_menu_main);

        switch (choice) {
            case -1:
                // Do nothing, no choice made
                break;
            case 0:
                quit_play_select = false;
                draw_menu_play(render_data, 0, 0, false);
                break;
            case 1:
                // High score
                draw_menu_highscores(render_data, 0, 0, false, hs_first);
                quit_highscores = false;
                break;
            case 2:
                quit = true;
                break;
        }

        SDL_RenderPresent(renderer);

    }

    /* EXIT */

    // Save highscores to file
    store_highscores(hs_first);

    // Close files and free dynamically allocated memory
    free_tabla(uj_tabla);
    TTF_CloseFont(font);
    free_highscores(&hs_first);
    SDL_Quit();
    return 0;
}

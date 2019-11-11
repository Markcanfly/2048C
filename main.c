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


/*  -- TODO list - ordered by priority --
    TODO fix pressing back does not render
    TODO highscore table
    TODO move logging, undo
    TODO animations
    TODO ask before overwriting previous save
    TODO Only show 'continue' button if save file exists
*/

// Hyperparameters

const int WINSIZE_X = 400;
const int WINSIZE_Y = 400;

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

    srand(time(NULL));

    tabla *uj_tabla = load_save();
    print_tabla(uj_tabla);


    SDL_Window *window;
    SDL_Renderer *renderer;

    sdl_init(WINSIZE_X, WINSIZE_X, &window, &renderer);

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("arial.ttf", 32);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }


    const struct render_params render_data = {
        .renderer = renderer,
        .x0 = 0,
        .y0 = 0,
        .x1 = WINSIZE_X,
        .y1 = WINSIZE_Y,
        font = font
    };

    bool quit = false;
    bool quit_game = true;
    bool quit_play_select = true;
    while (!quit) {

        while (!quit_play_select) {

            while (!quit_game) {
                SDL_RenderClear(renderer);

                // Game background
                boxColor(renderer, 0, 0, WINSIZE_X, WINSIZE_Y, 0xD2B48CFF);

                // Draw tiles

                draw_tiles(render_data, uj_tabla);
                SDL_RenderPresent(renderer);

                SDL_Event game_event;
                SDL_WaitEvent(&game_event);

                switch (game_event.type) {

                    case SDL_KEYUP:
                        switch (game_event.key.keysym.sym) {
                            case SDLK_LEFT: push_left(uj_tabla); break;
                            case SDLK_RIGHT: push_right(uj_tabla); break;
                            case SDLK_UP: push_up(uj_tabla); break;
                            case SDLK_DOWN: push_down(uj_tabla); break;
                            case SDLK_ESCAPE:
                                quit_game = true;
                                // Show previous after quitting game
                                draw_menu_play(render_data, 0, 0, false);
                                SDL_RenderPresent(renderer);
                                break;
                        }
                        printf("---------\n");
                        print_tabla(uj_tabla);
                        break;

                    case SDL_QUIT:
                        quit_game = true;
                        quit = true;
                        break;
                }
            }

            int choice; // Store the menu option choice here
            choice = handle_menu_interaction(render_data, &quit_play_select, &draw_menu_play);
            switch (choice) {
                case 0:
                    quit_game = false;
                    break;
                case 1: // New game
                    free_tabla(uj_tabla);
                    uj_tabla = create_tabla(4, 4, 3);
                    quit_game = false;
                    break;
                case 2:
                    quit_play_select= true;
                    //  add draw main play here
                    break;
            }
            SDL_RenderPresent(renderer);


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
                break;
            case 2:
                quit = true;
                break;
        }

        SDL_RenderPresent(renderer);

    }

    free_tabla(uj_tabla);
    TTF_CloseFont(font);
    SDL_Quit();
    return 0;
}

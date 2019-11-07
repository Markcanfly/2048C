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


/*  -- TODO list - ordered by priority --
    TODO fix menu items not rendering if cursor is outside of window
    TODO fix menu not rendering until cursor move
    TODO menu
    TODO save game to file
    TODO highscore table
    TODO move logging, undo
    TODO animations
    TODO proper GUI
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

    tabla *uj_tabla = create_tabla(4, 4, 3);
    print_tabla(uj_tabla);


    SDL_Window *window;
    SDL_Renderer *renderer;
    //SDL_SetRenderDrawColor(renderer, 210, 180, 140, 255);

    // Open font
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("arial.ttf", 32);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    sdl_init(WINSIZE_X, WINSIZE_X, &window, &renderer);

    bool quit = false;
    bool quit_game = true;
    while (!quit) {
        while (!quit_game) {
            SDL_RenderClear(renderer);

            // Game background
            boxColor(renderer, 0, 0, WINSIZE_X, WINSIZE_Y, 0xD2B48CFF);

            // Draw tiles

            draw_tiles(renderer, uj_tabla, font, 0, 0, WINSIZE_X, WINSIZE_Y);
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
                        case SDLK_ESCAPE: quit_game = true; break;
                    }
                    printf("---------\n");
                    print_tabla(uj_tabla);
                    break;

                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }

        SDL_Event event;
        SDL_WaitEvent(&event);
        SDL_RenderClear(renderer);

        int choice = -1; // Store the menu option choice here

        switch (event.type) {
            case SDL_MOUSEMOTION:
                choice = draw_menu_main(renderer, font, 0, 0, WINSIZE_X, WINSIZE_Y, event.motion.x, event.motion.y, false);
                break;
            case SDL_MOUSEBUTTONDOWN:
                // Print without storing the choice
                draw_menu_main(renderer, font, 0, 0, WINSIZE_X, WINSIZE_Y, event.button.x, event.button.y, true);
                break;
            case SDL_MOUSEBUTTONUP:
                choice = draw_menu_main(renderer, font, 0, 0, WINSIZE_X, WINSIZE_Y, event.button.x, event.button.y, true);
                switch (choice) {
                    case 0:
                        quit_game = false;
                        break;
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: quit = true; break;
                }
                break;

        }
        SDL_RenderPresent(renderer);

    }

    free_tabla(uj_tabla);
    TTF_CloseFont(font);
    SDL_Quit();
    return 0;
}

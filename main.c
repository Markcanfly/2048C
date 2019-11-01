#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "game.h"
#include "push_tiles.h"


/*  -- TODO list - ordered by priority --
    TODO menu
    TODO save game to file
    TODO highscore table
    TODO move logging, undo
    TODO animations
    TODO proper GUI
    TODO fix merging a tile twice in one push? cannot reproduce, got it on video
*/

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

    sdl_init(400, 400, &window, &renderer);
    draw_tiles(renderer, uj_tabla, font, 0, 0, 400, 400);

    bool quit = false;

    while (!quit) {

        SDL_RenderClear(renderer);

        // Game background
        boxColor(renderer, 0, 0, 400, 400, 0xD2B48CFF);

        // Draw tiles

        draw_tiles(renderer, uj_tabla, font, 0, 0, 400, 400);

        SDL_RenderPresent(renderer);

        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type) {

            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT: push_left(uj_tabla); break;
                    case SDLK_RIGHT: push_right(uj_tabla); break;
                    case SDLK_UP: push_up(uj_tabla); break;
                    case SDLK_DOWN: push_down(uj_tabla); break;
                    case SDLK_ESCAPE: quit = true; break;
                }
                printf("---------\n");
                print_tabla(uj_tabla);
                break;

            case SDL_QUIT:
                quit = true;
                break;
        }

    }
    free_tabla(uj_tabla);
    TTF_CloseFont(font);
    SDL_Quit();
    return 0;
}

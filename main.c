
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <stdlib.h>


/* kulon fuggvenybe, hogy olvashatobb legyen */
void sdl_init(int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("SDL peldaprogram", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
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
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init(350, 200, &window, &renderer);

    bool quit = false;
    bool left = false;
    bool right = false;
    bool rajz = true;
    while (!quit) {
        if (rajz) {
            if (left)
                filledTrigonRGBA(renderer, 50, 100, 150, 50, 150, 150, 0x00, 0xC0, 0x00, 0xFF);
            else
                filledTrigonRGBA(renderer, 50, 100, 150, 50, 150, 150, 0xFF, 0x00, 0x00, 0xFF);
            if (right)
                filledTrigonRGBA(renderer, 300, 100, 200, 50, 200, 150, 0x00, 0xC0, 0x00, 0xFF);
            else
                filledTrigonRGBA(renderer, 300, 100, 200, 50, 200, 150, 0xFF, 0x00, 0x00, 0xFF);
            SDL_RenderPresent(renderer);
            rajz = false;
        }

        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type) {
            /* felhasznaloi esemeny: ilyeneket general az idozito fuggveny */
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT: left = false; rajz = true; break;
                    case SDLK_RIGHT: right = false; rajz = true; break;
                    case SDLK_ESCAPE: quit = true; break;
                }
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT: left = true; rajz = true; break;
                    case SDLK_RIGHT: right = true; rajz = true; break;
                }
                break;

            case SDL_QUIT:
                quit = true;
                break;
        }
    }

    SDL_Quit();
    return 0;
}

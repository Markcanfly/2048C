#ifndef STYLE_H_INCLUDED
#define STYLE_H_INCLUDED

#include <SDL2/SDL.h>

typedef struct tile {
    SDL_Color textColor;
    Uint32 backgroundColor;
}tile;

tile getTile(int tile_num);

#endif // STYLE_H_INCLUDED

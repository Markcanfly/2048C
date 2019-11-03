#ifndef STYLE_H_INCLUDED
#define STYLE_H_INCLUDED

#include <SDL2/SDL.h>

typedef struct rect_style {
    SDL_Color textColor;
    Uint32 backgroundColor;
}rect_style;

struct button_style {
    rect_style inactive;
    rect_style hover;
    rect_style down;
};

rect_style getTile(int tile_num);

#endif // STYLE_H_INCLUDED

#ifndef STYLE_H_INCLUDED
#define STYLE_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct rect_style {
    SDL_Color textColor;
    Uint32 backgroundColor;
}rect_style;

struct render_params {
    SDL_Renderer *renderer;
    int x0, y0, x1, y1;
    TTF_Font *font;
};

struct button_style {
    rect_style inactive;
    rect_style hover;
    rect_style down;
};

rect_style getTile(int tile_num);

#endif // STYLE_H_INCLUDED

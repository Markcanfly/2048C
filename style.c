#include "style.h"
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/**
* \brief Global array of vals for styling of the tiles.
* The index in the array is the log2(n) of the tile num,
* except when it's >2048, in which case it's 0 (black).
*/
const rect_style tiles[12] = {
    {{0, 0, 0, 255}, 0xFFFFFFFF}, // super
    {{119, 110, 101, 255}, 0xEEE4DAFF}, // 2
    {{119, 110, 101, 255}, 0xEDE0C8FF}, // 4
    {{249, 246, 242, 255}, 0xF2B179FF}, // 8
    {{249, 246, 242, 255}, 0xF59563FF}, // 16
    {{249, 246, 242, 255}, 0xF67C5FFF}, // 32
    {{249, 246, 242, 255}, 0xF65E3BFF}, // 64
    {{249, 246, 242, 255}, 0xECDF72FF}, // 128
    {{249, 246, 242, 255}, 0xEDCC61FF}, // 256
    {{249, 246, 242, 255}, 0xEDC850FF}, // 512
    {{249, 246, 242, 255}, 0xEDC53FFF}, // 1024
    {{249, 246, 242, 255}, 0xEDC22EFF} // 2048
};

/**
* \brief Global styles
*/
const SDL_Color background_color = {238, 228, 218, 255};
const SDL_Color menu_text_color = {119, 110, 101, 255};

/**
* \brief Get appropriate styling for a number
* \param tile_num The number on a tile
* \return A rect_style object containing the text and background color for a tile.
*/
rect_style getTile(int tile_num) {
    if (tile_num > 2048)
        return tiles[0];

    return tiles[(int) log2(tile_num)];
}

/**
* \brief Draw a string
* \param renderer
* \param x0,y0,x1,y1 frame
* \param text Text to draw
* \param font pointer to a TTF_Font to draw with
* \param text_color SDL_Color to draw with
*/
void draw_text_to_center(SDL_Renderer *renderer, int x0, int y0, int x1, int y1, char *text, TTF_Font *font, SDL_Color text_color) {
    SDL_Surface *text_s;
    SDL_Texture *text_t;

    SDL_Rect loc = { 0, 0, 0, 0 };

    text_s = TTF_RenderUTF8_Blended(font, text, text_color);
    text_t = SDL_CreateTextureFromSurface(renderer, text_s);
    loc.x = (x1 + x0) / 2 - (text_s -> w / 2);
    loc.y = (y1 + y0) / 2 - (text_s -> h / 2);
    loc.w = text_s -> w;
    loc.h = text_s -> h;

    SDL_RenderCopy(renderer, text_t, NULL, &loc);
}


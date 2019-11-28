#ifndef STYLE_H_INCLUDED
#define STYLE_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/**
* \brief Text & background color struct
* Structure used to store the visual properties
* of a rectangle with text, for example a button
*/
typedef struct rect_style {
    SDL_Color textColor;
    Uint32 backgroundColor;
}rect_style;

/**
* \brief Rendering hyperparameters
* To be used as a shorthand for the (mostly) constants
* that are needed to draw anything to the renderer.
*/
struct render_params {
    SDL_Renderer *renderer;
    int x0, y0, x1, y1;
    TTF_Font *font;
};

/**
* \brief Global menu background color
*/
extern const SDL_Color background_color;
/**
* \brief Global menu text color
*/
extern const SDL_Color menu_text_color;

struct button_style {
    rect_style inactive;
    rect_style hover;
    rect_style down;
};

/**
* \brief Get appropriate styling for a number
* \param tile_num The number on a tile
* \return A rect_style object containing the text and background color for a tile.
*/
rect_style getTile(int tile_num);

/**
* \brief Draw a string
* \param renderer
* \param x0,y0,x1,y1 frame
* \param text Text to draw
* \param font pointer to a TTF_Font to draw with
* \param text_color SDL_Color to draw with
*/
void draw_text_to_center(SDL_Renderer *renderer, int x0, int y0, int x1, int y1, char *text, TTF_Font *font, SDL_Color text_color);

#endif // STYLE_H_INCLUDED

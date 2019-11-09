#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include "style.h"

typedef struct menu_item {
    int id;
    char* title;
    struct button_style style;
}menu_item;

int handle_menu_interaction(SDL_Renderer *renderer, TTF_Font *font, int x0, int y0, int x1, int y1, bool *quit, int (*draw_this_menu)(SDL_Renderer *renderer, TTF_Font *font, int x0, int y0, int x1, int y1, int mouse_x, int mouse_y, bool mouse_down));

int draw_menu_items(SDL_Renderer *renderer, TTF_Font *font, int x0, int y0, int x1, int y1, int mouse_x, int mouse_y, bool mouse_down, menu_item menu_elems[], int count_menu_elems);

int draw_menu_main(SDL_Renderer *renderer, TTF_Font *font, int x0, int y0, int x1, int y1, int mouse_x, int mouse_y, bool mouse_down);

int draw_menu_play(SDL_Renderer *renderer, TTF_Font *font, int x0, int y0, int x1, int y1, int mouse_x, int mouse_y, bool mouse_down);

int draw_menu_highscores(SDL_Renderer *renderer, TTF_Font *font, int x0, int y0, int x1, int y1);

#endif // MENU_H_INCLUDED

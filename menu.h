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

int handle_menu_interaction(const struct render_params render_data, bool *quit, int (*draw_this_menu)(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down));

int draw_menu_items(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down, menu_item menu_elems[], int count_menu_elems);

int draw_menu_main(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down);

int draw_menu_play(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down);

int draw_menu_highscores(const struct render_params render_data);

#endif // MENU_H_INCLUDED

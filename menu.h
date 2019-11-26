#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include "style.h"
#include "highscores.h"


typedef struct menu_item {
    int id;
    char* title;
    struct button_style style;
}menu_item;

int handle_menu_interaction(const struct render_params render_data, bool *quit, int (*draw_this_menu)(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down));

int draw_menu_items(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down, menu_item menu_elems[], int count_menu_elems);

int draw_menu_main(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down);

int draw_menu_play(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down);

int handle_menu_newgame_interaction(const struct render_params render_data, char *name, int len);

bool draw_menu_choose_name(const struct render_params render_data, char *dest, int len);

int draw_menu_choose_fieldsize(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down, int *field_size, char *name);

int handle_menu_hs_interaction(const struct render_params render_data, bool *quit, HS_Node *hs_node);

int draw_menu_highscores(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down, HS_Node *hs_node);

#endif // MENU_H_INCLUDED

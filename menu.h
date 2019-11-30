#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include "style.h"
#include "highscores.h"
#include "game.h"

/**
* \brief Structure used to store a menu button
*/
typedef struct menu_item {
    int id;
    char* title;
    struct button_style style;
}menu_item;

/**
* \brief Button-based menu handler
* \param render_data render hyperparameters
* \param quit bool pointer to store whether or not to quit the current menu event loop
* \param draw_this_menu function pointer pointing to the appropriate draw_{menu_name} function
*/
int handle_menu_interaction(const struct render_params render_data, bool *quit, int (*draw_this_menu)(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down));

/**
* \brief Draw menu buttons and get the selected button
* \param render_data
* \param menu_elems menu_item array with the buttons to be displayed and their ID-s
* Draws each button provided via the menu_elems array, and returns the ID of the
* button the user is currently hovering their mouse above.
* \return ID of selected menu
*/
int draw_menu_items(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down, menu_item menu_elems[], int count_menu_elems);

/**
* \brief Draw the main menu
*/
int draw_menu_main(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down);

/**
* \brief Draw 'Play' submenu
*/
int draw_menu_play(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down);

/**
* \brief Enter new game menu, handle all input and then return
* WARNING: while loops included free of charge
* \return tabla_size or -1 if user input failed
*/
bool handle_menu_newgame_interaction(const struct render_params render_data, tabla **to_create);

/**
* \brief Draw the name chooser and get text input
* \param render_data render_params object
* \param dest Character array pointer to store the text input at
* \param len Maximum length of text to read
* \return success boolean
*/
bool draw_menu_choose_name(const struct render_params render_data, char *dest, int len);

/**
* \brief Draw fieldsize chooser UI
*/
int draw_menu_choose_fieldsize(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down, int *field_size, char *name);

/**
* \brief Exit-only interaction handler for high scores
*/
bool handle_menu_hs_interaction(const struct render_params render_data, bool *quit, HS_Node *hs_node);

/**
* \brief Draw the **High Scores** menu
* Show the Top 10 highest scores across the board
*/
int draw_menu_highscores(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down, HS_Node *hs_node);

/**
* \brief Draw simple win message
*/
void draw_win_splash(const struct render_params render_data);

/**
* \brief Draw simple lost message
*/
void draw_lose_splash(const struct render_params render_data);

#endif // MENU_H_INCLUDED

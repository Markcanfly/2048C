#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>
#include "menu.h"
#include "style.h"
#include "text_input.h"
#include "debugmalloc.h"
#include "game.h"

/**
* \brief Global style for menu buttons
*/
const struct button_style menu_button = {
    .inactive = {{119, 110, 101, 255}, 0xEEE4DAFF},
    .hover = {{119, 110, 101, 255}, 0xEDE0C8FF},
    .down = {{249, 246, 242, 255}, 0xF2B179FF}
};

/**
* \brief Button-based menu handler
* \param render_data render hyperparameters
* \param quit bool pointer to store whether or not to quit the current menu event loop
* \param draw_this_menu function pointer pointing to the appropriate draw_{menu_name} function
*/
int handle_menu_interaction(const struct render_params render_data, bool *quit, int (*draw_this_menu)(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down)) {
    int mouse_x;
    int mouse_y;
    int choice;
    bool clicked = false;
    bool keyup = false;

    SDL_Event event;
    SDL_WaitEvent(&event);

    switch (event.type) {
        case SDL_MOUSEMOTION:
            mouse_x = event.motion.x;
            mouse_y = event.motion.y;
            break;
        case SDL_MOUSEBUTTONDOWN:
            mouse_x = event.button.x;
            mouse_y = event.button.y;
            clicked = true;
            break;
        case SDL_MOUSEBUTTONUP:
            mouse_x = event.button.x;
            mouse_y = event.button.y;
            clicked = true;
            keyup = true;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE: *quit = true; break;
            }
            break;
    }
    SDL_RenderClear(render_data.renderer);
    /*
    Draw menu with appropriate button states,
    then return choice on KEYUP or -1 otherwise.
    */
    choice = draw_this_menu(render_data, mouse_x, mouse_y, clicked);
    if (keyup)
        return choice;

    return -1;
}

/**
* \brief Draw menu buttons and get the selected button
* \param render_data
* \param menu_elems menu_item array with the buttons to be displayed and their ID-s
* Draws each button provided via the menu_elems array, and returns the ID of the
* button the user is currently hovering their mouse above.
* \return ID of selected menu
*/
int draw_menu_items(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down, menu_item menu_elems[], int count_menu_elems) {

    // Background
    boxColor(render_data.renderer, render_data.x0, render_data.y0, render_data.x1, render_data.y1, 0xD2B48CFF);

    int selected_menu_elem = -1;

    int frame_width = render_data.x1 - render_data.x0;
    int frame_height = render_data.y1 - render_data.y0;
    SDL_Rect buttons_frame = {
        render_data.x0 + frame_width / 4,
        render_data.y0 + frame_height / 4,
        frame_width / 2,
        frame_height / 2
    };

    // Draw each menu item

    for (int i = 0; i < count_menu_elems; i++) {
        int btn_height = (buttons_frame.h - render_data.y0) / count_menu_elems;

        int r_x0 = buttons_frame.x;
        int r_y0 = btn_height * i;
        int r_x1 = buttons_frame.x + buttons_frame.w;
        int r_y1 = btn_height * (i+1);


        rect_style btn_style;
        if (r_x0 <= mouse_x && mouse_x <= r_x1 && r_y0 <= mouse_y && mouse_y <= r_y1) {
            selected_menu_elem = menu_elems[i].id;
            if (mouse_down) {
                btn_style = menu_elems[i].style.down;
            } else {
                btn_style = menu_elems[i].style.hover;
            }
        } else {
            btn_style = menu_elems[i].style.inactive;
        }

        roundedBoxColor(render_data.renderer, r_x0, r_y0, r_x1, r_y1, 10, btn_style.backgroundColor);

        // Text handling
        draw_text_to_center(render_data.renderer, r_x0, r_y0, r_x1, r_y1, menu_elems[i].title, render_data.font, btn_style.textColor);
    }

    return selected_menu_elem;
}

/**
* \brief Draw the main menu
*/
int draw_menu_main(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down){
    const int count_menu_elems = 3;
    menu_item menu_elems[] = {
        {0, "Play", menu_button},
        {1, "High Scores", menu_button},
        {2, "Exit", menu_button},
    };

    // Draw frame and return choice
    return draw_menu_items(render_data, mouse_x, mouse_y, mouse_down, menu_elems, count_menu_elems);
}

/**
* \brief Draw 'Play' submenu
*/
int draw_menu_play(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down) {
    const int count_menu_elems = 3;
    menu_item menu_elems[] = {
        {0, "Continue", menu_button},
        {1, "New Game", menu_button},
        {2, "Back", menu_button},
    };

    // Draw frame and return choice
    return draw_menu_items(render_data, mouse_x, mouse_y, mouse_down, menu_elems, count_menu_elems);
}

/**
* \brief Enter new game menu, handle all input and then return
* WARNING: while loops included free of charge
* \return tabla_size or -1 if user input failed
*/
bool handle_menu_newgame_interaction(const struct render_params render_data, tabla **to_create) {
    char name[51];
    SDL_RenderClear(render_data.renderer);
    bool successful = draw_menu_choose_name(render_data, name, 51);
    bool quit_fs = false;
    int field_size = 4;
    if (successful) {
        // Handle field size selection
        draw_menu_choose_fieldsize(render_data, 0, 0, false, &field_size, name);

        while (!quit_fs) {
            int mouse_x;
            int mouse_y;
            int choice = -1;
            bool clicked = false;
            bool keyup = false;

            SDL_Event event;
            SDL_WaitEvent(&event);

            switch (event.type) {
                case SDL_MOUSEMOTION:
                    mouse_x = event.motion.x;
                    mouse_y = event.motion.y;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mouse_x = event.button.x;
                    mouse_y = event.button.y;
                    clicked = true;
                    break;
                case SDL_MOUSEBUTTONUP:
                    mouse_x = event.button.x;
                    mouse_y = event.button.y;
                    clicked = true;
                    keyup = true;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE: quit_fs = true; return false; break;
                        case SDLK_LEFT: if (field_size > 1) field_size--; break;
                        case SDLK_RIGHT: if (field_size < 10) field_size++; break;
                        case SDLK_RETURN: quit_fs = true;
                    }
                    break;
            }
            SDL_RenderClear(render_data.renderer);
            choice = draw_menu_choose_fieldsize(render_data, mouse_x, mouse_y, clicked, &field_size, name);
            SDL_RenderPresent(render_data.renderer);

            if (keyup) {
                switch (choice) {
                    case 0:
                        field_size--;
                        break;
                    case 1:
                        field_size++;
                        break;
                    case 2:
                        quit_fs = true;
                        break;
                }
            }
        }

        // Check if field size satisfies constraints
        if (field_size > 1 && field_size < 10) {
            free_tabla(*to_create);
            *to_create = create_tabla(name, field_size, field_size, field_size - 1);
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }

    // Anything fails terribly, just return false
    return false;

}

/**
* \brief Draw the name chooser and get text input
* \param render_data render_params object
* \param dest Character array pointer to store the text input at
* \param len Maximum length of text to read
* \return success boolean
*/
bool draw_menu_choose_name(const struct render_params render_data, char *dest, int len) {

    // Background
    boxColor(render_data.renderer, render_data.x0, render_data.y0, render_data.x1, render_data.y1, 0xD2B48CFF);

    // Draw user prompt

    int l[4]; // Location of text to draw to, in the format 'x0 y0 x1 y1'

    l[0] = render_data.x0 + (render_data.x1 - render_data.x0) / 4;
    l[1] = render_data.y0 + (render_data.y1 - render_data.y0) / 8;
    l[2] = render_data.x0 + ((render_data.x1 - render_data.x0) / 4) * 3;
    l[3] = render_data.y0 + (render_data.y1 - render_data.y0) / 8;

    draw_text_to_center(render_data.renderer,l[0], l[1], l[2], l[3] * 2, "New Game", render_data.font, menu_text_color);
    draw_text_to_center(render_data.renderer,l[0], l[1], l[2], l[3] * 6, "Your name:", render_data.font, menu_text_color);
    draw_text_to_center(render_data.renderer,l[0], l[1] * 3, l[2], l[3] * 7, "(overwrites save)", render_data.font, menu_text_color);
    draw_text_to_center(render_data.renderer,l[0], l[1] * 6, l[2], l[3] * 8, "Press ESC to go Back", render_data.font, menu_text_color);

    SDL_RenderPresent(render_data.renderer);

    // Draw text input field

    SDL_Rect input_rect = { 0, 0, 0, 0 };

    input_rect.x = render_data.x0 + (render_data.x1 - render_data.x0) / 4;
    input_rect.y = render_data.y0 + (render_data.y1 - render_data.y0) / 2;
    input_rect.w = (render_data.x1 - render_data.x0) / 2;
    input_rect.h = 40; // const, because the text loaded is a constant size

    bool successful = input_text(dest, 51, input_rect, background_color, menu_text_color, render_data.font, render_data.renderer);

    return successful;
}

/**
* \brief Draw fieldsize chooser UI
*/
int draw_menu_choose_fieldsize(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down, int *field_size, char *name) {
    boxColor(render_data.renderer, render_data.x0, render_data.y0, render_data.x1, render_data.y1, 0xD2B48CFF); // Background
    char fs_str[3];
    itoa(*field_size, fs_str, 10);

    // Text
    draw_text_to_center(render_data.renderer, render_data.x0, 50, render_data.x1, 250, "Select game size", render_data.font, menu_text_color);
    if (*field_size <= 1 || *field_size >= 10)
        draw_text_to_center(render_data.renderer, render_data.x0, 200, render_data.x1, 350, "1 < x < 10", render_data.font, menu_text_color);
    draw_text_to_center(render_data.renderer, render_data.x0, 100, render_data.x1, 300, fs_str, render_data.font, menu_text_color);
    draw_text_to_center(render_data.renderer, render_data.x0, 100, render_data.x1, 400, "<   >", render_data.font, menu_text_color);
    draw_text_to_center(render_data.renderer, render_data.x0, 300, render_data.x1, 400, "Press ESC to go Back", render_data.font, menu_text_color);

    // Draw buttons



    return 0;

}

/**
* \brief Exit-only interaction handler for high scores
* \return true if user hit exit, otherwise false
*/
bool handle_menu_hs_interaction(const struct render_params render_data, bool *quit, HS_Node *hs_node) {
    int mouse_x;
    int mouse_y;
    int choice;
    bool clicked = false;
    bool keyup = false;

    SDL_Event event;
    SDL_WaitEvent(&event);

    switch (event.type) {
        case SDL_MOUSEMOTION:
            mouse_x = event.motion.x;
            mouse_y = event.motion.y;
            break;
        case SDL_MOUSEBUTTONDOWN:
            mouse_x = event.button.x;
            mouse_y = event.button.y;
            clicked = true;
            break;
        case SDL_MOUSEBUTTONUP:
            mouse_x = event.button.x;
            mouse_y = event.button.y;
            clicked = true;
            keyup = true;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE: *quit = true; break;
            }
            break;
    }
    SDL_RenderClear(render_data.renderer);
    /*
    If choice is 0, we know user hit back, otherwise it's -1
    */
    choice = draw_menu_highscores(render_data, mouse_x, mouse_y, clicked, hs_node);
    if (keyup)
        if (choice == 0)
            return true;

    return false;
}

/**
* \brief Draw the **High Scores** menu
* Show the Top 10 highest scores across the board
*/
int draw_menu_highscores(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down, HS_Node *hs_node) {

    int i = 8;
    // Set drawing parameters
    const int y_offset = 40;
    const int y_bottom_margin = 80;
    const int table_height = (render_data.y1 - render_data.y0) - y_bottom_margin;
    const int cell = table_height / i;
    const int name_x0 = render_data.x0 + (render_data.x1 - render_data.x0) / 10; // Margin for name
    const int name_x1 = render_data.x0 + (render_data.x1 - render_data.x0) / 3;
    const int field_size_x0 = render_data.x0 + (render_data.x1 - render_data.x0) / 3 + 5;
    const int field_size_x1 = render_data.x0 + (render_data.x1 - render_data.x0) / 3 * 2;
    const int score_x0 = render_data.x0 + (render_data.x1 - render_data.x0) / 3 * 2 + 5;
    const int score_x1 = render_data.x1 - 5;

    // Draw highscores
    boxColor(render_data.renderer, render_data.x0, render_data.y0, render_data.x1, render_data.y1, 0xD2B48CFF); // Background
    const int i_max = i;
    for (HS_Node *c = hs_node; c != NULL && i > 0; c = c -> next) {
        // Draw data
        draw_text_to_center(render_data.renderer, name_x0, y_offset + cell * (i_max-i), name_x1, y_offset + cell * (i_max - (i - 1)), c -> name, render_data.font, menu_text_color);
        char field_size_text[3]; // buffer for 2-digit numbers for the field size
        char score_text[12]; // buffer for 11 digit score
        itoa(c -> field_size, field_size_text, 10);
        itoa(c -> score, score_text, 10);
        draw_text_to_center(render_data.renderer, field_size_x0, y_offset + cell * (i_max-i), field_size_x1, y_offset + cell * (i_max - (i - 1)), field_size_text, render_data.font, menu_text_color);
        draw_text_to_center(render_data.renderer, score_x0, y_offset + cell * (i_max-i), score_x1, y_offset + cell * (i_max - (i - 1)), score_text, render_data.font, menu_text_color);
        i--;
    }

    // Draw exit button
    int button_height = (render_data.y1 - render_data.y0) / 8;
    int r_x0 = render_data.x0 + (render_data.x1 - render_data.x0) / 4;
    int r_y0 = render_data.y1 - button_height;
    int r_x1 = render_data.x1 - (render_data.x1 - render_data.x0) / 4;
    int r_y1 = render_data.y1;

    rect_style btn_style;
    menu_item exit_button = { 0, "Exit", menu_button };
    int selected_menu_elem = -1;

    if (r_x0 <= mouse_x && mouse_x <= r_x1 && r_y0 <= mouse_y && mouse_y <= r_y1) {
        selected_menu_elem = 0;
        if (mouse_down) {
            btn_style = exit_button.style.down;
        } else {
            btn_style = exit_button.style.hover;
        }
    } else {
        btn_style = exit_button.style.inactive;
    }

    roundedBoxColor(render_data.renderer, r_x0, r_y0, r_x1, r_y1, 10, btn_style.backgroundColor);

    // Text handling
    draw_text_to_center(render_data.renderer, r_x0, r_y0, r_x1, r_y1, exit_button.title, render_data.font, btn_style.textColor);

    SDL_RenderPresent(render_data.renderer);

    return selected_menu_elem;
}

/**
* \brief Draw simple win message
*/
void draw_win_splash(const struct render_params render_data) {
    // Semi-transparent background
    boxColor(render_data.renderer, render_data.x0, render_data.y0, render_data.x1, render_data.y1, 0xD2B48C88);
    draw_text_to_center(render_data.renderer, render_data.x0, render_data.y0, render_data.x1, render_data.y1 - 80, "Congratulations!", render_data.font, menu_text_color);
    draw_text_to_center(render_data.renderer, render_data.x0, render_data.y0, render_data.x1, render_data.y1, "You've won.", render_data.font, menu_text_color);
    // draw_text_to_center(render_data.renderer, render_data.x0, render_data.y0 + 80, render_data.x1, render_data.y1, "Press any key to continue.", render_data.font, menu_text_color);
}

/**
* \brief Draw simple lost message
*/
void draw_lose_splash(const struct render_params render_data) {
    // Semi-transparent background
    boxColor(render_data.renderer, render_data.x0, render_data.y0, render_data.x1, render_data.y1, 0xD2B48C88);
    draw_text_to_center(render_data.renderer, render_data.x0, render_data.y0, render_data.x1, render_data.y1 - 40, "Game over!", render_data.font, menu_text_color);
    draw_text_to_center(render_data.renderer, render_data.x0, render_data.y0 + 40, render_data.x1, render_data.y1, "Press ESC to exit", render_data.font, menu_text_color);
}


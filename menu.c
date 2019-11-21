#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include "menu.h"
#include "style.h"
#include "text_input.h"

// Global style for menu buttons
struct button_style menu_button = {
    .inactive = {{119, 110, 101, 255}, 0xEEE4DAFF},
    .hover = {{119, 110, 101, 255}, 0xEDE0C8FF},
    .down = {{249, 246, 242, 255}, 0xF2B179FF}
};

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

int draw_menu_items(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down, menu_item menu_elems[], int count_menu_elems) {
    /*
    Takes cursor position, sets the appropriate style of each button,
    draws the frame, and returns the id of the menu item
    that the cursor's hovering above.
    */

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

int draw_menu_main(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down){
    const int count_menu_elems = 3;
    menu_item menu_elems[] = {
        {0, "Play", menu_button},
        {1, "High scores", menu_button},
        {2, "Exit", menu_button},
    };

    // Draw frame and return choice
    return draw_menu_items(render_data, mouse_x, mouse_y, mouse_down, menu_elems, count_menu_elems);
}

int draw_menu_play(const struct render_params render_data, int mouse_x, int mouse_y, bool mouse_down) {
    const int count_menu_elems = 3;
    menu_item menu_elems[] = {
        {0, "Continue", menu_button},
        {1, "New game", menu_button},
        {2, "Back", menu_button},
    };

    // Draw frame and return choice
    return draw_menu_items(render_data, mouse_x, mouse_y, mouse_down, menu_elems, count_menu_elems);
}

bool draw_menu_new_game(const struct render_params render_data, char *dest, int len) {
    SDL_Rect input_rect = { 0, 0, 0, 0 };

    input_rect.x = 100;
    input_rect.y = 100;
    input_rect.w = 300;
    input_rect.h = 300;

    bool successful = input_text(dest, 51, input_rect, background_color, menu_text_color, render_data.font, render_data.renderer);

    return successful;
}

int draw_menu_highscores(const struct render_params render_data);

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include "menu.h"
#include "style.h"

// Global style for menu buttons
struct button_style menu_button = {
    .inactive = {{119, 110, 101, 255}, 0xEEE4DAFF},
    .hover = {{119, 110, 101, 255}, 0xEDE0C8FF},
    .down = {{249, 246, 242, 255}, 0xF2B179FF}
};

int draw_menu_main(SDL_Renderer *renderer, TTF_Font *font, int x0, int y0, int x1, int y1, int mouse_x, int mouse_y, bool mouse_down) {
    /*
    Takes cursor position, sets the appropriate style of each button,
    draws the frame, and returns the id of the menu item
    that the cursor's hovering above.
    */

    printf("%d %d\n", mouse_x, mouse_y); // DEBUG ONLY
    menu_item main_menu[3] = {
        {0, "Play", menu_button},
        {1, "High scores", menu_button},
        {2, "Exit", menu_button}
    };
    int count_menu_elems = 3;
    int selected_menu_elem = -1;

    int frame_width = x1 - x0;
    int frame_height = y1 - y0;
    SDL_Rect buttons_frame = {
        x0 + frame_width / 4,
        y0 + frame_height / 4,
        frame_width / 2,
        frame_height / 2
    };
    // Text surface
    SDL_Surface *button_s;
    SDL_Texture *button_t;
    SDL_Rect loc = { 0, 0, 0, 0 };


    // Draw each menu item

    for (int i = 0; i < count_menu_elems; i++) {
        int r_x0 = buttons_frame.x;
        int r_y0 = (buttons_frame.h / count_menu_elems) * i;
        int r_x1 = buttons_frame.x + buttons_frame.w;
        int r_y1 = (buttons_frame.h / count_menu_elems) * (i+1);
        int btn_height = buttons_frame.h / count_menu_elems;

        rect_style btn_style;
        if (r_x0 <= mouse_x && mouse_x <= r_x1 && r_y0 <= mouse_y && mouse_y <= r_y1) {
            selected_menu_elem = i;
            if (mouse_down) {
                btn_style = main_menu[i].style.down;
            } else {
                btn_style = main_menu[i].style.hover;
            }
        } else {
            btn_style = main_menu[i].style.inactive;
        }

        roundedBoxColor(renderer, r_x0, r_y0, r_x1, r_y1, 10, btn_style.backgroundColor);

        // Text handling

        button_s = TTF_RenderUTF8_Blended(font, main_menu[i].title, btn_style.textColor);
        button_t = SDL_CreateTextureFromSurface(renderer, button_s);

        loc.x = r_x0 + (buttons_frame.w - button_s -> w) / 2;
        loc.y = r_y0 + (((buttons_frame.h - y0) / count_menu_elems) - button_s -> h) / 2;
        loc.w = button_s -> w;
        loc.h = button_s -> h;

        SDL_RenderCopy(renderer, button_t, NULL, &loc);
    }

    SDL_FreeSurface(button_s);
    SDL_DestroyTexture(button_t);

    return selected_menu_elem;
}

void draw_menu_highscores(SDL_Renderer *renderer, TTF_Font *num_font, int x0, int y0, int x1, int y1);

void draw_menu_play(SDL_Renderer *renderer, TTF_Font *num_font, int x0, int y0, int x1, int y1);

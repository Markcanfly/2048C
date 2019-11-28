#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "text_input.h"
#include "debugmalloc.h"

/** \brief Reads text input from the keyboard.
* \param dest Char array pointer to store the input at
* \param len Max length of text
* \param rectangle SDL_Rect object to draw to
* \param background SDL_Color
* \param text_color SDL_Color
* \param font The TTF_Font object pointer to draw the text with
* \param renderer The SDL_Renderer pointer to draw to
* \return true if successful, otherwise false
*
* Source: https://infoc.eet.bme.hu
*/
bool input_text(char *dest, size_t len, SDL_Rect rectangle, SDL_Color background, SDL_Color text_color, TTF_Font *font, SDL_Renderer *renderer) {

    /* Ez tartalmazza az aktualis szerkesztest */
    char composition[SDL_TEXTEDITINGEVENT_TEXT_SIZE];
    composition[0] = '\0';
    /* Ezt a kirajzolas kozben hasznaljuk */
    char textandcomposition[len + SDL_TEXTEDITINGEVENT_TEXT_SIZE + 1];
    /* Max hasznalhato szelesseg */
    int maxw = rectangle.w - 2;
    int maxh = rectangle.h - 2;

    dest[0] = '\0';

    bool enter = false;
    bool kilep = false;

    SDL_StartTextInput();
    while (!kilep && !enter) {
        /* doboz kirajzolasa */
        boxRGBA(renderer, rectangle.x, rectangle.y, rectangle.x + rectangle.w - 1, rectangle.y + rectangle.h - 1, background.r, background.g, background.b, 255);
        rectangleRGBA(renderer, rectangle.x, rectangle.y, rectangle.x + rectangle.w - 1, rectangle.y + rectangle.h - 1, text_color.r, text_color.g, text_color.b, 255);


        /* szoveg kirajzolasa */
        int w;
        strcpy(textandcomposition, dest);
        strcat(textandcomposition, composition);
        if (textandcomposition[0] != '\0') {
            SDL_Surface *felirat = TTF_RenderUTF8_Blended(font, textandcomposition, text_color);
            SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
            SDL_Rect cel = { rectangle.x, rectangle.y, felirat->w < maxw ? felirat->w : maxw, felirat->h < maxh ? felirat->h : maxh };
            SDL_RenderCopy(renderer, felirat_t, NULL, &cel);
            SDL_FreeSurface(felirat);
            SDL_DestroyTexture(felirat_t);
            w = cel.w;
        } else {
            w = 0;
        }
        /* kurzor kirajzolasa */
        if (w < maxw) {
            vlineRGBA(renderer, rectangle.x + w + 2, rectangle.y + 2, rectangle.y + rectangle.h - 3, text_color.r, text_color.g, text_color.b, 192);
        }
        /* megjeleniti a képernyon az eddig rajzoltakat */
        SDL_RenderPresent(renderer);

        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type) {
            /* Kulonleges karakter */
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    int textlen = strlen(dest);
                    do {
                        if (textlen == 0) {
                            break;
                        }
                        if ((dest[textlen-1] & 0x80) == 0x00)   {
                            /* Egy bajt */
                            dest[textlen-1] = 0x00;
                            break;
                        }
                        if ((dest[textlen-1] & 0xC0) == 0x80) {
                            /* Bajt, egy tobb-bajtos szekvenciabol */
                            dest[textlen-1] = 0x00;
                            textlen--;
                        }
                        if ((dest[textlen-1] & 0xC0) == 0xC0) {
                            /* Egy tobb-bajtos szekvencia elso bajtja */
                            dest[textlen-1] = 0x00;
                            break;
                        }
                    } while(true);
                } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    kilep = true;
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    enter = true;
                }
                break;

            /* A feldolgozott szoveg bemenete */
            case SDL_TEXTINPUT:
                if (strlen(dest) + strlen(event.text.text) < len) {
                    strcat(dest, event.text.text);
                }

                /* Az eddigi szerkesztes torolheto */
                composition[0] = '\0';
                break;

            /* Szoveg szerkesztese */
            case SDL_TEXTEDITING:
                strcpy(composition, event.edit.text);
                break;

            case SDL_QUIT:
                /* visszatesszuk a sorba ezt az eventet, mert
                 * sok mindent nem tudunk vele kezdeni */
                kilep = true;
                break;
        }
    }

    /* igaz jelzi a helyes beolvasast; = ha enter miatt allt meg a ciklus */
    SDL_StopTextInput();
    return enter;
}

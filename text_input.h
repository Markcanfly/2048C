#ifndef TEXTINPUT_H_INCLUDED
#define TEXTINPUT_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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
bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer);

#endif // TEXTINPUT_H_INCLUDED

#ifndef TEXTINPUT_H_INCLUDED
#define TEXTINPUT_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer);

#endif // TEXTINPUT_H_INCLUDED

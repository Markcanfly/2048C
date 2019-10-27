#ifndef PUSH_TILES_H_INCLUDED
#define PUSH_TILES_H_INCLUDED
#include "game.h"

void push_up(tabla *to_push);
void push_down(tabla *to_push);
void push_left(tabla *to_push);
void push_right(tabla *to_push);

void add_random(tabla *to_add);
int my_rand(int from, int to);

#endif // PUSH_TILES_H_INCLUDED

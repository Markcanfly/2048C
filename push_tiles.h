#ifndef PUSH_TILES_H_INCLUDED
#define PUSH_TILES_H_INCLUDED
#include <stdbool.h>
#include "game.h"
#include "highscores.h"

int push_up(tabla *to_push, HS_Node **hs_node);
int push_down(tabla *to_push, HS_Node **hs_node);
int push_left(tabla *to_push, HS_Node **hs_node);
int push_right(tabla *to_push, HS_Node **hs_node);

void add_random(tabla *to_add);
int my_rand(int from, int to);

#endif // PUSH_TILES_H_INCLUDED

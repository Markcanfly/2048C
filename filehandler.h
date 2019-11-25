#ifndef FILEHANDLER_H_INCLUDED
#define FILEHANDLER_H_INCLUDED

#include "game.h"
#include "highscores.h"

tabla *load_save();

void store_save(const tabla *to_store);

HS_Node *load_highscores();

void store_highscores(HS_Node *to_store);

#endif // FILEHANDLER_H_INCLUDED

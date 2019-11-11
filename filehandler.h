#ifndef FILEHANDLER_H_INCLUDED
#define FILEHANDLER_H_INCLUDED

#include <stdio.h>
#include "filehandler.c"
#include "game.h"

tabla *load_save();

void store_save(const tabla *to_store);

//void load_highscores();




#endif // FILEHANDLER_H_INCLUDED

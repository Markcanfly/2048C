#ifndef FILEHANDLER_H_INCLUDED
#define FILEHANDLER_H_INCLUDED

#include "game.h"
#include "highscores.h"

/**
* \brief Load the saved gamestate from a static location
* Load the saved gamestate from *%workingdir%/data/mentes.txt*
* Expects the following format:
* - Name
* - Score
* - SIZE_X
* - SIZE_Y
* - `SIZE_Y` Rows and `SIZE_X` columns of numbers, separated with a space
* - Previous score (for undo)
* - `SIZE_Y` Rows and `SIZE_X` columns of numbers, separated with a space or '-' if nonexistent (previous state for undo)
*/
tabla *load_save();

/**
* \brief Store a save of the current gamestate to a static location
* Stores the save at *%workingdir%/data/mentes.txt*
* \param to_store pointer to a tabla object
*/
void store_save(const tabla *to_store);

/**
* \brief Load the saved highscores from a static location
* Load the saved highscores from *%workingdir%/data/toplista.txt*
* Expects the following format on each line:
* {name}-{field_size}-{score}
*/
HS_Node *load_highscores();

/**
* \brief Store the saved highscores to a static location
* Write the saved highscores to *%workingdir%/data/toplista.txt*
* Uses the following format on each line:
* {name}-{field_size}-{score}
*/
void store_highscores(HS_Node *to_store);

#endif // FILEHANDLER_H_INCLUDED

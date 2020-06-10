//

#ifndef INPUT_H
#define INPUT_H

#include "game.h"

// ------------------------------------------------------------

// Top-level routine for user input thread
// Handle user input and add events to the game state external event queue
void input_ThreadRoutine(Game&);

// ------------------------------------------------------------

#endif

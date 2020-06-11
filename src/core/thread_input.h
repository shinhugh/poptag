// Routine that handles user input and pushes updates to the game state

#ifndef THREAD_INPUT_H
#define THREAD_INPUT_H

#include "game.h"

// ------------------------------------------------------------

// Top-level routine for user input thread
// Handle user input and add events to the game state external event queue
void threadRoutine_Input(Game&);

// ------------------------------------------------------------

#endif

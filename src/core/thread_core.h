// Routine that maintains the game state with respect to the flow of time

#ifndef THREAD_CORE_H
#define THREAD_CORE_H

#include "game.h"

// ------------------------------------------------------------

// Top-level routine for game core thread
// Repeatedly update the state for a game instance
void threadRoutine_Core(Game&);

// ------------------------------------------------------------

#endif

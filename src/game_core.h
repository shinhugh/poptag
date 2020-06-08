// Maintain game state and handle access/modification requests

#ifndef GAME_CORE_H
#define GAME_CORE_H

// Thread's top-level routine
void gameCore_ThreadStart();

// Free all resources and terminate the thread
void gameCore_ThreadStop();

// Queue an event
void gameCore_QueueEvent(unsigned int event_counter);

#endif

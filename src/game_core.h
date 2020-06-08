// Maintain game state and handle access/modification requests

#ifndef GAME_CORE_H
#define GAME_CORE_H

// Thread's top-level routine
void threadTaskGameCore();

// Queue an event
void gameCoreQueueEvent(unsigned int event_counter);

#endif

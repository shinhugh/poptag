// Maintain game state and handle access/modification requests

#ifndef GAME_CORE_H
#define GAME_CORE_H

// Thread's top-level routine
void gameCore_ThreadStart();

// Free all resources and terminate the thread
void gameCore_ThreadStop();

// Queue an update to game state
void gameCore_QueueUpdate(unsigned long interval, unsigned int update_type,
void *update_data, unsigned int update_data_size);

#endif

// Maintain game state and handle access/modification requests

#ifndef GAME_CORE_H
#define GAME_CORE_H

#include <mutex>
#include <condition_variable>

// Mutex that locks the thread, making it "sleep"
extern std::mutex thread_mutex;

// Condition for waking the thread
extern std::condition_variable thread_game_core_cv;

// Whether thread should be awake
extern bool thread_game_core_awake;

// Thread's top-level routine
void threadTaskGameCore();

#endif

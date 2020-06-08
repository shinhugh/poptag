// Maintain game state and handle access/modification requests

#ifndef GAME_CORE_H
#define GAME_CORE_H

#include <mutex>
#include <condition_variable>
#include <chrono>
#include <queue>

struct GameEvent {
  std::chrono::time_point<std::chrono::system_clock,
  std::chrono::duration<int>> time;
};

// Mutex that locks the thread, making it "sleep"
extern std::mutex thread_game_core_mutex;

// Condition for waking the thread
extern std::condition_variable thread_game_core_cv;

// Whether thread should be awake
extern bool thread_game_core_awake;

// Queue (min-heap) of planned events, sorted by occurrence times
bool GameEventCompare(const GameEvent&, const GameEvent&);
extern std::priority_queue<GameEvent, std::vector<GameEvent>,
decltype(&GameEventCompare)> event_queue;

// Thread's top-level routine
void threadTaskGameCore();

#endif

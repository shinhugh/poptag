#include "game_core.h"

#include <iostream> // DEBUG

// ------------------------------------------------------------

// Mutex that locks the thread, making it "sleep"
std::mutex thread_game_core_mutex;

// Condition for waking the thread
std::condition_variable thread_game_core_cv;

// Whether thread should be awake
bool thread_game_core_awake = false;

// Queue (min-heap) of planned events, sorted by occurrence times
std::priority_queue<GameEvent, std::vector<GameEvent>,
decltype(&GameEventCompare)> event_queue;

// ------------------------------------------------------------

// Thread's top-level routine
void threadTaskGameCore() {

  // Whether game is ongoing
  bool game_ongoing = true;

  /*
   * TODO:
   * Maintain a min-heap of planned events, sorted by their occurrence times
   * Sleep thread until next occurring event
   * Wake thread upon new incoming event
   */

  // Manipulate mutex through lock wrapper
  std::unique_lock<std::mutex> thread_lock (thread_game_core_mutex,
  std::defer_lock);

  // Handle events
  while(game_ongoing) {
    // Lock mutex (wait until lock is acquirable; proceed once acquired)
    thread_lock.lock();
    // Unlock mutex and sleep until notified to wake up
    thread_game_core_cv.wait(thread_lock, []() {
      return thread_game_core_awake;
    });
    // Wake up and reacquire lock
    std::cerr << "Game core thread has woken up.\n"; // DEBUG
    // Set awake flag to be false
    thread_game_core_awake = false;
    // Release lock
    thread_lock.unlock();
  }
}

// ------------------------------------------------------------

bool GameEventCompare(const GameEvent& a, const GameEvent& b) {
  return a.time < b.time;
}

// ------------------------------------------------------------

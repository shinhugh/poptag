#include <chrono>
#include "game_core.h"

#include <iostream> // DEBUG

// ------------------------------------------------------------

// Mutex that locks the thread, making it "sleep"
std::mutex thread_game_core_mutex;

// Condition for waking the thread
std::condition_variable thread_game_core_cv;

// Whether thread should be awake
bool thread_game_core_awake = false;

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

  // Handle event
  while(game_ongoing) {
    // Lock mutex (wait until mutex is acquirable; proceed once acquired)
    std::unique_lock<std::mutex> thread_lock (thread_game_core_mutex);
    // Unlock mutex and sleep until notified to wake up
    thread_game_core_cv.wait(thread_lock, []() {
      return thread_game_core_awake;
    });
    std::cout << "Game core thread has woken up.\n"; // DEBUG
  }
}

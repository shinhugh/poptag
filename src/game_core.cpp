#include <mutex>
#include <condition_variable>
#include <chrono>
#include <queue>
#include <cstring>
#include "game_core.h"
#include "game_state.h"

#include <iostream> // DEBUG

// ------------------------------------------------------------

// Type declarations

// Update to game state planned to occur at specific time
struct GameStateUpdate {
  // Planned time of occurrence
  std::chrono::time_point<std::chrono::system_clock,
  std::chrono::duration<unsigned long long, std::ratio<1, 1000000000> > > time;
  // Type of update
  unsigned int update_type;
  // Pointer to update object
  unsigned char *update_data;
};

// ------------------------------------------------------------

// Function declarations

// Compare two GameStateUpdate instances by occurrence time
static bool GameStateUpdateCompare(const GameStateUpdate *,
const GameStateUpdate *);

// ------------------------------------------------------------

// Variables

// Whether thread should be terminated
bool kill_thread;

// Mutex that locks the thread, making it "sleep"
static std::mutex thread_game_core_mutex;

// Condition for waking the thread
static std::condition_variable thread_game_core_cv;

// Whether thread should be waken up
static bool thread_game_core_wake = false;

// Queue (min-heap) of planned updates, sorted by occurrence times
static std::priority_queue<GameStateUpdate*, std::vector<GameStateUpdate*>,
bool (*)(const GameStateUpdate *, const GameStateUpdate *)>
update_queue(GameStateUpdateCompare);

// ------------------------------------------------------------

// Thread's top-level routine
void gameCore_ThreadStart() {

  /*
   * TODO:
   * Maintain a min-heap of planned updates, sorted by their occurrence times
   * Sleep thread until next occurring update
   * Wake thread upon new incoming update
   */

  // Manipulate mutex through lock wrapper
  std::unique_lock<std::mutex> thread_lock (thread_game_core_mutex,
  std::defer_lock);

  kill_thread = false;

  // Handle updates
  while(!kill_thread) {

    // Lock mutex (wait until lock is acquirable; proceed once acquired)
    thread_lock.lock();

    // If queue is empty, unlock mutex and sleep until notified to wake up
    if(update_queue.empty()) {
      thread_game_core_cv.wait(thread_lock, []() {
        return thread_game_core_wake;
      });
    }
    // If queue is not empty, unlock mutex and sleep until next queued update or
    // notified to wake up
    else {
      thread_game_core_cv.wait_until(thread_lock, update_queue.top()->time,
      []() {
        return thread_game_core_wake;
      });
    }

    // Wake up and reacquire lock

    // If woken up by notification, new update was added to queue
    if(thread_game_core_wake) {
      // Do nothing; thread will update next wake time on next loop iteration
    }
    // If woken up by timeout, next queued update is ready
    else {
      // Get earliest update, which is the one that caused the timeout
      GameStateUpdate *curr_update = update_queue.top();
      std::cerr << "Update type: "
      + std::to_string(curr_update->update_type) + "\n"; // DEBUG
      update_state(curr_update->update_type, curr_update->update_data);
      // Remove update from queue and free memory
      update_queue.pop();
      delete[] curr_update->update_data;
      delete curr_update;
    }

    // Set wake flag to be false
    thread_game_core_wake = false;
    // Release lock
    thread_lock.unlock();

  }

}

// ------------------------------------------------------------

void gameCore_ThreadStop() {

  {
    // Lock mutex
    std::lock_guard<std::mutex> thread_lock(thread_game_core_mutex);
    // Free memory
    while(!update_queue.empty()) {
      GameStateUpdate *curr_update = update_queue.top();
      update_queue.pop();
      delete[] curr_update->update_data;
      delete curr_update;
    }
    // Set thread termination flag
    kill_thread = true;
    // Set wake flag to true
    thread_game_core_wake = true;
    // Release lock by letting it go out of scope
  }
  thread_game_core_cv.notify_one();

}

// ------------------------------------------------------------

void gameCore_QueueUpdate(unsigned long interval, unsigned int update_type,
void *update_data, unsigned int update_data_size) {

  {
    // Lock mutex
    std::lock_guard<std::mutex> thread_lock(thread_game_core_mutex);
    // Queue new update
    GameStateUpdate *new_update = new GameStateUpdate();
    new_update->time = std::chrono::system_clock::now()
    + std::chrono::milliseconds(interval);
    new_update->update_type = update_type;
    new_update->update_data = new unsigned char[update_data_size];
    memcpy(new_update->update_data, update_data, update_data_size);
    update_queue.push(new_update);
    // Set wake flag to true
    thread_game_core_wake = true;
    // Release lock by letting it go out of scope
  }
  thread_game_core_cv.notify_one();

}

// ------------------------------------------------------------

static bool GameStateUpdateCompare(const GameStateUpdate *a,
const GameStateUpdate *b) {

  return a->time > b->time;

}

// ------------------------------------------------------------

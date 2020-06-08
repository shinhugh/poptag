#include <mutex>
#include <condition_variable>
#include <chrono>
#include <queue>
#include "game_core.h"

#include <iostream> // DEBUG

// ------------------------------------------------------------

// Type declarations

// Game event planned to occur at specific time
struct GameEvent {
  std::chrono::time_point<std::chrono::system_clock,
  std::chrono::duration<unsigned long long, std::ratio<1, 1000000000> > > time;
  unsigned int event_counter; // DEBUG
};

// ------------------------------------------------------------

// Function declarations

// Compare two GameEvent instances by occurrence time
static bool GameEventCompare(const GameEvent *, const GameEvent *);

// ------------------------------------------------------------

// Variables

// Mutex that locks the thread, making it "sleep"
static std::mutex thread_game_core_mutex;

// Condition for waking the thread
static std::condition_variable thread_game_core_cv;

// Whether thread should be awake
static bool thread_game_core_wake = false;

// Queue (min-heap) of planned events, sorted by occurrence times
static std::priority_queue<GameEvent*, std::vector<GameEvent*>,
bool (*)(const GameEvent *, const GameEvent *)> event_queue(GameEventCompare);

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

    // If queue is empty, unlock mutex and sleep until notified to wake up
    if(event_queue.empty()) {
      thread_game_core_cv.wait(thread_lock, []() {
        return thread_game_core_wake;
      });
    }
    // If queue is not empty, unlock mutex and sleep until next queued event or
    // notified to wake up
    else {
      thread_game_core_cv.wait_until(thread_lock, event_queue.top()->time,
      []() {
        return thread_game_core_wake;
      });
    }

    // Wake up and reacquire lock
    std::cerr << "Game core thread has woken up\n"; // DEBUG

    // If woken up by notification, new event was added to queue
    if(thread_game_core_wake) {
      // Do nothing
      std::cerr << "  Reason: Notification\n"; // DEBUG
    }
    // If woken up by timeout, next queued event is ready
    else {
      std::cerr << "  Reason: Timeout\n"; // DEBUG
      // Get earliest event, which is the one that caused the timeout
      GameEvent *currEvent = event_queue.top();
      std::cerr << "  Event counter: "
      + std::to_string(currEvent->event_counter) + "\n"; // DEBUG
      // Remove event from queue and free memory
      event_queue.pop();
      delete currEvent;
    }

    // Set wake flag to be false
    thread_game_core_wake = false;
    // Release lock
    thread_lock.unlock();

  }
}

// ------------------------------------------------------------

void gameCoreQueueEvent(unsigned int event_counter) {
  {
    // Lock mutex
    std::lock_guard<std::mutex> thread_lock(thread_game_core_mutex);
    // Queue new event for 3 seconds later
    GameEvent *new_event = new GameEvent();
    new_event->time = std::chrono::system_clock::now()
    + std::chrono::seconds(3);
    new_event->event_counter = event_counter;
    event_queue.push(new_event);
    // Set wake flag to true
    thread_game_core_wake = true;
    // Release lock by letting it go out of scope
  }
  thread_game_core_cv.notify_one();
}

// ------------------------------------------------------------

static bool GameEventCompare(const GameEvent *a, const GameEvent *b) {
  return a->time > b->time;
}

// ------------------------------------------------------------

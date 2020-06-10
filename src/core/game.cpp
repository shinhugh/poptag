#include <thread>
#include <chrono>
#include "game.h"

// ------------------------------------------------------------

Game::Game(unsigned int tick_duration) :
exit_flag(false), tick_duration(tick_duration) {}

// ------------------------------------------------------------

void Game::tickUpdate() {

  // Update state with respect to external events

  while(!(this->event_queue.empty())) {

    // Next queued event
    EventData event;

    {
      // Acquire mutex protecting event queue
      std::lock_guard<std::mutex> lock(this->event_queue_mutex);

      // Get next queued event
      event = this->event_queue.front();
      this->event_queue.pop();

      // Release mutex by letting lock go out of scope
    }

    {
      // Acquire mutex protecting game state
      std::lock_guard<std::mutex> lock(this->state_mutex);

      // Handle event
      this->state.externalUpdate(event.getType(), event.getData());

      // Release mutex by letting lock go out of scope
    }

  }


  // Update state with respect to internal conditions

  {
    // Acquire mutex protecting game state
    std::lock_guard<std::mutex> lock(this->state_mutex);

    // Update state according to passage of time (1 tick)
    this->state.internalUpdate();

    // Release mutex by letting lock go out of scope
  }

}

// ------------------------------------------------------------

void Game::queueEvent(const EventData& event) {

  {
    // Acquire mutex protecting event queue
    std::lock_guard<std::mutex> lock(this->event_queue_mutex);

    // Place event in queue
    this->event_queue.push(event);

    // Release mutex by letting lock go out of scope
  }

}

// ------------------------------------------------------------

void Game::exit() {

  this->exit_flag = true;

}

// ------------------------------------------------------------

bool Game::isExit() {

  return this->exit_flag;

}

// ------------------------------------------------------------

unsigned int Game::getTickDuration() {

  return this->tick_duration;

}

// ------------------------------------------------------------

void core_ThreadRoutine(Game& game) {

  // Sleep for 1 tick
  auto wakeup_time = std::chrono::system_clock::now()
  + (game.getTickDuration() * std::chrono::microseconds(1));
  std::this_thread::sleep_until(wakeup_time);

  // Wake up and calculate when to wake up next
  wakeup_time = std::chrono::system_clock::now()
  + (game.getTickDuration() * std::chrono::microseconds(1));

  while(!game.isExit()) {

    // Update state, reflecting passage of 1 tick
    game.tickUpdate();

    // Sleep for 1 tick
    std::this_thread::sleep_until(wakeup_time);

    // Wake up and calculate when to wake up next
    wakeup_time = std::chrono::system_clock::now()
    + (game.getTickDuration() * std::chrono::microseconds(1));

  }

}

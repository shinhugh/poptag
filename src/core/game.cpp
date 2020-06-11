#include "game.h"

// ------------------------------------------------------------

Game::Game(unsigned int tick_duration) :
exit_flag(false), tick_duration(tick_duration) {}

// ------------------------------------------------------------

void Game::exit() {

  // Acquire mutex protecting exit flag
  std::lock_guard<std::mutex> lock(this->exit_flag_mutex);

  // Set exit flag
  this->exit_flag = true;

  // Release mutex by letting lock go out of scope

}

// ------------------------------------------------------------

bool Game::isExit() {

  // Acquire mutex protecting exit flag
  std::lock_guard<std::mutex> lock(this->exit_flag_mutex);

  // Return exit flag
  return this->exit_flag;

  // Release mutex by letting lock go out of scope

}

// ------------------------------------------------------------

unsigned int Game::getTickDuration() {

  return this->tick_duration;

}

// ------------------------------------------------------------

void Game::tickUpdate() {

  // Update state with respect to external events

  while(!(this->event_queue.empty())) {

    // Next queued event
    DataPacket packet;

    {
      // Acquire mutex protecting event queue
      std::lock_guard<std::mutex> lock(this->event_queue_mutex);

      // Get next queued event
      packet = this->event_queue.front();
      this->event_queue.pop();

      // Release mutex by letting lock go out of scope
    }

    {
      // Acquire mutex protecting game state
      std::lock_guard<std::mutex> lock(this->state_mutex);

      // Handle event
      this->state.externalUpdate(packet);

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

void Game::queueEvent(const DataPacket& packet) {

  // Acquire mutex protecting event queue
  std::lock_guard<std::mutex> lock(this->event_queue_mutex);

  // Place event in queue
  this->event_queue.push(packet);

  // Release mutex by letting lock go out of scope

}

// ------------------------------------------------------------

DataPacket Game::readState() {

  // Acquire mutex protecting game state
  std::lock_guard<std::mutex> lock(this->state_mutex);

  // Return state
  return this->state.readState();

  // Release mutex by letting lock go out of scope

}

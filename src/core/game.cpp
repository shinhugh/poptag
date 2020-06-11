#include "game.h"

// ------------------------------------------------------------

Game::Game(unsigned int tick_duration) :
exit_flag(false), tick_duration(tick_duration) {}

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

void Game::tickUpdate() {

  // DEBUG: Print state to stderr
  static unsigned int tick_count = 0;
  if(tick_count % 1000 == 0) {
    this->state.drawState();
  }
  tick_count++;
  // DEBUG

  // Update state with respect to external events

  while(!(this->event_queue.empty())) {

    // Next queued event
    DataPacket eventData;

    {
      // Acquire mutex protecting event queue
      std::lock_guard<std::mutex> lock(this->event_queue_mutex);

      // Get next queued event
      eventData = this->event_queue.front();
      this->event_queue.pop();

      // Release mutex by letting lock go out of scope
    }

    {
      // Acquire mutex protecting game state
      std::lock_guard<std::mutex> lock(this->state_mutex);

      // Handle event
      this->state.externalUpdate(eventData.getType(), eventData.getData());

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

void Game::queueEvent(const DataPacket& eventData) {

  {
    // Acquire mutex protecting event queue
    std::lock_guard<std::mutex> lock(this->event_queue_mutex);

    // Place event in queue
    this->event_queue.push(eventData);

    // Release mutex by letting lock go out of scope
  }

}

// ------------------------------------------------------------

DataPacket Game::readState() {

  DataPacket stateData;

  return stateData;

}

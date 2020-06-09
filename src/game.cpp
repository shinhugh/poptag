#include <thread>
#include <chrono>
#include "game.h"

// ------------------------------------------------------------

Game::Game(unsigned int tick_duration) :
exit_flag(false), tick_duration(tick_duration) {

}

// ------------------------------------------------------------

void Game::tickUpdate() {

  {
    // Acquire mutex protecting game state
    std::lock_guard<std::mutex> lock(this->state_mutex);

    // Update state according to passage of time (1 tick)
    this->state.tickUpdate();

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

void Game::placeBomb() {

  {
    // Acquire mutex protecting game state
    std::lock_guard<std::mutex> lock(this->state_mutex);

    // Update test state
    this->state.placeBomb();

    // Release mutex by letting lock go out of scope
  }

}

// ------------------------------------------------------------

void core_ThreadRoutine(Game& game) {

  while(!game.isExit()) {
    // Sleep for 1 tick
    auto wakeup_time = std::chrono::system_clock::now()
    + (game.getTickDuration() * std::chrono::microseconds(1));
    std::this_thread::sleep_until(wakeup_time);
    // If exit flag hasn't been set during sleep, update state
    if(!game.isExit()) {
      // Update state, reflecting passage of 1 tick
      game.tickUpdate();
    }
  }

}

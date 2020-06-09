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

void Game::updateTestState(bool value) {

  {
    // Acquire mutex protecting game state
    std::lock_guard<std::mutex> lock(this->state_mutex);

    // Update test state
    this->state.updateTestState(value);

    // Release mutex by letting lock go out of scope
  }

}

// ------------------------------------------------------------

void Game::exit() {

  this->exit_flag = true;

}

// ------------------------------------------------------------

void core_ThreadStart(Game& game) {

  game.exit_flag = false;

  while(!game.exit_flag) {
    // Sleep for 1 tick
    auto wakeup_time = std::chrono::system_clock::now()
    + (game.tick_duration * std::chrono::microseconds(1));
    std::this_thread::sleep_until(wakeup_time);
    // Update state, reflecting passage of 1 tick
    game.tickUpdate();
  }

}

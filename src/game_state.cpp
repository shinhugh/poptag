#include "game_state.h"

#include <iostream> // DEBUG

// ------------------------------------------------------------

GameState::GameState() : test_state(false) {

}

// ------------------------------------------------------------

void GameState::tickUpdate() {

  // Update test state
  this->test_state = !(this->test_state);

  // DEBUG
  std::cerr << "test_state: " + std::to_string(this->test_state) + "\n";
  // DEBUG

}

// ------------------------------------------------------------

void GameState::updateTestState(bool value) {

  // Update test state
  this->test_state = value;

  // DEBUG
  std::cerr << "test_state: " + std::to_string(this->test_state) + "\n";
  // DEBUG

}

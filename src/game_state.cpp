#include "game_state.h"

// ------------------------------------------------------------

GameState::GameState() : test_state(false) {

}

// ------------------------------------------------------------

void GameState::tickUpdate() {

  // Update test state
  this->test_state = !(this->test_state);

}

// ------------------------------------------------------------

void GameState::updateTestState(bool value) {

  // Update test state
  this->test_state = value;

}

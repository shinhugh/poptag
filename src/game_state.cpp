#include "game_state.h"

#include <iostream> // DEBUG

// ------------------------------------------------------------

GameState::GameState() {

}

// ------------------------------------------------------------

void GameState::tickUpdate() {

  // Update ticks on all bombs
  for(unsigned int i = 0; i < bombs.size(); i++) {
    bombs.at(i).tick();
  }

  // Search for expired bombs
  for(unsigned int i = 0; i < bombs.size();) {
    if(bombs.at(i).getTickAge() >= bombs.at(i).getTickDetonate()) {
      // Remove bomb from list
      bombs.erase(bombs.begin() + i);
      std::cerr << "BOOM!\n";
    } else {
      i++;
    }
  }

}

// ------------------------------------------------------------

void GameState::placeBomb(unsigned int y, unsigned int x) {

  bombs.push_back(Bomb(y, x));

  // DEBUG
  std::cerr << "Bomb placed.\n";
  // DEBUG

}

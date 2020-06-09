#include "game_state.h"

#include <iostream> // DEBUG

// ------------------------------------------------------------

GameState::GameState() {

}

// ------------------------------------------------------------

void GameState::internalUpdate() {

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

void GameState::externalUpdate(const EventData *event) {

  // Check event type
  switch(event->type) {

    // Test event
    case test:
      {
        std::cerr << "Test event: " + std::to_string(event->test_data) + "\n";
      }
      break;

  }

}

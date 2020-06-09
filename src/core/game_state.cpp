#include "game_state.h"

#include <iostream> // DEBUG

// ------------------------------------------------------------

GameState::GameState() {}

// ------------------------------------------------------------

void GameState::internalUpdate() {

  // Update ticks on all bombs
  for(unsigned int i = 0; i < this->bombs.size(); i++) {
    this->bombs.at(i).tick();
  }

  // Search for expired bombs
  for(unsigned int i = 0; i < this->bombs.size();) {
    if(this->bombs.at(i).getTickAge() >= this->bombs.at(i).getTickDetonate()) {
      // Remove bomb from list
      this->bombs.erase(this->bombs.begin() + i);
      std::cerr << "BOOM!\n";
    } else {
      i++;
    }
  }

}

// ------------------------------------------------------------

void GameState::externalUpdate(unsigned int type, void *event_data) {

  // Check event type
  switch(type) {

    // Test event
    case test:
      {
        EventData_Test *data = static_cast<EventData_Test *>(event_data);
        std::cerr << "Test event: " + std::to_string(data->test_data) + "\n";
      }
      break;

    // Bomb event
    case bomb:
      {
        EventData_Bomb *data = static_cast<EventData_Bomb *>(event_data);
        this->bombs.push_back(Bomb(data->y, data->x));
        std::cerr << "Bomb event: [" + std::to_string(data->y) + ", "
        + std::to_string(data->x) + "]\n";
      }
      break;

  }

}

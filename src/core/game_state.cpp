#include "game_state.h"

#include <iostream> // DEBUG

// ------------------------------------------------------------

GameState::GameState() {

}

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

void GameState::externalUpdate(void *event_ptr) {

  EventData *event = static_cast<EventData *>(event_ptr);

  // Check event type
  switch(event->type) {

    // Test event
    case test:
      {

        EventData_Test *data = static_cast<EventData_Test *>(event->data);
        std::cerr << "Test event: " + std::to_string(data->test_data) + "\n";

        delete data;

      }
      break;

    // Bomb event
    case bomb:
      {

        EventData_Bomb *data = static_cast<EventData_Bomb *>(event->data);
        std::cerr << "Bomb event: [" + std::to_string(data->y) + ", "
        + std::to_string(data->x) + "]\n";

        this->bombs.push_back(Bomb(data->y, data->x));

        delete data;

      }
      break;

  }

}

#include "game_state.h"

#include <iostream> // DEBUG

// ------------------------------------------------------------

GameState::GameState() {

}

// ------------------------------------------------------------

void GameState::internalUpdate() {

  /*

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

  */

}

// ------------------------------------------------------------

void GameState::externalUpdate(void *event_ptr) {

  EventData *event = static_cast<EventData *>(event_ptr);

  // Check event type
  switch(event->type) {

    // Test event
    case test:
      {
        TestEventData *data = static_cast<TestEventData *>(event->data);
        std::cerr << "Test event: " + std::to_string(data->test_data) + "\n";

        delete data;
      }
      break;

  }

}

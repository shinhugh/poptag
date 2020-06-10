#include "game_state.h"
#include "event_types.h"

#include <iostream> // DEBUG

// ------------------------------------------------------------

GameState::GameState() :
board(BOARD_HEIGHT, BOARD_WIDTH) {}

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

    // Test
    case test:
      {
        EventData_Test *data = static_cast<EventData_Test *>(event_data);
        std::cerr << "Test: " + data->message + "\n";
      }
      break;

    // Place bomb
    case placeBomb:
      {
        // Parse event
        EventData_PlaceBomb *data
        = static_cast<EventData_PlaceBomb *>(event_data);
        // Add bomb
        this->bombs.push_back(Bomb(data->y, data->x, data->tick_detonate,
        data->range));
        // Log
        std::cerr << "Bomb placed at: (" + std::to_string(data->y) + ", "
        + std::to_string(data->x) + ")\n";
      }
      break;

  }

}

// ------------------------------------------------------------

void GameState::drawState() {

  std::cerr << "\n  ";
  for(unsigned int x = 0; x < this->board.getWidth(); x++) {
    if(x > 0) {
      std::cerr << " ";
    }
    std::cerr << std::to_string(x);
  }
  std::cerr << "\n +";
  for(unsigned int x = 0; x < 2 * this->board.getWidth() - 1; x++) {
    std::cerr << "-";
  }
  std::cerr << "+\n";
  for(unsigned int y = 0; y < this->board.getHeight(); y++) {
    if(y > 0) {
      std::cerr << " |";
      for(unsigned int x = 0; x < this->board.getWidth(); x++) {
        if(x > 0) {
          std::cerr << "+";
        }
        std::cerr << "-";
      }
      std::cerr << "|\n";
    }
    std::cerr << std::to_string(y) + "|";
    for(unsigned int x = 0; x < this->board.getWidth(); x++) {
      if(x > 0) {
        std::cerr << "|";
      }
      switch(this->board.getTerrain(y, x)) {
        case ground:
          {
            bool bomb_here = false;
            for(unsigned int i = 0; i < this->bombs.size(); i++) {
              if(this->bombs.at(i).getY() == y
              && this->bombs.at(i).getX() == x) {
                bomb_here = true;
                break;
              }
            }
            if(bomb_here) {
              std::cerr << "B";
            } else {
              std::cerr << " ";
            }
          }
          break;
        case breakable:
          {
            std::cerr << "O";
          }
          break;
        case unbreakable:
          {
            std::cerr << "X";
          }
          break;
      }
    }
    std::cerr << "|\n";
  }
  std::cerr << " +";
  for(unsigned int x = 0; x < 2 * this->board.getWidth() - 1; x++) {
    std::cerr << "-";
  }
  std::cerr << "+\n\n";

}

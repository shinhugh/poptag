#include "game_state.h"
#include "event_data.h"
#include "state_data.h"

#include <iostream> // DEBUG

// ------------------------------------------------------------

GameState::GameState() :
board(BOARD_HEIGHT, BOARD_WIDTH) {

  this->characters.push_back(Character(0.5, 0.5, 0.01, &(this->board), 2));

}

// ------------------------------------------------------------

void GameState::internalUpdate() {

  // Update positions of all characters
  for(unsigned int i = 0; i < this->characters.size(); i++) {
    this->characters.at(i).tick();
  }

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

void GameState::externalUpdate(DataPacket packet) {

  // Check event type
  switch(packet.getType()) {

    // Initialize
    case initialize:
      {
        EventData_Initialize *event_data
        = static_cast<EventData_Initialize *>(packet.getData());
        if(event_data->initialize) {
          // Reset
          this->characters.clear();
          this->characters.push_back(Character(0.5, 0.5, 0.01, &(this->board),
          2));
        }
      }
      break;

    // Place bomb
    case placeBomb:
      {
        // Parse event
        EventData_PlaceBomb *event_data
        = static_cast<EventData_PlaceBomb *>(packet.getData());
        // Get character's bomb traits
        float character_y
        = this->characters.at(event_data->character_id).getY();
        float character_x
        = this->characters.at(event_data->character_id).getX();
        unsigned int bomb_range
        = this->characters.at(event_data->character_id).getBombRange();
        // Place bomb at whichever square character's center lies in
        this->bombs.push_back(Bomb(static_cast<unsigned int>(character_y),
        static_cast<unsigned int>(character_x), TICK_DETONATE, bomb_range));
      }
      break;

    // Stop moving
    case moveStop:
      {
        EventData_MoveStop *event_data
        = static_cast<EventData_MoveStop *>(packet.getData());
        this->characters.at(event_data->character_id).setDirMove(stop);
      }
      break;

    // Move up
    case moveUp:
      {
        EventData_MoveUp *event_data
        = static_cast<EventData_MoveUp *>(packet.getData());
        this->characters.at(event_data->character_id).setDirMove(up);
      }
      break;

    // Move right
    case moveRight:
      {
        EventData_MoveRight *event_data
        = static_cast<EventData_MoveRight *>(packet.getData());
        this->characters.at(event_data->character_id).setDirMove(right);
      }
      break;

    // Move down
    case moveDown:
      {
        EventData_MoveDown *event_data
        = static_cast<EventData_MoveDown *>(packet.getData());
        this->characters.at(event_data->character_id).setDirMove(down);
      }
      break;

    // Move left
    case moveLeft:
      {
        EventData_MoveLeft *event_data
        = static_cast<EventData_MoveLeft *>(packet.getData());
        this->characters.at(event_data->character_id).setDirMove(left);
      }
      break;

  }

}

// ------------------------------------------------------------

DataPacket GameState::readState() {

  DataPacket packet;
  StateData state_data;

  // TEST
  if(characters.empty()) {
    state_data.character_y = 0;
    state_data.character_x = 0;
  } else {
    state_data.character_y = this->characters.at(0).getY();
    state_data.character_x = this->characters.at(0).getX();
  }
  // TEST

  packet.setData(&state_data, sizeof(StateData));

  return packet;

}

// ------------------------------------------------------------

/*

void GameState::drawState() { // TODO: Remove

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

  std::cerr << "Character positions:\n";
  for(unsigned int i = 0; i < this->characters.size(); i++) {
    std::cerr << "Character " + std::to_string(i) + ":\n("
    + std::to_string(this->characters.at(i).getY()) + ", "
    + std::to_string(this->characters.at(i).getX()) + ")\n";
  }

}

*/

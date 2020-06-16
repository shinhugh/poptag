#include "game_state.h"
#include "event_data.h"

#include <iostream> // DEBUG

// ------------------------------------------------------------

GameState::GameState() :
board(BOARD_HEIGHT, BOARD_WIDTH) {

  this->characters.push_back(Character(0.5, 0.5, 1, 2));

}

// ------------------------------------------------------------

void GameState::internalUpdate(std::chrono::microseconds elapsed_time) {

  // Update positions of all characters
  for(unsigned int i = 0; i < this->characters.size(); i++) {
    this->characters.at(i).update(&(this->board), elapsed_time);
  }

  // Update ticks on all bombs
  for(unsigned int i = 0;
  i < (this->board.getHeight() * this->board.getWidth()); i++) {
    if(this->bombs.count(i) > 0) {
      this->bombs.at(i).update(elapsed_time);
      if(this->bombs.at(i).getTimeAge()
      >= this->bombs.at(i).getTimeDetonate()) {
        // Remove bomb from list
        this->bombs.erase(i);
        std::cerr << "BOOM!\n";
      }
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
          this->characters.push_back(Character(0.5, 0.5, 1, 2));
          // Create blocks
          this->board.createBlock(2, 2);
          this->board.createBlock(3, 2);
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
        unsigned int bomb_y
        = static_cast<unsigned int>
        (this->characters.at(event_data->character_id).getHitbox()
        ->getCenterY());
        unsigned int bomb_x
        = static_cast<unsigned int>
        (this->characters.at(event_data->character_id).getHitbox()
        ->getCenterX());
        unsigned int bomb_range
        = this->characters.at(event_data->character_id).getBombRange();
        unsigned int bomb_key = bomb_y * this->board.getWidth() + bomb_x;
        // Place bomb at whichever square the character's center lies in
        if(this->bombs.count(bomb_key) == 0) {
          this->bombs.emplace(bomb_key, Bomb(bomb_y, bomb_x,
          std::chrono::milliseconds(2000), bomb_range));
        }
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

const Board * GameState::getBoard() const {

  return &(this->board);

}

// ------------------------------------------------------------

const std::vector<Character> * GameState::getCharacters() const {

  return &(this->characters);

}

// ------------------------------------------------------------

const std::map<unsigned int, Bomb> * GameState::getBombs() const {

  return &(this->bombs);

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

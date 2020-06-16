#include "game_state.h"
#include "event_data.h"

#include <iostream> // DEBUG

#define BOARD_HEIGHT 16
#define BOARD_WIDTH 16

// ------------------------------------------------------------

GameState::GameState() :
board_height(BOARD_HEIGHT), board_width(BOARD_WIDTH) {

  // Allocate memory
  this->bombs = new Bomb *[this->board_height];
  this->bombs_exist = new bool *[this->board_height];
  this->blocks = new BoardBlock *[this->board_height];
  this->blocks_exist = new bool *[this->board_height];
  for(unsigned int y = 0; y < this->board_height; y++) {
    this->bombs[y] = new Bomb[this->board_width];
    this->bombs_exist[y] = new bool[this->board_width];
    this->blocks[y] = new BoardBlock[this->board_width];
    this->blocks_exist[y] = new bool[this->board_width];
    // Unset all flags
    for(unsigned int x = 0; x < this->board_width; x++) {
      this->bombs_exist[y][x] = false;
      this->blocks_exist[y][x] = false;
    }
  }

}

// ------------------------------------------------------------

GameState::GameState(const GameState& src) :
board_height(src.board_height), board_width(src.board_width),
characters(src.characters) {

  // Allocate memory
  this->bombs = new Bomb *[this->board_height];
  this->bombs_exist = new bool *[this->board_height];
  this->blocks = new BoardBlock *[this->board_height];
  this->blocks_exist = new bool *[this->board_height];
  for(unsigned int y = 0; y < this->board_height; y++) {
    this->bombs[y] = new Bomb[this->board_width];
    this->bombs_exist[y] = new bool[this->board_width];
    this->blocks[y] = new BoardBlock[this->board_width];
    this->blocks_exist[y] = new bool[this->board_width];
    // Unset all flags
    for(unsigned int x = 0; x < this->board_width; x++) {
      this->bombs_exist[y][x] = false;
      this->blocks_exist[y][x] = false;
    }
  }

  // Copy from source
  for(unsigned int y = 0; y < this->board_height; y++) {
    for(unsigned int x = 0; x < this->board_width; x++) {
      this->bombs[y][x] = src.bombs[y][x];
      this->bombs_exist[y][x] = src.bombs_exist[y][x];
      this->blocks[y][x] = src.blocks[y][x];
      this->blocks_exist[y][x] = src.blocks_exist[y][x];
    }
  }

}

// ------------------------------------------------------------

GameState& GameState::operator=(const GameState& src) {

  // Free memory previously allocated
  for(unsigned int y = 0; y < this->board_height; y++) {
    delete[] this->bombs[y];
    delete[] this->bombs_exist[y];
    delete[] this->blocks[y];
    delete[] this->blocks_exist[y];
  }
  delete[] this->bombs;
  delete[] this->bombs_exist;
  delete[] this->blocks;
  delete[] this->blocks_exist;

  // Copy from source
  this->board_height = src.board_height;
  this->board_width = src.board_width;
  this->characters = src.characters;

  // Allocate memory
  this->bombs = new Bomb *[this->board_height];
  this->bombs_exist = new bool *[this->board_height];
  this->blocks = new BoardBlock *[this->board_height];
  this->blocks_exist = new bool *[this->board_height];
  for(unsigned int y = 0; y < this->board_height; y++) {
    this->bombs[y] = new Bomb[this->board_width];
    this->bombs_exist[y] = new bool[this->board_width];
    this->blocks[y] = new BoardBlock[this->board_width];
    this->blocks_exist[y] = new bool[this->board_width];
    // Unset all flags
    for(unsigned int x = 0; x < this->board_width; x++) {
      this->bombs_exist[y][x] = false;
      this->blocks_exist[y][x] = false;
    }
  }

  // Copy from source
  for(unsigned int y = 0; y < this->board_height; y++) {
    for(unsigned int x = 0; x < this->board_width; x++) {
      this->bombs[y][x] = src.bombs[y][x];
      this->bombs_exist[y][x] = src.bombs_exist[y][x];
      this->blocks[y][x] = src.blocks[y][x];
      this->blocks_exist[y][x] = src.blocks_exist[y][x];
    }
  }

  return *this;

}

// ------------------------------------------------------------

GameState::~GameState() {

  // Free memory previously allocated
  for(unsigned int y = 0; y < this->board_height; y++) {
    delete[] this->bombs[y];
    delete[] this->bombs_exist[y];
    delete[] this->blocks[y];
    delete[] this->blocks_exist[y];
  }
  delete[] this->bombs;
  delete[] this->bombs_exist;
  delete[] this->blocks;
  delete[] this->blocks_exist;

}

// ------------------------------------------------------------

unsigned int GameState::getBoardHeight() const {

  return this->board_height;

}

// ------------------------------------------------------------

unsigned int GameState::getBoardWidth() const {

  return this->board_width;

}

// ------------------------------------------------------------

const std::vector<Character> * GameState::getCharacters() const {

  return &(this->characters);

}

// ------------------------------------------------------------

const Bomb * GameState::getBomb(unsigned int y, unsigned int x) const {

  return &(this->bombs[y][x]);

}

// ------------------------------------------------------------

bool GameState::getBombExist(unsigned int y, unsigned int x) const {

  return this->bombs_exist[y][x];

}

// ------------------------------------------------------------

const BoardBlock * GameState::getBlock(unsigned int y, unsigned int x) const {

  return &(this->blocks[y][x]);

}

// ------------------------------------------------------------

bool GameState::getBlockExist(unsigned int y, unsigned int x) const {

  return this->blocks_exist[y][x];

}

// ------------------------------------------------------------

void GameState::createBlock(unsigned int y, unsigned int x) {

  if(!(this->blocks_exist[y][x])) {
    this->blocks[y][x] = BoardBlock(y, x, unbreakable);
    this->blocks_exist[y][x] = true;
  }

}

// ------------------------------------------------------------

void GameState::internalUpdate(std::chrono::microseconds elapsed_time) {

  // Update positions of all characters
  for(unsigned int i = 0; i < this->characters.size(); i++) {
    this->characters.at(i).update(this, elapsed_time);
  }

  // Update ticks on all bombs
  for(unsigned int y = 0; y < this->board_height; y++) {
    for(unsigned int x = 0; x < this->board_width; x++) {
      if(this->bombs_exist[y][x]) {
        this->bombs[y][x].update(elapsed_time);
        if(this->bombs[y][x].getTimeAge()
        >= this->bombs[y][x].getTimeDetonate()) {
          // Remove bomb from list
          this->bombs_exist[y][x] = false;
          std::cerr << "BOOM!\n";
        }
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
          this->characters.push_back(Character(1.5, 1.5, 1, 2));
          // Create blocks
          this->createBlock(0, 0);
          this->createBlock(1, 0);
          this->createBlock(2, 0);
          this->createBlock(0, 2);
          this->createBlock(1, 2);
          this->createBlock(2, 2);
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
        // Place bomb at whichever square the character's center lies in
        if(!(this->bombs_exist[bomb_y][bomb_x])) {
          this->bombs[bomb_y][bomb_x] = Bomb(bomb_y, bomb_x, bomb_range);
          this->bombs_exist[bomb_y][bomb_x] = true;
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

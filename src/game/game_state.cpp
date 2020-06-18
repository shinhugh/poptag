#include <cstdlib>
#include "game_state.h"
#include "event_data.h"

#include <iostream> // DEBUG

#define BOARD_HEIGHT 16
#define BOARD_WIDTH 16

// ------------------------------------------------------------

static bool randomBool();

// ------------------------------------------------------------

void GameState::init() {

  // Re-generate characters
  this->characters.clear();
  this->characters_alive.clear();
  this->characters.push_back(Character(0.5, 0.5, 8, 2));
  this->characters_alive.push_back(true);

  // Re-generate blocks
  for(unsigned int y = 0; y < this->board_height; y++) {
    for(unsigned int x = 0; x < this->board_width; x++) {
      this->blocks_exist[y][x] = false;
    }
  }
  for(unsigned int x = 2; x < this->board_width; x++) {
    if(randomBool()) {
      if(randomBool()) {
        this->createBlock(0, x, breakable);
      } else {
        this->createBlock(0, x, unbreakable);
      }
    }
  }
  for(unsigned int x = 1; x < this->board_width; x++) {
    if(randomBool()) {
      if(randomBool()) {
        this->createBlock(1, x, breakable);
      } else {
        this->createBlock(1, x, unbreakable);
      }
    }
  }
  for(unsigned int y = 2; y < this->board_height; y++) {
    for(unsigned int x = 0; x < this->board_width; x++) {
      if(randomBool()) {
        if(randomBool()) {
          this->createBlock(y, x, breakable);
        } else {
          this->createBlock(y, x, unbreakable);
        }
      }
    }
  }

}

// ------------------------------------------------------------

void GameState::createBlock(unsigned int y, unsigned int x, BlockType type) {

  if(!(this->blocks_exist[y][x])) {
    this->blocks[y][x] = BoardBlock(y, x, type);
    this->blocks_exist[y][x] = true;
  }

}

// ------------------------------------------------------------

void GameState::explodeBomb(unsigned int y, unsigned int x) {

  if(!(this->bombs_exist[y][x])) {
    return;
  }

  // Get coordintes the explosion reaches
  std::vector<unsigned int> coordinates
  = this->bombs[y][x].explosionCoordinates(this->board_height,
  this->board_width);

  // Whether the bomb's explosion continues through blocks
  bool breakthrough = this->bombs[y][x].getBreakthrough();
  bool stopped[4] = {false, false, false, false};

  // Remove bomb
  this->bombs_exist[y][x] = false;

  // Iterate through all coordinates the explosion reaches
  for(unsigned int i = 0; i + 1 < coordinates.size(); i += 2) {

    // Get coordinates of this explosion
    unsigned int coor_y = coordinates.at(i);
    unsigned int coor_x = coordinates.at(i + 1);

    // Get direction of this explosion relative to the bomb
    unsigned int direction = 0;
    if(coor_x > x) {
      direction = 1;
    } else if(coor_y > y) {
      direction = 2;
    } else if(coor_x < x) {
      direction = 3;
    }

    // If explosion in this direction hasn't been stopped
    if(!stopped[direction]) {

      // Destroy block if one exists and isn't unbreakable
      if(this->blocks_exist[coor_y][coor_x]) {
        if(this->blocks[coor_y][coor_x].getType() == breakable) {
          this->blocks_exist[coor_y][coor_x] = false;
          // Stop explosion in this direction if breakthrough isn't true
          if(!breakthrough) {
            stopped[direction] = true;
          }
        }
        else {
          stopped[direction] = true;
          continue;
        }
      }

      // Kill character if one exists
      for(unsigned int j = 0; j < this->characters.size(); j++) {
        // Get coordinates of square that the character's center resides in
        unsigned int character_y
        = static_cast<unsigned int>(this->characters.at(j).getHitbox()
        ->getCenterY());
        unsigned int character_x
        = static_cast<unsigned int>(this->characters.at(j).getHitbox()
        ->getCenterX());
        if(character_y == coor_y && character_x == coor_x) {
          this->characters_alive.at(j) = false;
        }
      }

      // Detonate bomb if one exists
      if(this->bombs_exist[coor_y][coor_x]) {
        this->explodeBomb(coor_y, coor_x);
      }

      // Create explosion
      if(this->explosions_exist[coor_y][coor_x]) {
        this->explosions[coor_y][coor_x].resetTimeAge();
      } else {
        this->explosions[coor_y][coor_x] = Explosion(coor_y, coor_x);
        this->explosions_exist[coor_y][coor_x] = true;
      }

    }

  }

}

// ------------------------------------------------------------

GameState::GameState() :
board_height(BOARD_HEIGHT), board_width(BOARD_WIDTH) {

  // Allocate memory
  this->bombs = new Bomb *[this->board_height];
  this->bombs_exist = new bool *[this->board_height];
  this->explosions = new Explosion *[this->board_height];
  this->explosions_exist = new bool *[this->board_height];
  this->blocks = new BoardBlock *[this->board_height];
  this->blocks_exist = new bool *[this->board_height];
  for(unsigned int y = 0; y < this->board_height; y++) {
    this->bombs[y] = new Bomb[this->board_width];
    this->bombs_exist[y] = new bool[this->board_width];
    this->explosions[y] = new Explosion[this->board_width];
    this->explosions_exist[y] = new bool[this->board_width];
    this->blocks[y] = new BoardBlock[this->board_width];
    this->blocks_exist[y] = new bool[this->board_width];
    // Unset all flags
    for(unsigned int x = 0; x < this->board_width; x++) {
      this->bombs_exist[y][x] = false;
      this->explosions_exist[y][x] = false;
      this->blocks_exist[y][x] = false;
    }
  }

}

// ------------------------------------------------------------

GameState::GameState(const GameState& src) :
board_height(src.board_height), board_width(src.board_width),
characters(src.characters), characters_alive(src.characters_alive) {

  // Allocate memory
  this->bombs = new Bomb *[this->board_height];
  this->bombs_exist = new bool *[this->board_height];
  this->explosions = new Explosion *[this->board_height];
  this->explosions_exist = new bool *[this->board_height];
  this->blocks = new BoardBlock *[this->board_height];
  this->blocks_exist = new bool *[this->board_height];
  for(unsigned int y = 0; y < this->board_height; y++) {
    this->bombs[y] = new Bomb[this->board_width];
    this->bombs_exist[y] = new bool[this->board_width];
    this->explosions[y] = new Explosion[this->board_width];
    this->explosions_exist[y] = new bool[this->board_width];
    this->blocks[y] = new BoardBlock[this->board_width];
    this->blocks_exist[y] = new bool[this->board_width];
    // Unset all flags
    for(unsigned int x = 0; x < this->board_width; x++) {
      this->bombs_exist[y][x] = false;
      this->explosions_exist[y][x] = false;
      this->blocks_exist[y][x] = false;
    }
  }

  // Copy from source
  for(unsigned int y = 0; y < this->board_height; y++) {
    for(unsigned int x = 0; x < this->board_width; x++) {
      this->bombs[y][x] = src.bombs[y][x];
      this->bombs_exist[y][x] = src.bombs_exist[y][x];
      this->explosions[y][x] = src.explosions[y][x];
      this->explosions_exist[y][x] = src.explosions_exist[y][x];
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
    delete[] this->explosions[y];
    delete[] this->explosions_exist[y];
    delete[] this->blocks[y];
    delete[] this->blocks_exist[y];
  }
  delete[] this->bombs;
  delete[] this->bombs_exist;
  delete[] this->explosions;
  delete[] this->explosions_exist;
  delete[] this->blocks;
  delete[] this->blocks_exist;

  // Copy from source
  this->board_height = src.board_height;
  this->board_width = src.board_width;
  this->characters = src.characters;
  this->characters_alive = src.characters_alive;

  // Allocate memory
  this->bombs = new Bomb *[this->board_height];
  this->bombs_exist = new bool *[this->board_height];
  this->explosions = new Explosion *[this->board_height];
  this->explosions_exist = new bool *[this->board_height];
  this->blocks = new BoardBlock *[this->board_height];
  this->blocks_exist = new bool *[this->board_height];
  for(unsigned int y = 0; y < this->board_height; y++) {
    this->bombs[y] = new Bomb[this->board_width];
    this->bombs_exist[y] = new bool[this->board_width];
    this->explosions[y] = new Explosion[this->board_width];
    this->explosions_exist[y] = new bool[this->board_width];
    this->blocks[y] = new BoardBlock[this->board_width];
    this->blocks_exist[y] = new bool[this->board_width];
    // Unset all flags
    for(unsigned int x = 0; x < this->board_width; x++) {
      this->bombs_exist[y][x] = false;
      this->explosions_exist[y][x] = false;
      this->blocks_exist[y][x] = false;
    }
  }

  // Copy from source
  for(unsigned int y = 0; y < this->board_height; y++) {
    for(unsigned int x = 0; x < this->board_width; x++) {
      this->bombs[y][x] = src.bombs[y][x];
      this->bombs_exist[y][x] = src.bombs_exist[y][x];
      this->explosions[y][x] = src.explosions[y][x];
      this->explosions_exist[y][x] = src.explosions_exist[y][x];
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
    delete[] this->explosions[y];
    delete[] this->explosions_exist[y];
    delete[] this->blocks[y];
    delete[] this->blocks_exist[y];
  }
  delete[] this->bombs;
  delete[] this->bombs_exist;
  delete[] this->explosions;
  delete[] this->explosions_exist;
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

const Character * GameState::getCharacter(unsigned int id) const {

  if(id < this->characters.size()) {
    return &(this->characters.at(id));
  } else {
    return 0;
  }

}

// ------------------------------------------------------------

bool GameState::getCharacterAlive(unsigned int id) const {

  if(id < this->characters_alive.size()) {
    return this->characters_alive.at(id);
  } else {
    return false;
  }

}

// ------------------------------------------------------------

unsigned int GameState::getCharacterCount() const {

  return this->characters.size();

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

const Explosion * GameState::getExplosion(unsigned int y, unsigned int x) const
{

  return &(this->explosions[y][x]);

}

// ------------------------------------------------------------

bool GameState::getExplosionExist(unsigned int y, unsigned int x) const {

  return this->explosions_exist[y][x];

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

void GameState::internalUpdate(std::chrono::microseconds elapsed_time) {

  // Update positions of all characters
  for(unsigned int i = 0; i < this->characters.size(); i++) {
    this->characters.at(i).update(this, elapsed_time);
  }

  // Update all explosions
  for(unsigned int y = 0; y < this->board_height; y++) {
    for(unsigned int x = 0; x < this->board_width; x++) {
      if(this->explosions_exist[y][x]) {
        // Kill character if taking up this spot
        for(unsigned int i = 0; i < this->characters.size(); i++) {
          // Get coordinates of square that the character's center resides in
          unsigned int character_y
          = static_cast<unsigned int>(this->characters.at(i).getHitbox()
          ->getCenterY());
          unsigned int character_x
          = static_cast<unsigned int>(this->characters.at(i).getHitbox()
          ->getCenterX());
          if(character_y == y && character_x == x) {
            this->characters_alive.at(i) = false;
          }
        }
        // Update explosion time and remove if necessary
        this->explosions[y][x].update(elapsed_time);
        if(this->explosions[y][x].getTimeAge()
        >= this->explosions[y][x].getTimeDisappear()) {
          // Remove explosion
          this->explosions_exist[y][x] = false;
        }
      }
    }
  }

  // Update all bombs
  for(unsigned int y = 0; y < this->board_height; y++) {
    for(unsigned int x = 0; x < this->board_width; x++) {
      if(this->bombs_exist[y][x]) {
        this->bombs[y][x].update(elapsed_time);
        if(this->bombs[y][x].getTimeAge()
        >= this->bombs[y][x].getTimeDetonate()) {
          this->explodeBomb(y, x);
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
          this->init();
        }
      }
      break;

    // Place bomb
    case placeBomb:
      {
        // Parse event
        EventData_PlaceBomb *event_data
        = static_cast<EventData_PlaceBomb *>(packet.getData());
        if(event_data->character_id < this->characters.size()
        && this->characters_alive.at(event_data->character_id)) {
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
          bool bomb_breakthrough
          = this->characters.at(event_data->character_id).getBombBreakthrough();
          // Place bomb at whichever square the character's center lies in
          if(!(this->bombs_exist[bomb_y][bomb_x])) {
            this->bombs[bomb_y][bomb_x] = Bomb(bomb_y, bomb_x, bomb_range,
            bomb_breakthrough);
            this->bombs_exist[bomb_y][bomb_x] = true;
          }
        }
      }
      break;

    // Stop moving
    case moveStop:
      {
        EventData_MoveStop *event_data
        = static_cast<EventData_MoveStop *>(packet.getData());
        if(event_data->character_id < this->characters.size()
        && this->characters_alive.at(event_data->character_id)) {
          this->characters.at(event_data->character_id).setDirMove(stop);
        }
      }
      break;

    // Move up
    case moveUp:
      {
        EventData_MoveUp *event_data
        = static_cast<EventData_MoveUp *>(packet.getData());
        if(event_data->character_id < this->characters.size()
        && this->characters_alive.at(event_data->character_id)) {
          this->characters.at(event_data->character_id).setDirMove(up);
        }
      }
      break;

    // Move right
    case moveRight:
      {
        EventData_MoveRight *event_data
        = static_cast<EventData_MoveRight *>(packet.getData());
        if(event_data->character_id < this->characters.size()
        && this->characters_alive.at(event_data->character_id)) {
          this->characters.at(event_data->character_id).setDirMove(right);
        }
      }
      break;

    // Move down
    case moveDown:
      {
        EventData_MoveDown *event_data
        = static_cast<EventData_MoveDown *>(packet.getData());
        if(event_data->character_id < this->characters.size()
        && this->characters_alive.at(event_data->character_id)) {
          this->characters.at(event_data->character_id).setDirMove(down);
        }
      }
      break;

    // Move left
    case moveLeft:
      {
        EventData_MoveLeft *event_data
        = static_cast<EventData_MoveLeft *>(packet.getData());
        if(event_data->character_id < this->characters.size()
        && this->characters_alive.at(event_data->character_id)) {
          this->characters.at(event_data->character_id).setDirMove(left);
        }
      }
      break;

  }

}

// ------------------------------------------------------------

static bool randomBool() {

  return (rand() % 2 == 0);

}

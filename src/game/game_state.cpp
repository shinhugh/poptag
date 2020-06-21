#include <queue>
#include <cstdlib>
#include "game_state.h"
#include "game_common.h"
#include "event_data.h"

#define BOARD_HEIGHT 15
#define BOARD_WIDTH 15

// ------------------------------------------------------------

static bool randomBool();

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
      }
    }
  }

  // Detonate bombs
  this->detonateBombs();

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

void GameState::init() {

  // Remove bombs and explosions
  for(unsigned int y = 0; y < this->board_height; y++) {
    for(unsigned int x = 0; x < this->board_width; x++) {
      this->bombs_exist[y][x] = false;
      this->explosions_exist[y][x] = false;
    }
  }

  // Re-generate characters
  this->characters.clear();
  this->characters_alive.clear();
  this->characters.push_back(Character(0.5, 0.5, 4, 2));
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

void GameState::detonateBombs() {

  // List of coordinates of blocks to destroy
  std::vector<unsigned int> destroyed_blocks_coordinates;

  // List of coordinates at which a bomb exploded on this update
  std::vector<unsigned int> explosion_center_coordinates;

  // Queue of coordinates at which a bomb exploded on this update
  std::queue<unsigned int> bomb_coordinates;

  // Find bombs that are due for detonation
  for(unsigned int y_bomb = 0; y_bomb < this->board_height; y_bomb++) {
    for(unsigned int x_bomb = 0; x_bomb < this->board_width; x_bomb++) {
      if(this->bombs_exist[y_bomb][x_bomb]
      && (this->bombs[y_bomb][x_bomb].getTimeAge()
      >= this->bombs[y_bomb][x_bomb].getTimeDetonate())) {
        bomb_coordinates.push(y_bomb);
        bomb_coordinates.push(x_bomb);
      }
    }
  }

  // Handle each bomb due for detonation
  while(!bomb_coordinates.empty()) {

    // Coordinates of bomb to detonate
    unsigned int y_bomb = bomb_coordinates.front();
    bomb_coordinates.pop();
    unsigned int x_bomb = bomb_coordinates.front();
    bomb_coordinates.pop();

    // If bomb hasn't been handled yet
    if(this->bombs_exist[y_bomb][x_bomb]) {

      // Get coordinates that this bomb's explosion would hit
      std::vector<unsigned int> explosion_coordinates
      = this->bombs[y_bomb][x_bomb].explosionCoordinates(this->board_height,
      this->board_width);

      // Whether explosion is stopped by a block, for each direction
      bool direction_stopped[4] = {false, false, false, false};

      // Handle each explosion unit
      for(unsigned int i = 0; i + 1 < explosion_coordinates.size(); i += 2) {

        // Coordinates of explosion unit
        unsigned int y_expl = explosion_coordinates.at(i);
        unsigned int x_expl = explosion_coordinates.at(i + 1);

        // Get direction of explosion relative to source bomb
        Direction explosion_direction = stop;
        if(y_expl < y_bomb) {
          explosion_direction = up;
        } else if(x_expl > x_bomb) {
          explosion_direction = right;
        } else if(y_expl > y_bomb) {
          explosion_direction = down;
        } else if(x_expl < x_bomb) {
          explosion_direction = left;
        }
        unsigned int direction_index;
        switch(explosion_direction) {
          case up:
            {
              direction_index = 0;
            }
            break;
          case right:
            {
              direction_index = 1;
            }
            break;
          case down:
            {
              direction_index = 2;
            }
            break;
          case left:
            {
              direction_index = 3;
            }
            break;
          default:
            {
              direction_index = 0;
            }
            break;
        }

        // If explosion in this direction hasn't been stopped
        if(!direction_stopped[direction_index]) {

          // Handle interaction with block if one exists
          if(this->blocks_exist[y_expl][x_expl]) {
            // Unbreakable block
            if(this->blocks[y_expl][x_expl].getType() == unbreakable) {
              direction_stopped[direction_index] = true;
            }
            // Breakable block
            else {
              if(!this->bombs[y_bomb][x_bomb].getBreakthrough()) {
                direction_stopped[direction_index] = true;
              }
              destroyed_blocks_coordinates.push_back(y_expl);
              destroyed_blocks_coordinates.push_back(x_expl);
            }
          }

          // Handle interaction with another bomb if one exists
          if(this->bombs_exist[y_expl][x_expl]
          && !(y_bomb == y_expl && x_bomb == x_expl)) {
            bomb_coordinates.push(y_expl);
            bomb_coordinates.push(x_expl);
          }

          // Create explosion instance
          this->explosions[y_expl][x_expl] = Explosion(y_expl, x_expl,
          explosion_direction);
          this->explosions_exist[y_expl][x_expl] = true;

        }

      }

      // Remove detonated bomb and add to list of explosion centers
      this->bombs_exist[y_bomb][x_bomb] = false;
      explosion_center_coordinates.push_back(y_bomb);
      explosion_center_coordinates.push_back(x_bomb);

    }

  }

  // Update explosion centers
  for(unsigned int i = 0; i + 1 < explosion_center_coordinates.size(); i += 2) {
    unsigned int y = explosion_center_coordinates.at(i);
    unsigned int x = explosion_center_coordinates.at(i + 1);
    this->explosions[y][x] = Explosion(y, x, stop);
  }

  // Actually destroy blocks
  for(unsigned int i = 0; i + 1 < destroyed_blocks_coordinates.size(); i += 2) {
    this->blocks_exist[destroyed_blocks_coordinates.at(i)]
    [destroyed_blocks_coordinates.at(i + 1)] = false;
  }

}

// ------------------------------------------------------------

static bool randomBool() {

  return (rand() % 2 == 0);

}

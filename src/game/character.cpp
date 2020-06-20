#include "character.h"
#include "game_state.h"

#define CHARACTER_WIDTH 1.0f

// ------------------------------------------------------------

Character::Character(float y, float x, float speed, unsigned int bomb_range) :
hitbox(y, x, CHARACTER_WIDTH, CHARACTER_WIDTH), speed(speed), dir_move(stop),
dir_face(down), bomb_range(bomb_range), bomb_breakthrough(false) {}

// ------------------------------------------------------------

const Hitbox * Character::getHitbox() const {

  return &(this->hitbox);

}

// ------------------------------------------------------------

Direction Character::getDirFace() const {

  return this->dir_face;

}

// ------------------------------------------------------------

Direction Character::getDirMove() const {

  return this->dir_move;

}

// ------------------------------------------------------------

unsigned int Character::getBombRange() const {

  return this->bomb_range;

}

// ------------------------------------------------------------

bool Character::getBombBreakthrough() const {

  return this->bomb_breakthrough;

}

// ------------------------------------------------------------

void Character::setBombBreakthrough(bool bomb_breakthrough) {

  this->bomb_breakthrough = bomb_breakthrough;

}

// ------------------------------------------------------------

void Character::setDirMove(Direction dir_move) {

  this->dir_move = dir_move;

}

// ------------------------------------------------------------

void Character::update(const void * parent_state,
std::chrono::microseconds elapsed_time) {

  // Game state instance that contains this character
  const GameState * game_state = static_cast<const GameState *>(parent_state);

  // Distance traveled
  float distance = elapsed_time.count() * this->speed / 1000000;

  // Hitboxes that may collide with this character
  std::vector<const Hitbox *> blocks;
  for(unsigned int y = 0; y < game_state->getBoardHeight(); y++) {
    for(unsigned int x = 0; x < game_state->getBoardWidth(); x++) {
      if(game_state->getBlockExist(y, x)) {
        blocks.push_back(game_state->getBlock(y, x)->getHitbox());
      }
      if(game_state->getBombExist(y, x)) {
        blocks.push_back(game_state->getBomb(y, x)->getHitbox());
      }
    }
  }

  // Square on board the character's center resides in
  unsigned int reside_y
  = static_cast<unsigned int>(this->hitbox.getCenterY());
  unsigned int reside_x
  = static_cast<unsigned int>(this->hitbox.getCenterX());

  switch(this->dir_move) {
    // Move up
    case up:
      {
        // Move
        bool canMove = this->hitbox.moveUp(distance, &blocks,
        game_state->getBoardHeight(), game_state->getBoardWidth());
        // Snap to center of square if path is open
        if(!canMove && (reside_y > 0)
        && !(game_state->getBlockExist(reside_y - 1, reside_x))
        && !(game_state->getBombExist(reside_y - 1, reside_x))) {
          this->hitbox.setCenterX(
          static_cast<unsigned int>(this->hitbox.getCenterX())
          + (CHARACTER_WIDTH / 2));
        }
      }
      break;
    // Move right
    case right:
      {
        // Move
        bool canMove = this->hitbox.moveRight(distance, &blocks,
        game_state->getBoardHeight(), game_state->getBoardWidth());
        // Snap to center of square if path is open
        if(!canMove && (reside_x < game_state->getBoardWidth() - 1)
        && !(game_state->getBlockExist(reside_y, reside_x + 1))
        && !(game_state->getBombExist(reside_y, reside_x + 1))) {
          this->hitbox.setCenterY(
          static_cast<unsigned int>(this->hitbox.getCenterY())
          + (CHARACTER_WIDTH / 2));
        }
      }
      break;
    // Move down
    case down:
      {
        // Move
        bool canMove = this->hitbox.moveDown(distance, &blocks,
        game_state->getBoardHeight(), game_state->getBoardWidth());
        // Snap to center of square if path is open
        if(!canMove && (reside_y < game_state->getBoardHeight() - 1)
        && !(game_state->getBlockExist(reside_y + 1, reside_x))
        && !(game_state->getBombExist(reside_y + 1, reside_x))) {
          this->hitbox.setCenterX(
          static_cast<unsigned int>(this->hitbox.getCenterX())
          + (CHARACTER_WIDTH / 2));
        }
      }
      break;
    // Move left
    case left:
      {
        // Move
        bool canMove = this->hitbox.moveLeft(distance, &blocks,
        game_state->getBoardHeight(), game_state->getBoardWidth());
        // Snap to center of square if path is open
        if(!canMove && (reside_x > 0)
        && !(game_state->getBlockExist(reside_y, reside_x - 1))
        && !(game_state->getBombExist(reside_y, reside_x - 1))) {
          this->hitbox.setCenterY(
          static_cast<unsigned int>(this->hitbox.getCenterY())
          + (CHARACTER_WIDTH / 2));
        }
      }
      break;
    default:
      break;
  }

}

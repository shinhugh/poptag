#include "character.h"

#define CHARACTER_WIDTH 1.0f

// ------------------------------------------------------------

Character::Character(float y, float x, float speed, unsigned int bomb_range) :
hitbox(y, x, CHARACTER_WIDTH, CHARACTER_WIDTH), speed(speed), dir_move(stop),
dir_face(down), bomb_range(bomb_range) {}

// ------------------------------------------------------------

const Hitbox * Character::getHitbox() const {

  return &(this->hitbox);

}

// ------------------------------------------------------------

Direction Character::getDirFace() const {

  return this->dir_face;

}

// ------------------------------------------------------------

unsigned int Character::getBombRange() const {

  return this->bomb_range;

}

// ------------------------------------------------------------

void Character::setDirMove(Direction dir_move) {

  this->dir_move = dir_move;

}

// ------------------------------------------------------------

void Character::update(const Board *board,
std::chrono::microseconds elapsed_time) {

  // Distance traveled
  float distance = elapsed_time.count() * this->speed / 1000000;

  // Blocks on board
  std::vector<const Hitbox *> blocks;
  for(unsigned int y = 0; y < board->getHeight(); y++) {
    for(unsigned int x = 0; x < board->getWidth(); x++) {
      if(board->getBlockExist(y, x)) {
        blocks.push_back(board->getBlock(y, x)->getHitbox());
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
        board->getHeight(), board->getWidth());
        // Snap to center of square if path is open
        if(!canMove && (reside_y > 0)
        && !(board->getBlockExist(reside_y - 1, reside_x))) {
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
        board->getHeight(), board->getWidth());
        // Snap to center of square if path is open
        if(!canMove && (reside_x < board->getWidth() - 1)
        && !(board->getBlockExist(reside_y, reside_x + 1))) {
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
        board->getHeight(), board->getWidth());
        // Snap to center of square if path is open
        if(!canMove && (reside_y < board->getHeight() - 1)
        && !(board->getBlockExist(reside_y + 1, reside_x))) {
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
        board->getHeight(), board->getWidth());
        // Snap to center of square if path is open
        if(!canMove && (reside_x > 0)
        && !(board->getBlockExist(reside_y, reside_x - 1))) {
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

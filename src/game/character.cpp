#include "character.h"

// ------------------------------------------------------------

Character::Character(float y, float x, float speed, unsigned int bomb_range) :
hitbox(y, x, 1, 1), speed(speed), dir_move(stop), dir_face(down),
bomb_range(bomb_range) {}

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

  switch(this->dir_move) {
    // Move up
    case up:
      {
        this->hitbox.moveUp(distance, &blocks, board->getHeight(),
        board->getWidth());
      }
      break;
    // Move right
    case right:
      {
        this->hitbox.moveRight(distance, &blocks, board->getHeight(),
        board->getWidth());
      }
      break;
    // Move down
    case down:
      {
        this->hitbox.moveDown(distance, &blocks, board->getHeight(),
        board->getWidth());
      }
      break;
    // Move left
    case left:
      {
        this->hitbox.moveLeft(distance, &blocks, board->getHeight(),
        board->getWidth());
      }
      break;
    default:
      break;
  }

}

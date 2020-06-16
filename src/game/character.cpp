#include "character.h"

// ------------------------------------------------------------

Character::Character(float y, float x, float speed, unsigned int bomb_range) :
speed(speed), dir_move(stop), dir_face(down),
bomb_range(bomb_range) {

  hitbox = Hitbox(y, x, 1, 1);

}

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

void Character::update(Board *board, std::chrono::microseconds elapsed_time) {

  // Distance traveled
  float distance = elapsed_time.count() * this->speed / 1000000;

  switch(this->dir_move) {
    // Move up
    case up:
      {
        this->hitbox.moveUp(distance, , board->getHeight(), board->getWidth());
      }
      break;
    // Move right
    case right:
      {
        this->moveRight(board, distance);
      }
      break;
    // Move down
    case down:
      {
        this->moveDown(board, distance);
      }
      break;
    // Move left
    case left:
      {
        this->moveLeft(board, distance);
      }
      break;
    default:
      break;
  }

}

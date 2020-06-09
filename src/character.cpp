#include "character.h"

// ------------------------------------------------------------

Character::Character(float y, float x, float speed) :
y(y), x(x), speed(speed), dir_move(none), dir_face(down) {

}

// ------------------------------------------------------------

void Character::tick() {

  switch(this->dir_move) {
    case up:
      {
        this->dir_face = this->dir_move;
      }
      break;
    case right:
      {
        this->dir_face = this->dir_move;
      }
      break;
    case down:
      {
        this->dir_face = this->dir_move;
      }
      break;
    case left:
      {
        this->dir_face = this->dir_move;
      }
      break;
  }

}

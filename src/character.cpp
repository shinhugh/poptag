#include "character.h"

#define CHARACTER_WIDTH 0.8

// ------------------------------------------------------------

Character::Character(float y, float x, float speed) :
y(y), x(x), speed(speed), dir_move(none), dir_face(down) {

}

// ------------------------------------------------------------

void Character::tick() {

  switch(this->dir_move) {
    // Move up
    case up:
      {
        this->moveUp();
      }
      break;
    // Move right
    case right:
      {
        this->moveRight();
      }
      break;
    // Move down
    case down:
      {
        this->moveDown();
      }
      break;
    // Move left
    case left:
      {
        this->moveLeft();
      }
      break;
  }

}

// ------------------------------------------------------------

void Character::moveUp() {

  // Face up
  this->dir_face = up;

  // Allow move if character's outer width doesn't overrun a block boundary

  // Top left and top right corners of character's outer box
  float top_left[2];
  float top_right[2];
  top_left[0] = this->y - 0.5;
  top_left[1] = this->x - 0.5;
  top_right[0] = this->y - 0.5;
  top_right[1] = this->x + 0.5;

  // Locations on the board the character's corners will occupy should it move
  // freely
  unsigned int top_left_dest[2];
  unsigned int top_right_dest[2];
  top_left_dest[0] = static_cast<unsigned int>(top_left[0] > this->speed
  ? top_left[0] - this->speed : 0);
  top_left_dest[1] = static_cast<unsigned int>(top_left[1]);
  top_right_dest[0] = static_cast<unsigned int>(top_right[0] > this->speed
  ? top_right[0] - this->speed : 0);
  top_right_dest[1] = static_cast<unsigned int>(top_right[1]);

  // Check board terrain
  if(this->board->terrain[top_left_dest[0]][top_left_dest[1]] == ground
  && this->board->terrain[top_right_dest[0]][top_right_dest[1]] == ground) {
    // Can move up freely
    this->y -= this->speed;
  } else {
    // Can't move up freely; vertically center character within current space
    this->y = static_cast<unsigned int>(this->y) + 0.5;
  }

}

// ------------------------------------------------------------

void Character::moveRight() {

  // Face right
  this->dir_face = right;

  // Allow move if character's outer width doesn't overrun a block boundary

  // Top right and bottom right corners of character's outer box
  float top_right[2];
  float bottom_right[2];
  top_right[0] = this->y - 0.5;
  top_right[1] = this->x + 0.5;
  bottom_right[0] = this->y + 0.5;
  bottom_right[1] = this->x + 0.5;

  // Locations on the board the character's corners will occupy should it move
  // freely
  unsigned int top_right_dest[2];
  unsigned int bottom_right_dest[2];
  // TODO

  // Check board terrain
  // TODO

  /*

  unsigned int floor = static_cast<unsigned int>(this->y);

  if(((this->y - floor) > (CHARACTER_WIDTH / 2.0))
  && ((this->y - floor) < (1 - (CHARACTER_WIDTH / 2.0)))) {
    return true;
  } else {
    return false;
  }

  */

}

// ------------------------------------------------------------

void Character::moveDown() {

  // TODO

}

// ------------------------------------------------------------

void Character::moveLeft() {

  // TODO

}

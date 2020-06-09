#include "character.h"

#define CHARACTER_WIDTH 0.8

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

  // Movement should not take the character off the board
  if(top_left[0] <= this->speed) {
    // Place farthest up without going off the board
    this->y = 0.5;
    return;
  }

  // Locations on the board the character's corners will occupy should it move
  // freely
  unsigned int top_left_dest[2];
  unsigned int top_right_dest[2];
  top_left_dest[0] = static_cast<unsigned int>(top_left[0] - this->speed);
  top_left_dest[1] = static_cast<unsigned int>(top_left[1]);
  top_right_dest[0] = static_cast<unsigned int>(top_right[0] - this->speed);
  top_right_dest[1] = static_cast<unsigned int>(top_right[1]);

  // Check board terrain
  if(this->board->getTerrain(top_left_dest[0], top_left_dest[1]) == ground
  && this->board->getTerrain(top_right_dest[0], top_right_dest[1]) == ground) {
    // Can move freely
    this->y -= this->speed;
  } else {
    // Can't move freely; vertically center character within current space
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

  // Movement should not take the character off the board
  if(top_right[1] + this->speed >= this->board->getWidth()) {
    // Place farthest right without going off the board
    this->x = this->board->getWidth() - 0.5;
    return;
  }

  // Locations on the board the character's corners will occupy should it move
  // freely
  unsigned int top_right_dest[2];
  unsigned int bottom_right_dest[2];
  top_right_dest[0] = static_cast<unsigned int>(top_right[0]);
  top_right_dest[1] = static_cast<unsigned int>(top_right[1] + this->speed);
  bottom_right_dest[0] = static_cast<unsigned int>(bottom_right[0]);
  bottom_right_dest[1] = static_cast<unsigned int>(bottom_right[1]
  + this->speed);

  // Check board terrain
  if(this->board->getTerrain(top_right_dest[0], top_right_dest[1]) == ground
  && this->board->getTerrain(bottom_right_dest[0], bottom_right_dest[1])
  == ground) {
    // Can move freely
    this->x += this->speed;
  } else {
    // Can't move freely; horizontally center character within current space
    this->x = static_cast<unsigned int>(this->x) + 0.5;
  }

}

// ------------------------------------------------------------

void Character::moveDown() {

  // Face down
  this->dir_face = down;

  // Allow move if character's outer width doesn't overrun a block boundary

  // Top right and bottom right corners of character's outer box
  float bottom_right[2];
  float bottom_left[2];
  bottom_right[0] = this->y + 0.5;
  bottom_right[1] = this->x + 0.5;
  bottom_left[0] = this->y + 0.5;
  bottom_left[1] = this->x - 0.5;

  // Movement should not take the character off the board
  if(bottom_right[0] + this->speed >= this->board->getHeight()) {
    // Place farthest down without going off the board
    this->y = this->board->getHeight() - 0.5;
    return;
  }

  // Locations on the board the character's corners will occupy should it move
  // freely
  unsigned int bottom_right_dest[2];
  unsigned int bottom_left_dest[2];
  bottom_right_dest[0] = static_cast<unsigned int>(bottom_right[0]
  + this->speed);
  bottom_right_dest[1] = static_cast<unsigned int>(bottom_right[1]);
  bottom_left_dest[0] = static_cast<unsigned int>(bottom_left[0]
  + this->speed);
  bottom_left_dest[1] = static_cast<unsigned int>(bottom_left[1]);

  // Check board terrain
  if(this->board->getTerrain(bottom_right_dest[0], bottom_right_dest[1])
  == ground
  && this->board->getTerrain(bottom_left_dest[0], bottom_left_dest[1])
  == ground) {
    // Can move freely
    this->y += this->speed;
  } else {
    // Can't move freely; vertically center character within current space
    this->y = static_cast<unsigned int>(this->y) + 0.5;
  }

}

// ------------------------------------------------------------

void Character::moveLeft() {

  // Face left
  this->dir_face = left;

  // Allow move if character's outer width doesn't overrun a block boundary

  // Top right and bottom right corners of character's outer box
  float bottom_left[2];
  float top_left[2];
  bottom_left[0] = this->y + 0.5;
  bottom_left[1] = this->x - 0.5;
  top_left[0] = this->y - 0.5;
  top_left[1] = this->x - 0.5;

  // Movement should not take the character off the board
  if(bottom_left[1] <= this->speed) {
    // Place farthest left without going off the board
    this->x = 0.5;
    return;
  }

  // Locations on the board the character's corners will occupy should it move
  // freely
  unsigned int bottom_left_dest[2];
  unsigned int top_left_dest[2];
  bottom_left_dest[0] = static_cast<unsigned int>(bottom_left[0]);
  bottom_left_dest[1] = static_cast<unsigned int>(bottom_left[1] - this->speed);
  top_left_dest[0] = static_cast<unsigned int>(top_left[0]);
  top_left_dest[1] = static_cast<unsigned int>(top_left[1] - this->speed);

  // Check board terrain
  if(this->board->getTerrain(bottom_left_dest[0], bottom_left_dest[1]) == ground
  && this->board->getTerrain(top_left_dest[0], top_left_dest[1]) == ground) {
    // Can move freely
    this->x -= this->speed;
  } else {
    // Can't move freely; horizontally center character within current space
    this->x = static_cast<unsigned int>(this->x) + 0.5;
  }

}

// ------------------------------------------------------------

Character::Character(float y, float x, float speed) :
y(y), x(x), speed(speed), dir_move(none), dir_face(down) {}

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
    default:
      break;
  }

}

// ------------------------------------------------------------

void Character::setDirMove(Direction dir_move) {

  this->dir_move = dir_move;

}

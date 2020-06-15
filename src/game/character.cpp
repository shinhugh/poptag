#include "character.h"

// ------------------------------------------------------------

void Character::moveUp(Board *board) {

  // Face up
  this->dir_face = up;

  // Allow move if character's outer width doesn't overrun a block boundary

  // Top left and top right corners of character's outer box
  float top_left[2];
  float top_right[2];
  top_left[0] = this->y > 0.5 ? this->y - 0.5 : 0;
  top_left[1] = this->x > 0.5 ? this->x - 0.5 : 0;
  top_right[0] = this->y > 0.5 ? this->y - 0.5 : 0;
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

  // Correct potential errors
  if(top_right_dest[1] >= board->getWidth()) {
    top_right_dest[1] = board->getWidth() - 1;
  }

  // Check board terrain
  if(board->getTerrain(top_left_dest[0], top_left_dest[1]) == ground
  && board->getTerrain(top_right_dest[0], top_right_dest[1]) == ground) {
    // Can move freely
    this->y -= this->speed;
  } else {
    // Can't move freely; vertically center character within current space
    this->y = static_cast<unsigned int>(this->y) + 0.5;
  }

}

// ------------------------------------------------------------

void Character::moveRight(Board *board) {

  // Face right
  this->dir_face = right;

  // Allow move if character's outer width doesn't overrun a block boundary

  // Top right and bottom right corners of character's outer box
  float top_right[2];
  float bottom_right[2];
  top_right[0] = this->y > 0.5 ? this->y - 0.5 : 0;
  top_right[1] = this->x + 0.5;
  bottom_right[0] = this->y + 0.5;
  bottom_right[1] = this->x + 0.5;

  // Movement should not take the character off the board
  if(top_right[1] + this->speed >= board->getWidth()) {
    // Place farthest right without going off the board
    this->x = board->getWidth() - 0.5;
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

  // Correct potential errors
  if(bottom_right_dest[0] >= board->getHeight()) {
    bottom_right_dest[0] = board->getHeight() - 1;
  }

  // Check board terrain
  if(board->getTerrain(top_right_dest[0], top_right_dest[1]) == ground
  && board->getTerrain(bottom_right_dest[0], bottom_right_dest[1])
  == ground) {
    // Can move freely
    this->x += this->speed;
  } else {
    // Can't move freely; horizontally center character within current space
    this->x = static_cast<unsigned int>(this->x) + 0.5;
  }

}

// ------------------------------------------------------------

void Character::moveDown(Board *board) {

  // Face down
  this->dir_face = down;

  // Allow move if character's outer width doesn't overrun a block boundary

  // Top right and bottom right corners of character's outer box
  float bottom_right[2];
  float bottom_left[2];
  bottom_right[0] = this->y + 0.5;
  bottom_right[1] = this->x + 0.5;
  bottom_left[0] = this->y + 0.5;
  bottom_left[1] = this->x > 0.5 ? this->x - 0.5 : 0;

  // Movement should not take the character off the board
  if(bottom_right[0] + this->speed >= board->getHeight()) {
    // Place farthest down without going off the board
    this->y = board->getHeight() - 0.5;
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

  // Correct potential errors
  if(bottom_right_dest[1] >= board->getWidth()) {
    bottom_right_dest[1] = board->getWidth() - 1;
  }

  // Check board terrain
  if(board->getTerrain(bottom_right_dest[0], bottom_right_dest[1])
  == ground
  && board->getTerrain(bottom_left_dest[0], bottom_left_dest[1])
  == ground) {
    // Can move freely
    this->y += this->speed;
  } else {
    // Can't move freely; vertically center character within current space
    this->y = static_cast<unsigned int>(this->y) + 0.5;
  }

}

// ------------------------------------------------------------

void Character::moveLeft(Board *board) {

  // Face left
  this->dir_face = left;

  // Allow move if character's outer width doesn't overrun a block boundary

  // Top right and bottom right corners of character's outer box
  float bottom_left[2];
  float top_left[2];
  bottom_left[0] = this->y + 0.5;
  bottom_left[1] = this->x > 0.5 ? this->x - 0.5 : 0;
  top_left[0] = this->y > 0.5 ? this->y - 0.5 : 0;
  top_left[1] = this->x > 0.5 ? this->x - 0.5 : 0;

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

  // Correct potential errors
  if(bottom_left_dest[0] >= board->getHeight()) {
    bottom_left_dest[0] = board->getHeight() - 1;
  }

  // Check board terrain
  if(board->getTerrain(bottom_left_dest[0], bottom_left_dest[1]) == ground
  && board->getTerrain(top_left_dest[0], top_left_dest[1]) == ground) {
    // Can move freely
    this->x -= this->speed;
  } else {
    // Can't move freely; horizontally center character within current space
    this->x = static_cast<unsigned int>(this->x) + 0.5;
  }

}

// ------------------------------------------------------------

Character::Character(float y, float x, float speed, unsigned int bomb_range) :
y(y), x(x), speed(speed), dir_move(stop), dir_face(down),
bomb_range(bomb_range) {}

// ------------------------------------------------------------

float Character::getY() const {

  return this->y;

}

// ------------------------------------------------------------

float Character::getX() const {

  return this->x;

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

void Character::tick(Board *board, std::chrono::microseconds elapsed_time) {

  switch(this->dir_move) {
    // Move up
    case up:
      {
        this->moveUp(board);
      }
      break;
    // Move right
    case right:
      {
        this->moveRight(board);
      }
      break;
    // Move down
    case down:
      {
        this->moveDown(board);
      }
      break;
    // Move left
    case left:
      {
        this->moveLeft(board);
      }
      break;
    default:
      break;
  }

}

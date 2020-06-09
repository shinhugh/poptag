// In-game character

#ifndef CHARACTER_H
#define CHARACTER_H

#include "board.h"

// ------------------------------------------------------------

enum Direction {none, up, right, down, left};

// ------------------------------------------------------------

class Character {

private:
  Board *board;
  float y, x;
  float speed;
  enum Direction dir_move;
  enum Direction dir_face;
  void moveUp();
  void moveRight();
  void moveDown();
  void moveLeft();

public:
  Character(float, float, float);
  void tick();
  void setDirMove(Direction);

};

// ------------------------------------------------------------

#endif

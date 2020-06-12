// In-game character

#ifndef CHARACTER_H
#define CHARACTER_H

#include "board.h"

// ------------------------------------------------------------

enum Direction {stop, up, right, down, left};

// ------------------------------------------------------------

class Character {

private:
  float y, x;
  float speed;
  enum Direction dir_move;
  enum Direction dir_face;
  Board *board;
  unsigned int bomb_range;
  void moveUp();
  void moveRight();
  void moveDown();
  void moveLeft();

public:
  Character(float, float, float, Board *, unsigned int);
  float getY();
  float getX();
  Direction getDirFace();
  unsigned int getBombRange();
  void setDirMove(Direction);
  void tick();

};

// ------------------------------------------------------------

#endif

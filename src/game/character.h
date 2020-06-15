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
  unsigned int bomb_range;
  void moveUp(Board *);
  void moveRight(Board *);
  void moveDown(Board *);
  void moveLeft(Board *);

public:
  Character(float, float, float, unsigned int);
  float getY() const;
  float getX() const;
  Direction getDirFace() const;
  unsigned int getBombRange() const;
  void setDirMove(Direction);
  void tick(Board *, std::chrono::microseconds);

};

// ------------------------------------------------------------

#endif

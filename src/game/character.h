// In-game character

#ifndef CHARACTER_H
#define CHARACTER_H

#include <chrono>
#include "hitbox.h"
#include "board.h"
#include "bomb.h"

// ------------------------------------------------------------

enum Direction {stop, up, right, down, left};

// ------------------------------------------------------------

class Character {

private:
  Hitbox hitbox;
  float speed;
  enum Direction dir_move;
  enum Direction dir_face;
  unsigned int bomb_range;

public:
  Character(float, float, float, unsigned int);
  const Hitbox * getHitbox() const;
  Direction getDirFace() const;
  void setDirMove(Direction);
  unsigned int getBombRange() const;
  void update(Board *, std::chrono::microseconds);

};

// ------------------------------------------------------------

#endif

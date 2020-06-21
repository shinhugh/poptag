// In-game character

#ifndef CHARACTER_H
#define CHARACTER_H

#include <chrono>
#include "hitbox.h"
#include "game_common.h"

// ------------------------------------------------------------

class Character {

private:

  Hitbox hitbox;
  float speed;
  enum Direction dir_move;
  enum Direction dir_face;
  unsigned int bomb_range;
  bool bomb_breakthrough;

public:

  Character(float, float, float, unsigned int);
  const Hitbox * getHitbox() const;
  Direction getDirFace() const;
  Direction getDirMove() const;
  void setDirMove(Direction);
  unsigned int getBombRange() const;
  bool getBombBreakthrough() const;
  void setBombBreakthrough(bool);
  void update(const void *, std::chrono::microseconds);

};

// ------------------------------------------------------------

#endif

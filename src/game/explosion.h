// Explosion of a bomb

#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <chrono>
#include "hitbox.h"
#include "game_common.h"

// ------------------------------------------------------------

class Explosion {

private:
  Hitbox hitbox;
  std::chrono::milliseconds time_disappear;
  std::chrono::microseconds time_age;
  Direction direction;

public:
  Explosion();
  Explosion(unsigned int, unsigned int, Direction);
  const Hitbox * getHitbox() const;
  std::chrono::milliseconds getTimeDisappear() const;
  std::chrono::microseconds getTimeAge() const;
  void resetTimeAge();
  Direction getDirection() const;
  void setDirection(Direction);
  void update(std::chrono::microseconds);

};

// ------------------------------------------------------------

#endif

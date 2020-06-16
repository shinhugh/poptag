// A bomb that detonates after a time interval

#ifndef BOMB_H
#define BOMB_H

#include <chrono>
#include "hitbox.h"

// ------------------------------------------------------------

class Bomb {

private:
  Hitbox hitbox;
  std::chrono::milliseconds time_detonate;
  std::chrono::microseconds time_age;
  unsigned int range;

public:
  Bomb();
  Bomb(unsigned int, unsigned int, unsigned int);
  const Hitbox * getHitbox() const;
  std::chrono::milliseconds getTimeDetonate() const;
  std::chrono::microseconds getTimeAge() const;
  unsigned int getRange() const;
  void update(std::chrono::microseconds elapsed_time);

};

// ------------------------------------------------------------

#endif

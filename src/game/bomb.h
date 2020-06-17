// A bomb that detonates after a time interval

#ifndef BOMB_H
#define BOMB_H

#include <chrono>
#include <vector>
#include "hitbox.h"

// ------------------------------------------------------------

class Bomb {

private:
  Hitbox hitbox;
  std::chrono::milliseconds time_detonate;
  std::chrono::microseconds time_age;
  unsigned int range;
  bool breakthrough;

public:
  Bomb();
  Bomb(unsigned int, unsigned int, unsigned int, bool);
  const Hitbox * getHitbox() const;
  std::chrono::milliseconds getTimeDetonate() const;
  std::chrono::microseconds getTimeAge() const;
  unsigned int getRange() const;
  bool getBreakthrough() const;
  std::vector<unsigned int> explosionCoordinates(unsigned int, unsigned int)
  const;
  void update(std::chrono::microseconds elapsed_time);

};

// ------------------------------------------------------------

#endif

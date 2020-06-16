// A bomb that detonates after a time interval

#ifndef BOMB_H
#define BOMB_H

#include <chrono>
#include "board.h"

#define TICK_DETONATE 2000

// ------------------------------------------------------------

class Bomb {

private:
  unsigned int y, x;
  std::chrono::milliseconds time_detonate;
  std::chrono::microseconds time_age;
  unsigned int range;

public:
  Bomb(unsigned int, unsigned int, std::chrono::milliseconds, unsigned int);
  unsigned int getY() const;
  unsigned int getX() const;
  std::chrono::milliseconds getTimeDetonate() const;
  std::chrono::microseconds getTimeAge() const;
  unsigned int getRange() const;
  void update(std::chrono::microseconds elapsed_time);

};

// ------------------------------------------------------------

#endif

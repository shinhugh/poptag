// A bomb that detonates after a time interval

#ifndef BOMB_H
#define BOMB_H

#include "board.h"

#define TICK_DETONATE 2000

// ------------------------------------------------------------

class Bomb {

private:
  unsigned int y, x;
  unsigned int tick_detonate;
  unsigned int tick_age;
  unsigned int range;

public:
  Bomb(unsigned int, unsigned int, unsigned int, unsigned int);
  unsigned int getY() const;
  unsigned int getX() const;
  unsigned int getTickDetonate() const;
  unsigned int getTickAge() const;
  unsigned int getRange() const;
  void setRange(unsigned int);
  void tick(std::chrono::microseconds elapsed_time);

};

// ------------------------------------------------------------

#endif

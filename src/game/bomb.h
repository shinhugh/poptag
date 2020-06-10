// A bomb that detonates after a time interval

#ifndef BOMB_H
#define BOMB_H

#include "board.h"

// ------------------------------------------------------------

class Bomb {

private:
  unsigned int y, x;
  unsigned int tick_detonate;
  unsigned int tick_age;
  unsigned int range;

public:
  Bomb(unsigned int, unsigned int, unsigned int, unsigned int);
  unsigned int getY();
  unsigned int getX();
  unsigned int getTickDetonate();
  unsigned int getTickAge();
  unsigned int getRange();
  void setRange(unsigned int);
  void tick();

};

// ------------------------------------------------------------

#endif

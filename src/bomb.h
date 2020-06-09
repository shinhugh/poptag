// A bomb that detonates after a time interval

#ifndef BOMB_H
#define BOMB_H

#include "board.h"

// ------------------------------------------------------------

class Bomb {

private:
  Board *board;
  unsigned int y, x;
  unsigned int tick_detonate;
  unsigned int tick_age;
  unsigned int range;

public:
  Bomb(unsigned int, unsigned int);
  void tick();
  unsigned int getTickDetonate();
  unsigned int getTickAge();
  unsigned int getRange();
  void setRange(unsigned int);

};

// ------------------------------------------------------------

#endif

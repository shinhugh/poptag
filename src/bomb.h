// A bomb that detonates after a time interval

#ifndef BOMB_H
#define BOMB_H

// ------------------------------------------------------------

class Bomb {

private:
  unsigned int tick_detonate;
  unsigned int tick_age;

public:
  Bomb();
  void tick();
  unsigned int getTickAge();
  unsigned int getTickDetonate();

};

// ------------------------------------------------------------

#endif

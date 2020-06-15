#include "bomb.h"

// ------------------------------------------------------------

Bomb::Bomb(unsigned int y, unsigned int x, unsigned int tick_detonate,
unsigned int range) :
y(y), x(x), tick_detonate(tick_detonate), tick_age(0), range(range) {}

// ------------------------------------------------------------

unsigned int Bomb::getY() const {

  return this->y;

}

// ------------------------------------------------------------

unsigned int Bomb::getX() const {

  return this->x;

}

// ------------------------------------------------------------

unsigned int Bomb::getTickDetonate() const {

  return this->tick_detonate;

}

// ------------------------------------------------------------

unsigned int Bomb::getTickAge() const {

  return this->tick_age;

}

// ------------------------------------------------------------

unsigned int Bomb::getRange() const {

  return this->range;

}

// ------------------------------------------------------------

void Bomb::setRange(unsigned int range) {

  this->range = range;

}

// ------------------------------------------------------------

void Bomb::tick(std::chrono::microseconds elapsed_time) {

  this->tick_age++;

}

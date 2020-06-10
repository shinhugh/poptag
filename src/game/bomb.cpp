#include "bomb.h"

// ------------------------------------------------------------

Bomb::Bomb(unsigned int y, unsigned int x, unsigned int tick_detonate,
unsigned int range) :
y(y), x(x), tick_detonate(tick_detonate), tick_age(0), range(range) {}

// ------------------------------------------------------------

unsigned int Bomb::getY() {

  return this->y;

}

// ------------------------------------------------------------

unsigned int Bomb::getX() {

  return this->x;

}

// ------------------------------------------------------------

unsigned int Bomb::getTickDetonate() {

  return this->tick_detonate;

}

// ------------------------------------------------------------

unsigned int Bomb::getTickAge() {

  return this->tick_age;

}

// ------------------------------------------------------------

unsigned int Bomb::getRange() {

  return this->range;

}

// ------------------------------------------------------------

void Bomb::setRange(unsigned int range) {

  this->range = range;

}

// ------------------------------------------------------------

void Bomb::tick() {

  this->tick_age++;

}

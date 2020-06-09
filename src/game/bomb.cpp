#include "bomb.h"

// ------------------------------------------------------------

Bomb::Bomb(unsigned int y, unsigned int x) :
y(y), x(x), tick_detonate(3000), tick_age(0), range(2) {

}

// ------------------------------------------------------------

void Bomb::tick() {

  this->tick_age++;

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

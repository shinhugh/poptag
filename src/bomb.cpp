#include "bomb.h"

// ------------------------------------------------------------

Bomb::Bomb() : tick_detonate(3000), tick_age(0) {

}

// ------------------------------------------------------------

void Bomb::tick() {

  this->tick_age++;

}

// ------------------------------------------------------------

unsigned int Bomb::getTickAge() {

  return this->tick_age;

}

// ------------------------------------------------------------

unsigned int Bomb::getTickDetonate() {

  return this->tick_detonate;

}

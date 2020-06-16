#include "bomb.h"

// ------------------------------------------------------------

Bomb::Bomb(unsigned int y, unsigned int x,
std::chrono::milliseconds time_detonate, unsigned int range) :
y(y), x(x), time_detonate(time_detonate),
time_age(std::chrono::microseconds(0)), range(range) {}

// ------------------------------------------------------------

unsigned int Bomb::getY() const {

  return this->y;

}

// ------------------------------------------------------------

unsigned int Bomb::getX() const {

  return this->x;

}

// ------------------------------------------------------------

std::chrono::milliseconds Bomb::getTimeDetonate() const {

  return this->time_detonate;

}

// ------------------------------------------------------------

std::chrono::microseconds Bomb::getTimeAge() const {

  return this->time_age;

}

// ------------------------------------------------------------

unsigned int Bomb::getRange() const {

  return this->range;

}

// ------------------------------------------------------------

void Bomb::update(std::chrono::microseconds elapsed_time) {

  // TODO: Increment this->time_age by elapsed_time
  this->time_age += elapsed_time;

}

#include "bomb.h"

#define BOMB_WIDTH 1.0f
#define BOMB_TIME_DETONATE 2000

// ------------------------------------------------------------

Bomb::Bomb() :
hitbox(BOMB_WIDTH / 2, BOMB_WIDTH / 2, BOMB_WIDTH, BOMB_WIDTH),
time_detonate(BOMB_TIME_DETONATE), time_age(std::chrono::microseconds(0)),
range(1) {}

// ------------------------------------------------------------

Bomb::Bomb(unsigned int y, unsigned int x, unsigned int range) :
hitbox(y + (BOMB_WIDTH / 2), x + (BOMB_WIDTH / 2), BOMB_WIDTH, BOMB_WIDTH),
time_detonate(BOMB_TIME_DETONATE), time_age(std::chrono::microseconds(0)),
range(range) {}

// ------------------------------------------------------------

const Hitbox * Bomb::getHitbox() const {

  return &(this->hitbox);

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

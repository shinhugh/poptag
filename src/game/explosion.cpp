#include "explosion.h"

#define EXPLOSION_WIDTH 1.0f
#define EXPLOSION_TIME_DISAPPEAR 400

// ------------------------------------------------------------

Explosion::Explosion() :
hitbox(EXPLOSION_WIDTH / 2, EXPLOSION_WIDTH / 2, EXPLOSION_WIDTH,
EXPLOSION_WIDTH),
time_disappear(EXPLOSION_TIME_DISAPPEAR),
time_age(std::chrono::microseconds(0)), direction(up) {}

// ------------------------------------------------------------

Explosion::Explosion(unsigned int y, unsigned int x, Direction direction) :
hitbox(y + (EXPLOSION_WIDTH / 2), x + (EXPLOSION_WIDTH / 2), EXPLOSION_WIDTH,
EXPLOSION_WIDTH),
time_disappear(EXPLOSION_TIME_DISAPPEAR),
time_age(std::chrono::microseconds(0)), direction(direction) {}

// ------------------------------------------------------------

const Hitbox * Explosion::getHitbox() const {

  return &(this->hitbox);

}

// ------------------------------------------------------------

std::chrono::milliseconds Explosion::getTimeDisappear() const {

  return this->time_disappear;

}

// ------------------------------------------------------------

std::chrono::microseconds Explosion::getTimeAge() const {

  return this->time_age;

}

// ------------------------------------------------------------

void Explosion::resetTimeAge() {

  this->time_age = std::chrono::microseconds(0);

}

// ------------------------------------------------------------

Direction Explosion::getDirection() const {

  return this->direction;

}

// ------------------------------------------------------------

void Explosion::setDirection(Direction direction) {

  this->direction = direction;

}

// ------------------------------------------------------------

void Explosion::update(std::chrono::microseconds elapsed_time) {

  this->time_age += elapsed_time;

}

#include "explosion.h"

#define EXPLOSION_WIDTH 1.0f
#define EXPLOSION_TIME_DISAPPEAR 500

// ------------------------------------------------------------

Explosion::Explosion() :
hitbox(EXPLOSION_WIDTH / 2, EXPLOSION_WIDTH / 2, EXPLOSION_WIDTH,
EXPLOSION_WIDTH),
time_disappear(EXPLOSION_TIME_DISAPPEAR),
time_age(std::chrono::microseconds(0)) {}

// ------------------------------------------------------------

Explosion::Explosion(unsigned int y, unsigned int x) :
hitbox(y + (EXPLOSION_WIDTH / 2), x + (EXPLOSION_WIDTH / 2), EXPLOSION_WIDTH,
EXPLOSION_WIDTH),
time_disappear(EXPLOSION_TIME_DISAPPEAR),
time_age(std::chrono::microseconds(0)) {}

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

void Explosion::update(std::chrono::microseconds elapsed_time) {

  this->time_age += elapsed_time;

}

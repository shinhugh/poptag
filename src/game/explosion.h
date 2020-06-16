// Explosion of a bomb

#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <chrono>
#include "hitbox.h"

// ------------------------------------------------------------

class Explosion {

private:
  Hitbox hitbox;
  std::chrono::milliseconds time_disappear;
  std::chrono::microseconds time_age;

public:
  Explosion();
  Explosion(unsigned int, unsigned int);
  const Hitbox * getHitbox() const;
  std::chrono::milliseconds getTimeDisappear() const;
  std::chrono::microseconds getTimeAge() const;
  void resetTimeAge();
  void update(std::chrono::microseconds);

};

// ------------------------------------------------------------

#endif

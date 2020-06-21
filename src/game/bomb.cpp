#include "bomb.h"

#define BOMB_WIDTH 1.0f
#define BOMB_TIME_DETONATE 2000

// ------------------------------------------------------------

Bomb::Bomb() :
hitbox(BOMB_WIDTH / 2, BOMB_WIDTH / 2, BOMB_WIDTH, BOMB_WIDTH),
time_detonate(BOMB_TIME_DETONATE), time_age(std::chrono::microseconds(0)),
range(1), breakthrough(false) {}

// ------------------------------------------------------------

Bomb::Bomb(unsigned int y, unsigned int x, unsigned int range,
bool breakthrough) :
hitbox(y + (BOMB_WIDTH / 2), x + (BOMB_WIDTH / 2), BOMB_WIDTH, BOMB_WIDTH),
time_detonate(BOMB_TIME_DETONATE), time_age(std::chrono::microseconds(0)),
range(range), breakthrough(breakthrough) {}

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

bool Bomb::getBreakthrough() const {

  return this->breakthrough;

}

// ------------------------------------------------------------

std::vector<unsigned int> Bomb::explosionCoordinates(unsigned int board_height,
unsigned int board_width) const {

  std::vector<unsigned int> coordinates;

  unsigned int center_y = static_cast<unsigned int>(this->hitbox.getCenterY());
  unsigned int center_x = static_cast<unsigned int>(this->hitbox.getCenterX());
  coordinates.push_back(center_y);
  coordinates.push_back(center_x);

  // BFS
  for(unsigned int curr_range = 1; curr_range <= this->range; curr_range++) {
    // Up
    if(center_y >= curr_range) {
      coordinates.push_back(center_y - curr_range);
      coordinates.push_back(center_x);
    }
    // Right
    if(center_x + curr_range < board_width) {
      coordinates.push_back(center_y);
      coordinates.push_back(center_x + curr_range);
    }
    // Down
    if(center_y + curr_range < board_height) {
      coordinates.push_back(center_y + curr_range);
      coordinates.push_back(center_x);
    }
    // Left
    if(center_x >= curr_range) {
      coordinates.push_back(center_y);
      coordinates.push_back(center_x - curr_range);
    }
  }

  return coordinates;

}

// ------------------------------------------------------------

void Bomb::update(std::chrono::microseconds elapsed_time) {

  this->time_age += elapsed_time;

}

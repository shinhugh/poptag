// Matter within the game's world; boundaries of an in-game entity

#ifndef HITBOX_H
#define HITBOX_H

#include <vector>

// ------------------------------------------------------------

class Hitbox {

private:
  float center[2];
  float dimensions[2];

public:
  Hitbox(float, float, float, float);
  float getCenterY() const;
  float getCenterX() const;
  float getHeight() const;
  float getWidth() const;
  void moveUp(float, const std::vector<const Hitbox *> *, float, float);
  void moveRight(float, const std::vector<const Hitbox *> *, float, float);
  void moveDown(float, const std::vector<const Hitbox *> *, float, float);
  void moveLeft(float, const std::vector<const Hitbox *> *, float, float);

};

// ------------------------------------------------------------

#endif

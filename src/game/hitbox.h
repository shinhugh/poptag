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
  void setCenterY(float);
  float getCenterX() const;
  void setCenterX(float);
  float getHeight() const;
  float getWidth() const;
  bool moveUp(float, const std::vector<const Hitbox *> *, float, float);
  bool moveRight(float, const std::vector<const Hitbox *> *, float, float);
  bool moveDown(float, const std::vector<const Hitbox *> *, float, float);
  bool moveLeft(float, const std::vector<const Hitbox *> *, float, float);

};

// ------------------------------------------------------------

#endif

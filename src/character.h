// In-game character

#ifndef CHARACTER_H
#define CHARACTER_H

// ------------------------------------------------------------

enum Direction {none, up, right, down, left};

// ------------------------------------------------------------

class Character {

private:
  float y, x;
  float speed;
  enum Direction dir_move;
  enum Direction dir_face;

public:
  Character(float, float, float);
  void tick();

};

// ------------------------------------------------------------

#endif

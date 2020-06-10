// Ongoing state for a game instance

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>

#include "bomb.h"

// ------------------------------------------------------------

class GameState {

private:
  std::vector<Bomb> bombs;

public:
  GameState();
  void internalUpdate();
  void externalUpdate(unsigned int, void *);

};

// ------------------------------------------------------------

#endif
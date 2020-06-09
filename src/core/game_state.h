// Ongoing state for a game instance

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>

#include "bomb.h"

// ------------------------------------------------------------

enum EventType {test, bomb};

struct EventData_Test {

  bool test_data;

};

struct EventData_Bomb {

  unsigned int y, x;

};

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

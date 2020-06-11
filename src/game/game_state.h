// Ongoing state for a game instance

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include "board.h"
#include "character.h"
#include "bomb.h"

// ------------------------------------------------------------

class GameState {

private:
  Board board;
  std::vector<Character> characters;
  std::vector<Bomb> bombs;

public:
  GameState();
  void internalUpdate();
  void externalUpdate(unsigned int, void *);
  void drawState();

};

// ------------------------------------------------------------

#endif

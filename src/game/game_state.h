// Data on the immediate state of a game instance
// Has no control over the flow of time; requires an external/wrapper module
// to moderate timing

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <chrono>
#include "data_packet.h"
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
  void internalUpdate(std::chrono::microseconds);
  void externalUpdate(DataPacket);
  const Board * getBoard();
  const std::vector<Character> * getCharacters();
  const std::vector<Bomb> * getBombs();

};

// ------------------------------------------------------------

#endif

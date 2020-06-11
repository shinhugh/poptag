// Ongoing state for a game instance

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
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
  void internalUpdate();
  void externalUpdate(DataPacket);
  DataPacket readState();

  void drawState(); // TODO: Remove

};

// ------------------------------------------------------------

#endif

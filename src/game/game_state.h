// Data on the immediate state of a game instance
// Has no control over the flow of time; requires an external/wrapper module
// to moderate timing

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <map>
#include <chrono>
#include "data_packet.h"
#include "character.h"
#include "bomb.h"
#include "explosion.h"
#include "board_block.h"

// ------------------------------------------------------------

class GameState {

private:
  unsigned int board_height, board_width;
  std::vector<Character> characters;
  std::vector<bool> characters_alive;
  Bomb **bombs;
  bool **bombs_exist;
  Explosion **explosions;
  bool **explosions_exist;
  BoardBlock **blocks;
  bool **blocks_exist;

  void createBlock(unsigned int, unsigned int);
  void explodeBomb(unsigned int, unsigned int);

public:
  GameState();
  GameState(const GameState&);
  GameState& operator=(const GameState&);
  ~GameState();
  unsigned int getBoardHeight() const;
  unsigned int getBoardWidth() const;
  const Character * getCharacter(unsigned int) const;
  bool getCharacterAlive(unsigned int) const;
  unsigned int getCharacterCount() const;
  const Bomb * getBomb(unsigned int, unsigned int) const;
  bool getBombExist(unsigned int, unsigned int) const;
  const Explosion * getExplosion(unsigned int, unsigned int) const;
  bool getExplosionExist(unsigned int, unsigned int) const;
  const BoardBlock * getBlock(unsigned int, unsigned int) const;
  bool getBlockExist(unsigned int, unsigned int) const;
  void internalUpdate(std::chrono::microseconds);
  void externalUpdate(DataPacket);

};

// ------------------------------------------------------------

#endif

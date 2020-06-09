// Top-level class Game and top-level routine for game core thread

#ifndef GAME_H
#define GAME_H

#include <mutex>
#include "game_state.h"

// ------------------------------------------------------------

// Top-level container for a game instance
// Contains the game state and a mutex protecting it from race conditions
class Game {

private:
  GameState state;
  std::mutex state_mutex;
  bool exit_flag;
  unsigned int tick_duration;

public:
  Game(unsigned int);
  void tickUpdate();
  void updateTestState(bool);
  void exit();
  bool isExit();
  unsigned int getTickDuration();

};

// ------------------------------------------------------------

// Top-level routine for game core thread
// Repeatedly update the state for a game instance
void core_ThreadRoutine(Game&);

// ------------------------------------------------------------

#endif

// Top-level class Game and top-level routine for game core thread

#ifndef GAME_H
#define GAME_H

#include <mutex>
#include <queue>
#include "game_state.h"
#include "data_packet.h"

// ------------------------------------------------------------

// Top-level container for a game instance
// Contains the game state and provides an interface to interact with it
class Game {

private:
  GameState state;
  std::mutex state_mutex;
  std::queue<DataPacket> event_queue;
  std::mutex event_queue_mutex;
  bool exit_flag;
  unsigned int tick_duration;

public:
  Game(unsigned int);
  void exit();
  bool isExit();
  unsigned int getTickDuration();
  void tickUpdate();
  void queueEvent(const DataPacket&);
  DataPacket readState();

};

// ------------------------------------------------------------

#endif

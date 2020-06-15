// Wrapper around game state that moderates multi-threaded access and provides
// a succinct interface for threads to use

#ifndef GAME_H
#define GAME_H

#include <chrono>
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
  std::mutex exit_flag_mutex;

public:
  Game();
  void exit();
  bool isExit();
  void updateState(std::chrono::microseconds);
  void queueEvent(const DataPacket&);
  GameState stateSnapshot();

};

// ------------------------------------------------------------

#endif

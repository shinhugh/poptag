// Ongoing state for a game instance

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>

// ------------------------------------------------------------

enum EventType {test};

struct EventData {

  EventType type;
  void *data;

};

struct TestEventData {

  bool test_data;

};

// ------------------------------------------------------------

class GameState {

private:

public:
  GameState();
  void internalUpdate();
  void externalUpdate(void *);

};

// ------------------------------------------------------------

#endif

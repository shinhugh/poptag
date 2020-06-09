// Ongoing state for a game instance

#ifndef GAME_STATE_H
#define GAME_STATE_H

class GameState {

private:
  bool test_state;

public:
  GameState();
  void tickUpdate();
  void updateTestState(bool);

};

#endif

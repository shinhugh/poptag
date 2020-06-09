// Ongoing state of the game

#ifndef GAME_STATE_H
#define GAME_STATE_H

// Type 0
struct GameStateUpdate_StateBool {
  bool value;
};

// Type 1
struct GameStateUpdate_StateUint {
  unsigned int value;
};

// Immediately update game state, specified by update type and data
void update_state(unsigned int update_type, void *update_data);

#endif

#include "game_state.h"

#include <iostream> // DEBUG

// Very basic states, just to test out everything
static bool state_bool;
static unsigned int state_uint;

// ------------------------------------------------------------

void update_state(unsigned int update_type, void *update_data) {

  switch(update_type) {
    case 0:
      {
        GameStateUpdate_StateBool *update_data_parsed
        = static_cast<GameStateUpdate_StateBool *>(update_data);
        state_bool = update_data_parsed->value;
      }
      break;
    case 1:
      {
        GameStateUpdate_StateUint *update_data_parsed
        = static_cast<GameStateUpdate_StateUint *>(update_data);
        state_uint = update_data_parsed->value;
      }
      break;
  }

  std::cerr << "state_bool: " + std::to_string(state_bool) + "\n"; // DEBUG
  std::cerr << "state_uint: " + std::to_string(state_uint) + "\n"; // DEBUG

}

// Entry routine for program

#include <iostream>
#include <thread>
#include "game_core.h"
#include "game_state.h"
#include "graphics.h"

// ------------------------------------------------------------

// Initialize various threads, each handling a specific task, and exit program
// when all threads complete
int main() {

  // Start thread that maintains game state and handles access/modification
  // requests
  std::thread thread_game_core(gameCore_ThreadStart);
  std::cerr << "Thread initialized: Game core\n";

  // Start thread that handles graphics output
  std::thread thread_graphics(graphics_ThreadStart);
  std::cerr << "Thread initialized: Graphics\n";



  // TESTING PORTION START
  bool test_ongoing = true;
  bool state_bool = false;
  unsigned int state_uint = 1;
  while(test_ongoing) {
    // Take input from user
    std::string user_input;
    std::cin >> user_input;
    // Quit
    if(user_input == "quit") {
      gameCore_ThreadStop();
      test_ongoing = false;
    }
    // Send update to game core to wake up thread
    else if (user_input == "bool") {
      GameStateUpdate_StateBool new_update;
      new_update.value = state_bool;
      state_bool = !state_bool;
      gameCore_QueueUpdate(500, 0, &new_update,
      sizeof(GameStateUpdate_StateBool));
    }
    else if (user_input == "uint") {
      GameStateUpdate_StateUint new_update;
      new_update.value = state_uint;
      state_uint++;
      gameCore_QueueUpdate(500, 1, &new_update,
      sizeof(GameStateUpdate_StateUint));
    }
  }
  // TESTING PORTION FINISH



  // Wait until all threads exit
  thread_game_core.join();
  thread_graphics.join();

  // Exit program
  std::cout << "Exiting program\n";
  return 0;

}

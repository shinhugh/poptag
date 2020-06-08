// Entry routine for program

#include <iostream>
#include <thread>
#include "game_core.h"
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
  unsigned int event_counter = 0;
  while(test_ongoing) {
    // Take input from user
    std::string user_input;
    std::cin >> user_input;
    // Quit
    if(user_input == "quit") {
      gameCore_ThreadStop();
      test_ongoing = false;
    }
    // Send event to game core to wake up thread
    else if (user_input == "e") {
      gameCore_QueueEvent(event_counter);
      event_counter++;
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

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
  std::thread thread_game_core(threadTaskGameCore);
  std::cerr << "Thread initialized: Game core\n";

  // Start thread that handles graphics output
  std::thread thread_graphics(threadTaskGraphics);
  std::cerr << "Thread initialized: Graphics\n";

  // TESTING PORTION START
  bool ongoing = true;
  while(ongoing) {
    // Take input from user
    std::string user_input;
    std::cin >> user_input;
    // Quit
    if(user_input == "quit") {
      // TODO: End game core thread
      ongoing = false;
    }
    // Send event to game core to wake up thread
    else if (user_input == "e") {
      {
        // Lock mutex
        std::lock_guard<std::mutex> thread_lock(thread_game_core_mutex);
        // Set awake flag to true
        thread_game_core_awake = true;
      }
      thread_game_core_cv.notify_one();
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

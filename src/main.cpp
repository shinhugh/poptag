// Top-level routine for program

#include <iostream>
#include <thread>
#include <string>
#include "game.h"

// ------------------------------------------------------------

int main() {

  // Game instance
  Game game(1000000); // 1 tick = 1 second

  // Initialize core game thread
  std::thread thread_core(core_ThreadRoutine, std::ref(game));
  std::cerr << "Thread initialized: Game core\n";

  // Take input from console
  while(1) {
    std::string input;
    std::cin >> input;
    if(input == "quit") {
      game.exit();
      break;
    }
    else if(input == "e") {
      game.updateTestState(true);
    }
  }

  // Wait until all threads exit
  thread_core.join();

  return 0;

}

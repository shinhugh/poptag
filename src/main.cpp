// Top-level routine for program

#include <iostream>
#include <thread>
#include "game.h"

// ------------------------------------------------------------

int main() {

  // Game instance
  Game game(1000000); // 1 tick = 1 second

  // Initialize core game thread
  std::thread thread_core(core_ThreadStart, game);
  std::cerr << "Thread initialized: Game core\n";

  // Wait until all threads exit
  thread_core.join();

  return 0;

}

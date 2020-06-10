// Top-level routine for program

#include <iostream>
#include <thread>
#include <string>
#include <cstring>
#include "game.h"
#include "input.h"

// Time interval between ticks, given in microseconds
#define TICK_MICROSEC 1000

// ------------------------------------------------------------

int main() {

  // Game instance
  Game game(TICK_MICROSEC);

  // Initialize core game thread
  std::thread thread_core(core_ThreadRoutine, std::ref(game));
  std::cerr << "Thread initialized: Game core\n";

  // Initialize user input thread
  std::thread thread_core(input_ThreadRoutine, std::ref(game));
  std::cerr << "Thread initialized: User input\n";

  // Wait until all threads exit
  thread_core.join();

  return 0;

}

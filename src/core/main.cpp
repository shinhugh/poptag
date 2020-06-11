// Top-level routine for program

#include <thread>
#include <iostream>
#include "thread_core.h"
#include "thread_display.h"
#include "thread_input.h"
#include "game.h"

// Time interval between ticks, given in microseconds
#define TICK_MICROSEC 1000

// ------------------------------------------------------------

int main() {

  // Game instance
  Game game(TICK_MICROSEC);

  // Initialize core game thread
  std::thread thread_core(threadRoutine_Core, std::ref(game));
  std::cerr << "Thread initialized: Game core\n";

  /*
  // Initialize display thread
  std::thread thread_display(threadRoutine_Display, std::ref(game));
  std::cerr << "Thread initialized: Game state display\n";
  */

  // Initialize user input thread
  std::thread thread_input(threadRoutine_Input, std::ref(game));
  std::cerr << "Thread initialized: User input handling\n";

  // Wait until all threads exit
  thread_core.join();
  // thread_display.join();
  thread_input.join();

  return 0;

}

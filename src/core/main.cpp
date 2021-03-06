// Top-level routine for program

#include <thread>
#include <iostream>
#include "thread_core.h"
#include "thread_display.h"
#include "game.h"

// ------------------------------------------------------------

int main() {

  // Game instance
  Game game;

  // Initialize core game thread
  std::thread thread_core(threadRoutine_Core, std::ref(game));
  std::cerr << "Thread initialized: Game core\n";

  // Initialize GUI thread (graphics + user input handling)
  std::thread thread_display(threadRoutine_Display, std::ref(game));
  std::cerr << "Thread initialized: Game state display\n";

  // Wait until all threads exit
  thread_core.join();
  thread_display.join();

  return 0;

}

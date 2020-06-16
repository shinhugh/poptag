#include <thread>
#include <chrono>
#include "thread_core.h"

#include <iostream> // DEBUG
#include <string> // DEBUG

// ------------------------------------------------------------

void threadRoutine_Core(Game& game) {

  // Timing
  auto time_start = std::chrono::system_clock::now();
  auto time_end = time_start;

  while(!game.isExit()) {

    // Capture end of time window
    time_end = std::chrono::system_clock::now();

    // Get elapsed time duration
    std::chrono::microseconds elapsed_time
    = std::chrono::duration_cast<std::chrono::microseconds>
    (time_end - time_start);

    /*
    // DEBUG
    std::cerr << std::string("Elapsed time: ") << elapsed_time.count() << '\n';
    // DEBUG
    */

    // Update state according to elapsed time
    game.updateState(elapsed_time);

    // End of current time window becomes start of next time window
    time_start = time_end;

    // Sleep thread briefly, just to avoid overwhelming computation
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

  }

}

#include <thread>
#include <chrono>
#include "thread_core.h"

// ------------------------------------------------------------

void threadRoutine_Core(Game& game) {

  // Sleep for 1 tick
  auto wakeup_time = std::chrono::system_clock::now()
  + (game.getTickDuration() * std::chrono::microseconds(1));
  std::this_thread::sleep_until(wakeup_time);

  // Wake up and calculate when to wake up next
  wakeup_time = std::chrono::system_clock::now()
  + (game.getTickDuration() * std::chrono::microseconds(1));

  while(!game.isExit()) {

    // Update state, reflecting passage of 1 tick
    game.tickUpdate();

    // Sleep for 1 tick
    std::this_thread::sleep_until(wakeup_time);

    // Wake up and calculate when to wake up next
    wakeup_time = std::chrono::system_clock::now()
    + (game.getTickDuration() * std::chrono::microseconds(1));

  }

}

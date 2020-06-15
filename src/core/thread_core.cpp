#include <thread>
#include <chrono>
#include "thread_core.h"

#include <iostream> // DEBUG
#include <iomanip> // DEBUG
#include <ctime> // DEBUG

// ------------------------------------------------------------

void threadRoutine_Core(Game& game) {

  // Sleep for 1 tick
  auto wakeup_time = std::chrono::system_clock::now()
  + (game.getTickDuration() * std::chrono::microseconds(1));
  std::this_thread::sleep_until(wakeup_time);

  // Wake up and calculate when to wake up next
  wakeup_time = std::chrono::system_clock::now()
  + (game.getTickDuration() * std::chrono::microseconds(1));

  // DEBUG
  unsigned int count = 0;
  // DEBUG

  while(!game.isExit()) {

    // Update state, reflecting passage of 1 tick
    game.tickUpdate();

    // Sleep for 1 tick
    std::this_thread::sleep_until(wakeup_time);

    // Wake up and calculate when to wake up next
    wakeup_time = std::chrono::system_clock::now()
    + (game.getTickDuration() * std::chrono::microseconds(1));

    // DEBUG
    /*
    auto time_point = std::chrono::system_clock::now();
    std::time_t ttp = std::chrono::system_clock::to_time_t(time_point);
    std::cerr << "time: " << std::ctime(&ttp) << "\n";
    std::cerr << count << "\n";
    count++;
    */
    // DEBUG

  }

}

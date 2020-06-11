#include <thread>
#include <chrono>
#include <iostream>
#include "thread_display.h"
#include "data_packet.h"
#include "state_data.h"

#define FRAMES_PER_SEC 5

// ------------------------------------------------------------

static void printState(Game&);

// ------------------------------------------------------------

void threadRoutine_Display(Game& game) {

  unsigned int frame_duration
  = static_cast<unsigned int>(((static_cast<float>(1.0)) / FRAMES_PER_SEC)
  * 1000000);

  // Next wakeup time
  auto wakeup_time = std::chrono::system_clock::now()
  + (frame_duration * std::chrono::microseconds(1));

  // Loop until game exits
  while(!(game.isExit())) {

    // Print state
    printState(game);

    // Sleep until next frame should be shown
    std::this_thread::sleep_until(wakeup_time);

    // Wake up and calculate when to wake up next
    wakeup_time = std::chrono::system_clock::now()
    + (frame_duration * std::chrono::microseconds(1));

  }

}

// ------------------------------------------------------------

static void printState(Game& game) {

  DataPacket packet = game.readState();
  StateData *state_data = static_cast<StateData *>(packet.getData());

  std::cerr << "Character: (" + std::to_string(state_data->character_y) + ", "
  + std::to_string(state_data->character_x) + ")\n";

}

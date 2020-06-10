// Top-level routine for program

#include <iostream>
#include <thread>
#include <string>
#include <cstring>
#include "game.h"
#include "game_state.h"
#include "event_types.h"

// Time interval between ticks, given in microseconds
#define TICK_MICROSEC 1000

// ------------------------------------------------------------

int main() {

  // Game instance
  Game game(TICK_MICROSEC);

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
      EventData event;
      EventData_Test event_data;
      event_data.test_data = true;
      event.setType(test);
      event.setData(&event_data, sizeof(EventData_Test));
      game.queueEvent(event);
    }
    else if(input == "b") {
      EventData event;
      EventData_Bomb event_data;
      event_data.y = 1;
      event_data.x = 1;
      event.setType(bomb);
      event.setData(&event_data, sizeof(EventData_Bomb));
      game.queueEvent(event);
    }
  }

  // Wait until all threads exit
  thread_core.join();

  return 0;

}

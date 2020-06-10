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
    else if(input == "test") {
      EventData event;
      EventData_Test event_data;
      event_data.message = "Hello world!";
      event.setType(test);
      event.setData(&event_data, sizeof(EventData_Test));
      game.queueEvent(event);
    }
    else if(input == "bomb") {
      EventData event;
      EventData_PlaceBomb event_data;
      event_data.y = 1;
      event_data.x = 1;
      event_data.tick_detonate = 1000;
      event_data.range = 1;
      event.setType(placeBomb);
      event.setData(&event_data, sizeof(EventData_PlaceBomb));
      game.queueEvent(event);
    }
  }

  // Wait until all threads exit
  thread_core.join();

  return 0;

}

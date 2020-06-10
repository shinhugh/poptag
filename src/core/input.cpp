#include <iostream>
#include "input.h"
#include "event_types.h"

void input_ThreadRoutine(Game& game) {

  // Take input from console
  while(1) {
    std::string input;
    std::getline(std::cin, input);

    // Quit
    if(input == "quit") {
      game.exit();
      break;
    }

    // Test
    else if(input == "test") {
      EventData event;
      EventData_Test event_data;
      event_data.message = "Hello world!";
      event.setType(test);
      event.setData(&event_data, sizeof(EventData_Test));
      game.queueEvent(event);
    }

    // Place bomb
    else if(input.substr(0, input.find(" ")) == "b") {
      EventData event;
      EventData_PlaceBomb event_data;
      event_data.y = std::stoi(input.substr(2, 1));
      event_data.x = std::stoi(input.substr(4, 1));
      event_data.tick_detonate = 3000;
      event_data.range = 1;
      event.setType(placeBomb);
      event.setData(&event_data, sizeof(EventData_PlaceBomb));
      game.queueEvent(event);
    }

    // Stop moving
    else if(input == "e") {

    }

    // Move up
    else if(input == "w") {

    }

    // Move right
    else if(input == "d") {

    }

    // Move down
    else if(input == "s") {

    }

    // Move left
    else if(input == "a") {

    }

  }

}

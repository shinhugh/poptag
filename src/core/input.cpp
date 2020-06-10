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

    // Initialize
    else if(input == "i") {
      EventData event;
      EventData_Initialize event_data;
      event_data.initialize = true;
      event.setType(initialize);
      event.setData(&event_data, sizeof(EventData_Initialize));
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
      EventData event;
      EventData_MoveStop event_data;
      event_data.character_id = 0;
      event.setType(moveStop);
      event.setData(&event_data, sizeof(EventData_MoveStop));
      game.queueEvent(event);
    }

    // Move up
    else if(input == "w") {
      EventData event;
      EventData_MoveUp event_data;
      event_data.character_id = 0;
      event.setType(moveUp);
      event.setData(&event_data, sizeof(EventData_MoveUp));
      game.queueEvent(event);
    }

    // Move right
    else if(input == "d") {
      EventData event;
      EventData_MoveRight event_data;
      event_data.character_id = 0;
      event.setType(moveRight);
      event.setData(&event_data, sizeof(EventData_MoveRight));
      game.queueEvent(event);
    }

    // Move down
    else if(input == "s") {
      EventData event;
      EventData_MoveDown event_data;
      event_data.character_id = 0;
      event.setType(moveDown);
      event.setData(&event_data, sizeof(EventData_MoveDown));
      game.queueEvent(event);
    }

    // Move left
    else if(input == "a") {
      EventData event;
      EventData_MoveLeft event_data;
      event_data.character_id = 0;
      event.setType(moveLeft);
      event.setData(&event_data, sizeof(EventData_MoveLeft));
      game.queueEvent(event);
    }

  }

}

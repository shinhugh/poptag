#include <iostream>
#include "thread_input.h"
#include "data_packet.h"
#include "event_data.h"

// ------------------------------------------------------------

void threadRoutine_Input(Game& game) {

  // Take input from console
  while(1) {
    std::string input;
    std::getline(std::cin, input);

    // Quit
    if(input == "q") {
      game.exit();
      break;
    }

    // Initialize
    else if(input == "i") {
      DataPacket event;
      EventData_Initialize event_data;
      event_data.initialize = true;
      event.setType(initialize);
      event.setData(&event_data, sizeof(EventData_Initialize));
      game.queueEvent(event);
    }

    // Place bomb
    else if(input.substr(0, input.find(" ")) == "b") {
      DataPacket event;
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
      DataPacket event;
      EventData_MoveStop event_data;
      event_data.character_id = 0;
      event.setType(moveStop);
      event.setData(&event_data, sizeof(EventData_MoveStop));
      game.queueEvent(event);
    }

    // Move up
    else if(input == "w") {
      DataPacket event;
      EventData_MoveUp event_data;
      event_data.character_id = 0;
      event.setType(moveUp);
      event.setData(&event_data, sizeof(EventData_MoveUp));
      game.queueEvent(event);
    }

    // Move right
    else if(input == "d") {
      DataPacket event;
      EventData_MoveRight event_data;
      event_data.character_id = 0;
      event.setType(moveRight);
      event.setData(&event_data, sizeof(EventData_MoveRight));
      game.queueEvent(event);
    }

    // Move down
    else if(input == "s") {
      DataPacket event;
      EventData_MoveDown event_data;
      event_data.character_id = 0;
      event.setType(moveDown);
      event.setData(&event_data, sizeof(EventData_MoveDown));
      game.queueEvent(event);
    }

    // Move left
    else if(input == "a") {
      DataPacket event;
      EventData_MoveLeft event_data;
      event_data.character_id = 0;
      event.setType(moveLeft);
      event.setData(&event_data, sizeof(EventData_MoveLeft));
      game.queueEvent(event);
    }

  }

}

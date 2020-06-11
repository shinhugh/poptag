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
      DataPacket packet;
      EventData_Initialize event_data;
      event_data.initialize = true;
      packet.setType(initialize);
      packet.setData(&event_data, sizeof(EventData_Initialize));
      game.queueEvent(packet);
    }

    // Place bomb
    else if(input.substr(0, input.find(" ")) == "b") {
      DataPacket packet;
      EventData_PlaceBomb event_data;
      event_data.y = std::stoi(input.substr(2, 1));
      event_data.x = std::stoi(input.substr(4, 1));
      event_data.tick_detonate = 3000;
      event_data.range = 1;
      packet.setType(placeBomb);
      packet.setData(&event_data, sizeof(EventData_PlaceBomb));
      game.queueEvent(packet);
    }

    // Stop moving
    else if(input == "e") {
      DataPacket packet;
      EventData_MoveStop event_data;
      event_data.character_id = 0;
      packet.setType(moveStop);
      packet.setData(&event_data, sizeof(EventData_MoveStop));
      game.queueEvent(packet);
    }

    // Move up
    else if(input == "w") {
      DataPacket packet;
      EventData_MoveUp event_data;
      event_data.character_id = 0;
      packet.setType(moveUp);
      packet.setData(&event_data, sizeof(EventData_MoveUp));
      game.queueEvent(packet);
    }

    // Move right
    else if(input == "d") {
      DataPacket packet;
      EventData_MoveRight event_data;
      event_data.character_id = 0;
      packet.setType(moveRight);
      packet.setData(&event_data, sizeof(EventData_MoveRight));
      game.queueEvent(packet);
    }

    // Move down
    else if(input == "s") {
      DataPacket packet;
      EventData_MoveDown event_data;
      event_data.character_id = 0;
      packet.setType(moveDown);
      packet.setData(&event_data, sizeof(EventData_MoveDown));
      game.queueEvent(packet);
    }

    // Move left
    else if(input == "a") {
      DataPacket packet;
      EventData_MoveLeft event_data;
      event_data.character_id = 0;
      packet.setType(moveLeft);
      packet.setData(&event_data, sizeof(EventData_MoveLeft));
      game.queueEvent(packet);
    }

  }

}

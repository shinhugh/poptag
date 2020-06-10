// Various events an external module can pass into the game state module

#ifndef EVENT_TYPES_H
#define EVENT_TYPES_H

#include <string>

// ------------------------------------------------------------

enum EventType {test, initialize, placeBomb, moveStop, moveUp, moveRight,
moveDown, moveLeft};

struct EventData_Test {

  std::string message;

};

struct EventData_Initialize {

  bool initialize;

};

struct EventData_PlaceBomb {

  unsigned int y, x;
  unsigned int tick_detonate;
  unsigned int range;

};

struct EventData_MoveStop {

  unsigned int character_id;

};

struct EventData_MoveUp {

  unsigned int character_id;

};

struct EventData_MoveRight {

  unsigned int character_id;

};

struct EventData_MoveDown {

  unsigned int character_id;

};

struct EventData_MoveLeft {

  unsigned int character_id;

};

// ------------------------------------------------------------

#endif

// Various events an external module can pass into the game state module

#ifndef EVENT_TYPES_H
#define EVENT_TYPES_H

#include <string>

// ------------------------------------------------------------

enum EventType {test, placeBomb};

struct EventData_Test {

  std::string message;

};

struct EventData_PlaceBomb {

  unsigned int y, x;
  unsigned int tick_detonate;
  unsigned int range;

};

// ------------------------------------------------------------

#endif

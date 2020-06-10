// Various events an external module can pass into the game state module

#ifndef EVENT_TYPES_H
#define EVENT_TYPES_H

// ------------------------------------------------------------

enum EventType {test, bomb};

struct EventData_Test {

  bool test_data;

};

struct EventData_Bomb {

  unsigned int y, x;

};

// ------------------------------------------------------------

#endif

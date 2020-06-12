// Various events an external module can pass into the game state module to
// trigger state changes

#ifndef EVENT_DATA_H
#define EVENT_DATA_H

// ------------------------------------------------------------

enum EventType {test, initialize, placeBomb, moveStop, moveUp, moveRight,
moveDown, moveLeft};

// ------------------------------------------------------------

struct EventData_Initialize {

  bool initialize;

};

struct EventData_PlaceBomb {

  unsigned int character_id;

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

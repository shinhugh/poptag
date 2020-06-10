// Event data being passed from input handler to game state

#ifndef EVENT_DATA_H
#define EVENT_DATA_H

// ------------------------------------------------------------

// A pending update to the game state caused by an external event
class EventData {

private:
  unsigned int type;
  unsigned char *data;
  unsigned int data_size;

public:
  EventData();
  EventData(const EventData&);
  EventData& operator=(const EventData&);
  ~EventData();
  unsigned int getType();
  void setType(unsigned int);
  void * getData();
  void setData(void *, unsigned int);
  unsigned int getDataSize();

};

// ------------------------------------------------------------

#endif

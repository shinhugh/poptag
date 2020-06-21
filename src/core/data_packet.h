// A general container for variable size data
// Used to transfer information between the game state core and external modules
// Class Game allows communication between its internal game state instance and
// external modules by DataPacket I/O

#ifndef DATA_PACKET_H
#define DATA_PACKET_H

// ------------------------------------------------------------

// A container for any variable size data, with no semantic understanding of the
// contents
class DataPacket {

private:

  unsigned int type;
  unsigned char *data;
  unsigned int data_size;

public:

  DataPacket();
  DataPacket(const DataPacket&);
  DataPacket& operator=(const DataPacket&);
  ~DataPacket();
  unsigned int getType();
  void setType(unsigned int);
  void * getData();
  void setData(void *, unsigned int);
  unsigned int getDataSize();

};

// ------------------------------------------------------------

#endif

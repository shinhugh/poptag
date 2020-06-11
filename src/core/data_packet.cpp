#include <cstring>
#include "data_packet.h"

// ------------------------------------------------------------

DataPacket::DataPacket() :
type(0), data(0), data_size(0) {}

// ------------------------------------------------------------

DataPacket::DataPacket(const DataPacket& src) :
type(src.type), data_size(src.data_size) {

  if(src.data) {
    this->data = new unsigned char[this->data_size];
    memcpy(this->data, src.data, this->data_size);
  } else {
    this->data = 0;
  }

}

// ------------------------------------------------------------

DataPacket& DataPacket::operator=(const DataPacket& src) {

  this->type = src.type;
  this->data_size = src.data_size;
  if(this->data) {
    delete[] this->data;
  }
  if(src.data) {
    this->data = new unsigned char[this->data_size];
    memcpy(this->data, src.data, this->data_size);
  } else {
    this->data = 0;
  }

  return *this;

}

// ------------------------------------------------------------

DataPacket::~DataPacket() {

  if(this->data) {
    delete[] this->data;
  }

}

// ------------------------------------------------------------

unsigned int DataPacket::getType() {

  return this->type;

}

// ------------------------------------------------------------

void DataPacket::setType(unsigned int type) {

  this->type = type;

}

// ------------------------------------------------------------

void * DataPacket::getData() {

  return this->data;

}

// ------------------------------------------------------------

void DataPacket::setData(void * data, unsigned int data_size) {

  if(this->data) {
    delete[] this->data;
  }
  this->data = new unsigned char[data_size];
  memcpy(this->data, data, data_size);
  this->data_size = data_size;

}

// ------------------------------------------------------------

unsigned int DataPacket::getDataSize() {

  return this->data_size;

}

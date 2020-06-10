#include <cstring>
#include "event_data.h"

// ------------------------------------------------------------

EventData::EventData() :
type(0), data(0), data_size(0) {}

// ------------------------------------------------------------

EventData::EventData(const EventData& src) :
type(src.type), data_size(src.data_size) {

  if(src.data) {
    this->data = new unsigned char[this->data_size];
    memcpy(this->data, src.data, this->data_size);
  } else {
    this->data = 0;
  }

}

// ------------------------------------------------------------

EventData& EventData::operator=(const EventData& src) {

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

EventData::~EventData() {

  if(this->data) {
    delete[] this->data;
  }

}

// ------------------------------------------------------------

unsigned int EventData::getType() {

  return this->type;

}

// ------------------------------------------------------------

void EventData::setType(unsigned int type) {

  this->type = type;

}

// ------------------------------------------------------------

void * EventData::getData() {

  return this->data;

}

// ------------------------------------------------------------

void EventData::setData(void * data, unsigned int data_size) {

  if(this->data) {
    delete[] this->data;
  }
  this->data = new unsigned char[data_size];
  memcpy(this->data, data, data_size);
  this->data_size = data_size;

}

// ------------------------------------------------------------

unsigned int EventData::getDataSize() {

  return this->data_size;

}

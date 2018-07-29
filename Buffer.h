#ifndef BUFFER_H
#define BUFFER_H
#include <iostream>
#include <string>
#include "Config.h"
const byte WRITING = 0x01;
const byte READING = 0x02;
class Buffer_Exception : std::runtime_error {
 private:
  std::string msg;

 public:
  Buffer_Exception(const std::string& str);
  const char* what() const throw();
};
class Buffer {
 private:
  byte* buffer;
  int limit;
  int position;
  int capacity;
  byte state;

 public:
  Buffer(int size);
  // for reading
  Buffer(const byte b[], int size);
  // put byte
  void putByte(const byte& value);
  // put short
  void putShort(const ::uint16_t& value);
  // put int
  void putInt(const ::u_int32_t& value);
  //put long
  void putLong(const ::uint64_t& value);
  // put float
  void putFloat(const Float& value);
  // put string
  void putString(const std::string& s);
  // getByte
  byte getByte();
  // getShort
  ::uint16_t getShort();
  // getInt
  ::uint32_t getInt();
  //get Long
  ::uint64_t getLong();
  // get float
  float getFloat();

  std::string getString(const int& len);

  byte getState() const;

  void startReading();

  void reset();

  byte* getBuffer() const;

  int getCapacity() const;

  int getPacketSize() const;

  ~Buffer() {
    if (buffer != nullptr) delete[] buffer;
  }
};

#endif

#include "Buffer.h"

Buffer_Exception::Buffer_Exception(const std::string& str)
    : std::runtime_error(str.c_str()), msg(str) {}
const char* Buffer_Exception::what() const throw() { return msg.c_str(); }

Buffer::Buffer(int size)

    : limit(0), position(0), capacity(size), state(WRITING) {
  buffer = new byte[capacity];
}
Buffer::Buffer(const byte b[], int size)
    : limit(size), position(0), capacity(size), state(READING) {
  buffer = new byte[capacity];
  for (int i = 0; i < capacity; i++) buffer[i] = b[i];
#ifdef DEBUG
  std::cout << "buffer size: " << capacity << std::endl;
#endif 
}

void Buffer::putByte(const byte& value) {
  if (position + 1 > capacity)
    throw Buffer_Exception("out of range ( position > capacity)");
  if (state != WRITING) throw Buffer_Exception("state is reading mode");
  buffer[position++] = value;
  limit = position;
}

void Buffer::putShort(const ::uint16_t& value) {
  if (position + 2 > capacity)
    throw Buffer_Exception("out of range (position > capacity)");
  if (state != WRITING) throw Buffer_Exception("state is reading mode");
  byte byte1;
  byte byte2;
  byte1 = value & 0xFF;
  byte2 = (value >> 8) & 0xFF;
  buffer[position++] = byte1;
  buffer[position++] = byte2;
  limit = position;
}

void Buffer::putInt(const ::uint32_t& value) {
  if (position + 4 > capacity)
    throw Buffer_Exception("out of range(position > capacity)");
  if (state != WRITING) throw Buffer_Exception("state is reading mode");
  byte byte1 = value & 0xFF;
  byte byte2 = (value >> 8) & 0xFF;
  byte byte3 = (value >> 16) & 0xFF;
  byte byte4 = (value >> 24) & 0xFF;
  buffer[position++] = byte1;
  buffer[position++] = byte2;
  buffer[position++] = byte3;
  buffer[position++] = byte4;
  limit = position;
}

void Buffer::putFloat(const Float& value) { putInt(value.intValue); }

void Buffer::putString(const std::string& s) {
  int stringSize = s.size();
  const char* c_str = s.c_str();
  for (int i = 0; i < stringSize; i++) putByte(c_str[i]);
}

byte Buffer::getByte() {
  if (position + 1 > limit)
    throw Buffer_Exception("out of range (limit < position)");
  if (state != READING) throw Buffer_Exception("state is writing mode");
  return buffer[position++];
}
::uint16_t Buffer::getShort() {
  if (position + 2 > limit)
    throw Buffer_Exception("out of range (limit < position)");
  if (state != READING) throw Buffer_Exception("state is writing mode");
  short byte1 = buffer[position++];
  short byte2 = (buffer[position++]) << 8;
  short value = byte1 | byte2;
  return value;
}

::uint32_t Buffer::getInt() {
  if (position + 4 > limit)
    throw Buffer_Exception("out of range (limit < position)");
  if (state != READING) throw Buffer_Exception("state is writing mode");
  int byte1 = buffer[position++];
  int byte2 = buffer[position++] << 8;
  int byte3 = buffer[position++] << 16;
  int byte4 = buffer[position++] << 24;
  int value = byte1 | byte2 | byte3 | byte4;
  return value;
}
float Buffer::getFloat() {
  if (position + 4 > limit)
    throw Buffer_Exception("out of range (limit < position)");
  if (state != READING) throw Buffer_Exception("state is writing mode");
  Float mfloat;
  mfloat.intValue = getInt();
  return mfloat.floatValue;
}
byte Buffer::getState() const { return state; }
void Buffer::startReading() {
  limit = position;
  position = 0;
  state = READING;
}

void Buffer::reset() {
  position = 0;
  limit = 0;
  state = WRITING;
}
byte* Buffer::getBuffer() const { return buffer; }
int Buffer::getCapacity() const { return capacity; }
int Buffer::getPacketSize() const { return limit; }

std::string Buffer::getString(const int& len) {
  if (position + len > limit)
    throw Buffer_Exception("out of range(limit < position)");
  if (state != READING) throw Buffer_Exception("state is writing mode");
  byte array[len];
  for (int i = 0; i < len; i++) {
    array[i] = buffer[position + i];
  }
  position += len;
  std::string rt((char*)array, len);
  return rt;
}

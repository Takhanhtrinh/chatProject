#ifndef PACKET_H
#define PACKET_H
#include <string>
#include <vector>
#include "Buffer.h"
#include "Config.h"
const byte CREATE_USER_PACKET = 'q';
const byte CREATE_PROFILE_PICTURE = 'w';
const byte CREATE_ROOM_PACKET = 'e';
const byte JOIN_A_ROOM_PACKET = 'r';
const byte SEND_DATA_TO_EVERYONE_IN_A_ROOM_PACKET = 't';
const byte SEND_DATA_TO_USERS_IN_A_ROOM_PACKET = 'y';
const byte LEAVE_A_ROOM_PACKET = 'u';
const byte AN_USER_LEAVE_A_ROOM_PACKET = 'i';
const byte AN_USER_JOIN_A_ROOM = 'o';

int getPacketSize(const byte& type, void* data) {
  switch (type) {
    case CREATE_USER_PACKET: {
      size_t* array = (size_t*)data;
      size_t nameLength = array[0];

      return 1 + nameLength;
    }
    case CREATE_PROFILE_PICTURE: {
      // 1  for packet type + 1024(32 x 32 pixels)
      return 1025;
    }
    case CREATE_ROOM_PACKET: {
      size_t* array = (size_t*)data;
      size_t nameLength = array[0];
      // 1 for packet type;
      return 1 + nameLength;
    }
    case JOIN_A_ROOM_PACKET: {
      // 4 bytes for room id and 1 for packetType
      return 5;
    }
    case SEND_DATA_TO_EVERYONE_IN_A_ROOM_PACKET: {
      int* array = (int*)data;
      int dataSize = array[0];
      // 1 for packet type
      //   + sizeOfData + roomId
      return 1 + dataSize + 4 + 4;
    }
    case SEND_DATA_TO_USERS_IN_A_ROOM_PACKET: {
      int* intData = (int*)data;
      int dataSize = intData[0];
      int numberOfUsers = intData[1];
      return 1 + 4 + 4 + 4 + dataSize + numberOfUsers * 4;
    }
    case LEAVE_A_ROOM_PACKET: {
      return 5;
    }
    case AN_USER_LEAVE_A_ROOM_PACKET: {
      return 5;
    }
    case AN_USER_JOIN_A_ROOM: {
      int nameLength = *(int*)data;
      return 1 + (32 * 32) * 4 + nameLength + 4;
    }
    default:
#ifdef DEBUG
      printf("DEBUG getPacketSize this packet type doesn't exist %d\n", type);
#endif
      return -1;
  }
}
class Packet {
 protected:
  Buffer buffer;
  byte packetType;
  int m_size;

 public:
  Packet(const int& size, const byte& type)
      : buffer(size), packetType(type), m_size(size) {}
  Packet(byte* data, const int& size) : buffer(data, size), m_size(size) {}
  virtual void serialization() = 0;
  virtual void derserialization() = 0;
  int getPacketSize() const { return m_size; }
  byte getPacketType() const { return packetType; }
  void startReading() { buffer.startReading(); }
  byte* getBuffer() { return buffer.getBuffer(); }
  virtual ~Packet() {}
};

class CreateUserPacket : public Packet {
 private:
  std::string name;

 public:
  CreateUserPacket(const std::string& n)
      : Packet(::getPacketSize(CREATE_USER_PACKET, (size_t[]){n.size()}),
               CREATE_USER_PACKET),
        name(n) {}
  CreateUserPacket(byte* buffer, const int& size) : Packet(buffer, size) {}
  const std::string& getName() const { return name; }
  void serialization() override {
    buffer.putByte(packetType);
    buffer.putString(name);
  }
  void derserialization() override {
    packetType = buffer.getByte();
    name = buffer.getString(m_size - 1);
  }
};
// send after success create user account
class CreateProfileImage : public Packet {
  // 32 x 32 image with rbg color in int
 private:
  std::vector<int> profileImage;

 public:
  CreateProfileImage(const std::vector<int>& color)
      : Packet(::getPacketSize(CREATE_PROFILE_PICTURE, nullptr),
               CREATE_PROFILE_PICTURE),
        profileImage(color) {}
  CreateProfileImage(byte* data, const int& size)
      : Packet(data, size), profileImage(1024) {}
  const std::vector<int>& getColors() const { return profileImage; }
  void serialization() override {
    for (int i = 0; i < (int)profileImage.size(); i++) {
      buffer.putInt(profileImage[i]);
    }
  }
  void derserialization() override {
    packetType = buffer.getByte();
    for (int i = 0; i < 1024; i++) {
      profileImage[i] = buffer.getInt();
    }
  }
};

class CreateARoomPacket : public Packet {
 private:
  std::string roomName;

 public:
  CreateARoomPacket(const std::string& name)
      : Packet(::getPacketSize(CREATE_ROOM_PACKET, (size_t[]){name.length()}),
               CREATE_ROOM_PACKET),
        roomName(name) {}
  CreateARoomPacket(byte* data, const int& size) : Packet(data, size) {}
  const std::string& getRoomName() const { return roomName; }
  void serialization() override {
    buffer.putByte(packetType);
    buffer.putString(roomName);
  }
  void derserialization() override {
    packetType = buffer.getByte();
    roomName = buffer.getString(m_size - 1);
  }
};

class JoinARoomPacket : public Packet {
  unsigned int roomId;

 public:
  JoinARoomPacket(const unsigned int& id)
      : Packet(::getPacketSize(JOIN_A_ROOM_PACKET, nullptr),
               JOIN_A_ROOM_PACKET),
        roomId(id) {}
  JoinARoomPacket(byte* data, const int& size) : Packet(data, size) {}
  const unsigned int& getRoomId() const { return roomId; }
  void serialization() override {
    buffer.putByte(packetType);
    buffer.putInt(roomId);
  }
  void derserialization() override {
    packetType = buffer.getByte();
    roomId = buffer.getInt();
  }
};
class SendDataToEveryoneInARoomPacket : public Packet {
  byte* data;
  std::string sData;
  int size;
  unsigned int roomNumber;

 public:
  SendDataToEveryoneInARoomPacket(byte* d, const int& s,
                                  const unsigned int& roomN)
      : Packet(
            ::getPacketSize(SEND_DATA_TO_EVERYONE_IN_A_ROOM_PACKET, (int[]){s}),
            SEND_DATA_TO_EVERYONE_IN_A_ROOM_PACKET),
        data(d),
        size(s),
        roomNumber(roomN) {}
  SendDataToEveryoneInARoomPacket(byte* data, const int& size)
      : Packet(data, size) {}
  const int& getSize() const { return size; }
  const std::string& getData() const { return sData; }
  const unsigned int& getRoomNumber() { return roomNumber; }
  void serialization() override {
    buffer.putByte(packetType);
    buffer.putInt(roomNumber);
    buffer.putInt(size);
    sData = std::string((const char*)data);
    buffer.putString(sData);
  }
  void derserialization() override {
    packetType = buffer.getByte();
    roomNumber = buffer.getInt();
    size = buffer.getInt();
    sData = buffer.getString(size);
  }
};

class SendDataToUsersInARoomPacket : public Packet {
  byte* data;
  std::string sdata;
  int dataSize;
  unsigned int roomNumber;
  unsigned int userSize;
  std::vector<unsigned int> users;

 public:
  SendDataToUsersInARoomPacket(byte* d, const int& s, const unsigned int& roomN,
                               const std::vector<unsigned int> u)
      : Packet(::getPacketSize(SEND_DATA_TO_USERS_IN_A_ROOM_PACKET,
                               (int[]){s, (int)u.size()}),
               SEND_DATA_TO_USERS_IN_A_ROOM_PACKET),
        data(d),
        dataSize(s),
        roomNumber(roomN),
        userSize(u.size()),
        users(u) {}
  SendDataToUsersInARoomPacket(byte* data, const int& size)
      : Packet(data, size) {}
  const int& getSize() const { return dataSize; }
  const std::string& getData() const { return sdata; }
  const unsigned int& getRoomNumber() const { return roomNumber; }
  const unsigned int& getUserSize() const { return userSize; }
  const std::vector<unsigned int>& getUsers() const { return users; }
  void serialization() override {
    buffer.putByte(packetType);
    buffer.putInt(roomNumber);
    buffer.putInt(dataSize);
    buffer.putInt((unsigned int)users.size());
    std::string d((const char*)data);
    buffer.putString(d);
    for (int i = 0; i < (int)users.size(); i++) buffer.putInt(users[i]);
  }
  void derserialization() override {
    packetType = buffer.getByte();
    roomNumber = buffer.getInt();
    dataSize = buffer.getInt();
    userSize = buffer.getInt();
    users.reserve(userSize);
    sdata = buffer.getString(dataSize);
    for (unsigned int i = 0; i < userSize; i++) {
      users.push_back(buffer.getInt());
    }
  }
};

class LeaveARoomPacket : public Packet {
  unsigned int roomId;

 public:
  LeaveARoomPacket(const unsigned int& roomN)
      : Packet(::getPacketSize(LEAVE_A_ROOM_PACKET, nullptr),
               LEAVE_A_ROOM_PACKET),
        roomId(roomN) {}
  LeaveARoomPacket(byte* data, const int& size) : Packet(data, size) {}
  const unsigned int& getRoomId() const { return roomId; }
  void serialization() override {
    buffer.putByte(packetType);
    buffer.putInt(roomId);
  }
  void derserialization() override {
    packetType = buffer.getByte();
    roomId = buffer.getInt();
  }
};
class AnUserEnterARoom : public Packet {
 private:
  std::string name;
  std::vector<int> profileImage;
  unsigned int id;

 public:
  AnUserEnterARoom(const std::string& n, const std::vector<int>& p,
                   const unsigned int& i)
      : Packet(::getPacketSize(AN_USER_JOIN_A_ROOM, (size_t[]){n.length()}),
               AN_USER_JOIN_A_ROOM),
        name(n),
        profileImage(p),
        id(i) {}
  AnUserEnterARoom(byte* data, const int& size)
      : Packet(data, size), profileImage(1024) {}
  const std::string& getName() const { return name; }
  const std::vector<int>& getProfileImage() const { return profileImage; }
  const unsigned int& getId() const { return id; }
  void serialization() override {
    buffer.putByte(packetType);
    buffer.putInt(id);
    buffer.putString(name);
    for (int i = 0; i < (int)profileImage.size(); i++) {
      buffer.putInt(profileImage[i]);
    }
  }
  void derserialization() override {
    packetType = buffer.getByte();
    id = buffer.getInt();
    for (int i = 0; i < (int)profileImage.size(); i++) {
      profileImage[i] = buffer.getInt();
    }
  }
};

class AnUserLeaveARoom : public Packet {
 private:
  unsigned int id;

 public:
  AnUserLeaveARoom(const unsigned int& i)
      : Packet(::getPacketSize(AN_USER_LEAVE_A_ROOM_PACKET, nullptr),
               AN_USER_LEAVE_A_ROOM_PACKET),
        id(i) {}
  AnUserLeaveARoom(byte* data, const int& size) : Packet(data, size) {}
  const unsigned int& getId() const { return id; }
  void serialization() override {
    buffer.putByte(packetType);
    buffer.putInt(id);
  }
  void derserialization() override {
    packetType = buffer.getByte();
    id = buffer.getInt();
  }
};

#endif

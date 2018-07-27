#ifndef PACKET_H
#define PACKET_H
#include <map>
#include <string>
#include <vector>
#include "Buffer.h"
#include "Config.h"
#include "User.h"
const byte CREATE_USER_PACKET = 'q';
const byte CREATE_PROFILE_PICTURE = 'w';
const byte CREATE_ROOM_PACKET = 'e';
const byte JOIN_A_ROOM_PACKET = 'r';
const byte SEND_DATA_TO_EVERYONE_IN_A_ROOM_PACKET = 't';
const byte SEND_DATA_TO_USERS_IN_A_ROOM_PACKET = 'y';
const byte LEAVE_A_ROOM_PACKET = 'u';

// server side
const byte AN_USER_LEAVE_A_ROOM_PACKET = 'i';
const byte AN_USER_JOIN_A_ROOM = 'o';
const byte NEW_MESSAGE_FROM_USER = 'p';
const byte JOIN_A_ROOM_RESPOND_PACKET = 'a';
const byte CREATE_A_ROOM_RESPOND_PACKET = 's';
const byte SERVER_ERROR_MSG = 'd';

const byte ROOM_RESPOND_NOT_EXIST = -1;
const byte ROOM_EXIST = 1;

const byte CREATE_ROOM_OK = 1;
const byte CREATE_ROOM_FAILED = -1;

inline byte getPacketType(const byte* data) { return data[0]; }

inline int getPacketSize(const byte& type, void* data) {
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
      size_t* array = (size_t*)data;
      size_t nameLegnth = array[0];
      return 1 + nameLegnth;
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
    case NEW_MESSAGE_FROM_USER: {
      size_t* array = (size_t*)data;
      size_t msgLen = array[0];
      // packettype(1) + msglen(4) + from(4) + data
      return 1 + 4 + 4 + 4 + msgLen;
    }
    case JOIN_A_ROOM_RESPOND_PACKET: {
      if (data == nullptr)
        return 2;
      else {
        std::map<unsigned int, User*>* d = (std::map<unsigned int, User*>*)data;
        int totalUsers = d->size();
        // number of variable of int to store user name length
        uint32_t totalByteToStoreVariableNameLength = totalUsers * 4;
        uint32_t totalByteToStoreUserId = totalUsers * 4;
        uint32_t totalStringLength = 0;
        for (auto it = d->begin(); it != d->end(); it++) {
          // find total string length
          totalStringLength += it->second->getName().length();
        }
        return 1 +  4 + 4 + totalByteToStoreVariableNameLength +
               totalByteToStoreUserId + totalStringLength;
      }
    }
    case CREATE_A_ROOM_RESPOND_PACKET: {
      return 2;
    }
    case SERVER_ERROR_MSG: {
      size_t* array = (size_t*)data;
      size_t length = array[0];
      return length + 1;
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
  Packet(const byte* data, const int& size)
      : buffer(data, size), m_size(size) {}
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
  CreateUserPacket(const byte* buffer, const int& size)
      : Packet(buffer, size) {}
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
  CreateProfileImage(const byte* data, const int& size)
      : Packet(data, size), profileImage(1024) {}
  const std::vector<int>& getColors() const { return profileImage; }
  void serialization() override {
    buffer.putByte(packetType);
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
  CreateARoomPacket(const byte* data, const int& size) : Packet(data, size) {}
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
  std::string name;

 public:
  JoinARoomPacket(const std::string& n)
      : Packet(::getPacketSize(JOIN_A_ROOM_PACKET, (size_t[]){n.size()}),
               JOIN_A_ROOM_PACKET),
        name(n) {}
  JoinARoomPacket(byte* data, const int& size) : Packet(data, size) {}
  const std::string& getName() const { return name; }
  void serialization() {
    buffer.putByte(packetType);
    buffer.putString(name);
  }
  void derserialization() {
    packetType = buffer.getByte();
    name = buffer.getString(m_size - 1);
  }
};
class SendDataToEveryoneInARoomPacket : public Packet {
  std::string sData;
  int size;
  unsigned int roomNumber;

 public:
  SendDataToEveryoneInARoomPacket(const std::string& data, const int& s,
                                  const unsigned int& roomN)
      : Packet(
            ::getPacketSize(SEND_DATA_TO_EVERYONE_IN_A_ROOM_PACKET, (int[]){s}),
            SEND_DATA_TO_EVERYONE_IN_A_ROOM_PACKET),
        sData(data),
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
class NewMsgFromUser : public Packet {
 private:
  std::string data;
  unsigned int from;
  unsigned int id;
  unsigned int msgLen;

 public:
  NewMsgFromUser(const std::string& d, const unsigned int& f,
                 const unsigned int& i)
      : Packet(::getPacketSize(NEW_MESSAGE_FROM_USER, (size_t[]){d.size()}),
               NEW_MESSAGE_FROM_USER),
        data(d),
        from(f),
        id(i),
        msgLen((int)d.size()) {}
  NewMsgFromUser(byte* data, const int& size) : Packet(data, size) {}
  const std::string& getData() const { return data; }
  const unsigned int& getSendFrom() const { return from; }
  const unsigned int& getMsgLen() const { return msgLen; }
  const unsigned int& getRoomId() const { return id; }
  void serialization() override {
    buffer.putByte(packetType);
    buffer.putInt(from);
    buffer.putInt(id);
    buffer.putInt(msgLen);
    buffer.putString(data);
  }
  void derserialization() override {
    packetType = buffer.getByte();
    from = buffer.getInt();
    id = buffer.getInt();
    msgLen = buffer.getInt();
    data = buffer.getString(msgLen);
  }
};
class JoinARoomRespondPacket : public Packet {
 private:
  uint8_t status;
  unsigned int roomId;
  std::string roomName; 
  std::map<unsigned int, User*>* users;
  std::map<unsigned int, std::string> usersForDeserialization;
  uint32_t totalUsers;

 public:
  JoinARoomRespondPacket(const uint8_t& s, const unsigned int& id , const std::string& r ,std::map<unsigned int, User*>* u)
      : Packet(::getPacketSize(JOIN_A_ROOM_RESPOND_PACKET, u),
               JOIN_A_ROOM_RESPOND_PACKET),
        status(s),
        roomId(id),
        roomName(r)
  {
    users = u;
    if (u != nullptr)
      totalUsers = u->size();
    else
      totalUsers = 0;
  }
  JoinARoomRespondPacket(byte* data, const int& size) : Packet(data, size) {}
  uint8_t getStatus() const { return status; }
  const uint32_t& getTotalUsers() const { return totalUsers; }
  const std::map<unsigned int, std::string>& getUsers() const {
    return usersForDeserialization;
  }

  void serialization() override {
    buffer.putByte(packetType);
    if (status == ROOM_RESPOND_NOT_EXIST) {
      buffer.putByte(status);
      return;
    }
    buffer.putInt(roomId);
    buffer.putInt(totalUsers);
    if (users != nullptr) {
      for (auto it = users->begin(); it != users->end(); it++) {
        // serialize id
        buffer.putInt(it->first);
        // serialize nameLength
        buffer.putInt(it->second->getName().length());
        // serialize name
        buffer.putString(it->second->getName());
      }
    }
  }
  void derserialization() override {
    packetType = buffer.getByte();
    if (m_size == 2) {
      status = buffer.getByte();
      return;
    }
    // room exist
    else {
      // deserialize totalUsrs
      totalUsers = buffer.getInt();

      for (int i = 0; i < (int)totalUsers; i++) {
        // deserialize user id
        unsigned int id = buffer.getInt();
        // deserialize nameLen
        unsigned int nameLen = buffer.getInt();
        // deserialize name
        std::string name = buffer.getString(nameLen);
        // push to container
        usersForDeserialization[id] = name;
      }
    }
  }
};
class CreateARoomRespondPacket : public Packet {
 private:
  byte status;

 public:
  CreateARoomRespondPacket(const byte& s)
      : Packet(::getPacketSize(CREATE_A_ROOM_RESPOND_PACKET, nullptr),
               CREATE_A_ROOM_RESPOND_PACKET),
        status(s) {}
  CreateARoomRespondPacket(byte* data, const int& size) : Packet(data, size) {}
  const byte& getStatus() { return status; }
  void serialization() override {
    buffer.putByte(packetType);
    buffer.putByte(status);
  }
  void derserialization() override {
    packetType = buffer.getByte();
    status = buffer.getByte();
  }
};
class ServerErrorMsg : public Packet {
 private:
  std::string message;

 public:
  ServerErrorMsg(const std::string& msg)
      : Packet(::getPacketSize(SERVER_ERROR_MSG, (size_t[]){msg.length()}),
               SERVER_ERROR_MSG),
        message(msg) {}
  ServerErrorMsg(const byte* data, const int& size) : Packet(data, size) {
    message.reserve(size - 1);
  }
  void serialization() override {
    buffer.putByte(packetType);
    buffer.putString(message);
  }
  void derserialization() override {
    packetType = buffer.getByte();
    message = buffer.getString(m_size - 1);
  }
};
#endif

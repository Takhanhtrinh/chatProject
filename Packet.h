#ifndef PACKET_H
#define PACKET_H
#include <assert.h>
#include <map>
#include <string>
#include <vector>
#include "Buffer.h"
#include "Config.h"
#include "User.h"

const byte CLIENT_INIT_PACKET = 'q';
const byte SERVER_INIT_PACKET = 'Q';
const byte CLIENT_SET_PROFILE_IMAGE_PACKET = 'w';
const byte SERVER_SET_PROFILE_IMAGE_PACKET = 'W';
const byte CLIENT_SEARCH_ROOM_PACKET = 'e';
const byte SERVER_SEARCH_ROOM_PACKET = 'E';
const byte CLIENT_JOIN_ROOM_PACKET = 'r';
const byte SERVER_JOIN_ROOM_PACKET = 'R';
const byte CLIENT_SEND_TEXT_MESSAGE = 't';
const byte SERVER_SEND_TEXT_MESSAGE_RESPOND = 'T';
const byte CLIENT_SEND_PIXEL_MESSAGE = 'y';
const byte SERVER_SEND_PIXEL_MESSAGE_RESPOND = 'Y';
const byte SERVER_NEW_TEXT_MESSAGE = 'u';
const byte SERVER_NEW_IMAGE_MESSAGE = 'U';
const byte SERVER_NEW_USER_ENTER_ROOM = 'o';
const byte SERVER_NEW_USER_LEAVE_ROOM = 'O';
const byte CLIENT_REMOVE_ROOM = 'p';
const byte SERVER_REMOVE_ROOM = 'P';
const byte SERVER_USERS_IN_A_ROOM = 'A';

const byte SERVER_INIT_PACKET_STATUS_SUCCESS = 1;
const byte SERVER_INIT_PACKET_STATUS_FAIL = 2;

const byte SERVER_ADD_ROOM_FAILED = 2;
const byte SERVER_ADD_ROOM_SUCCESS = 1;

const byte SERVER_SEND_TEXT_MESSAGE_RESPOND_OK = 1;
const byte SERVER_SEND_PIXEL_MESSAGE_RESPOND_OK = 1;

const byte SERVER_CREATE_A_ROOM_SUCCESS = 1;
const byte SERVER_CREATE_A_ROOM_FAIL_ALREADY_EXIST = 0;

inline uint64_t getServerCurrentTime() {
  using namespace std::chrono;
  uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::system_clock::now().time_since_epoch())
                     .count();
  return now;
}
inline byte getPacketType(const byte* data) { return data[0]; }
inline bool checkValidPacketSize(const byte& type, const size_t& length) {
  switch (type) {
    case CLIENT_INIT_PACKET:
      return length > 1 && length < MAX_NAME_LENGTH + 1;
    case CLIENT_SEARCH_ROOM_PACKET:
      return length > 1 && length < MAX_ROOM_NAME + 1;
    case CLIENT_SEND_TEXT_MESSAGE:
      return length > 1;
    case CLIENT_SEND_PIXEL_MESSAGE:
      return length > MAX_PIXEL_SIZE;
  }
  return false;
}

inline int getPacketSize(const byte& packetType, const void* data) {
  switch (packetType) {
    case CLIENT_INIT_PACKET: {
      int nameLenth = *(const int*)data;
      return 1 + nameLenth;
      break;
    }
    case SERVER_INIT_PACKET: {
      const int* array = (const int*)data;
      int status = array[0];
      if (status == SERVER_INIT_PACKET_STATUS_FAIL) {
        int statusLen = array[1];
        return 1 + statusLen + 1;
      } else {
        return 2;
      }
      break;
    }
    case CLIENT_SEARCH_ROOM_PACKET: {
      int roomLength = *(int*)data;
      return 1 + roomLength;
    }
    case SERVER_SEARCH_ROOM_PACKET: {
      byte* array = (byte*)data;
      byte status = array[0];
      if (status == SERVER_ADD_ROOM_SUCCESS) return 2 + 4;
      return 2;
    }
    case CLIENT_SEND_TEXT_MESSAGE: {
      int* array = (int*)data;
      int msgLen = array[0];
      int numberOfUsers = array[1];
#ifdef DEBUG
      printf("msgLen: %d\n", msgLen);
      printf("numberOfUsers: %d\n", numberOfUsers);
#endif
      // packetType + roomId + msgLen + msg
      // + numberOfUsers + userIds;
      return 1 + 4 + 4 + msgLen + 4 + numberOfUsers * 4;
      break;
    }
    case SERVER_SEND_TEXT_MESSAGE_RESPOND: {
      return 2;
    }
    case CLIENT_SEND_PIXEL_MESSAGE: {
      int numberOfUsers = *(int*)data;
      return MAX_PIXEL_SIZE * 4 + 4 + 4 + numberOfUsers * 4 + 1;
    }
    case SERVER_SEND_PIXEL_MESSAGE_RESPOND:
      return 2;
    case SERVER_NEW_TEXT_MESSAGE: {
      size_t size = *(size_t*)data;
      return 1 + 4 + 4 + 4 + 8 + size;
    }
    case SERVER_NEW_IMAGE_MESSAGE:
      return 1 + 4 + 4 + 4 + 8 + MAX_PIXEL_SIZE * 4;
    case SERVER_NEW_USER_ENTER_ROOM: {
      int nameLen = *(int*)data;
      return 1 + 4 + 4 + nameLen;
    }
    case SERVER_NEW_USER_LEAVE_ROOM:
      return 1 + 4 + 4;
    case SERVER_USERS_IN_A_ROOM: {
      const std::map<uint32_t, User*>* users =
          (const std::map<uint32_t, User*>*)data;
      uint32_t totalNameLength = 0;
      uint32_t totalUsers = users->size();
      for (auto it = users->begin(); it != users->end(); it++) {
        std::string name = it->second->getName();
        totalNameLength += name.length();
      }
      // packet type + numberOfUsers(4bytes) + roomId + namelength of all users
      // + usersId
      // + 1 byte for name length each user
      return 1 + 4 + 4 + totalNameLength + totalUsers * 4 + totalUsers;
    }
    case CLIENT_REMOVE_ROOM: {
      return 5;
    }
  }
  assert(1 == 0 && "PACKET TYPE NOT SPECIFIC\n");

  return -1;
}

struct Packet {
  Buffer buffer;
  int packetSize;
  byte type;
  Packet(const int& size, const byte& t)
      : buffer(size), packetSize(size), type(t) {
#ifdef DEBUG
    printf("packetSize for serialize: %d\n", packetSize);
#endif
  }
  Packet(const byte b[], const int& size) : buffer(b, size), packetSize(size) {
#ifdef DEBUG
    printf("packetSize for deSerialize: %d\n", packetSize);
#endif
  }
  virtual void serialize() = 0;
  virtual bool deSerialize() = 0;
  virtual ~Packet() {}
};

struct Client_Init_Packet : public Packet {
  std::string name;
  Client_Init_Packet(const std::string& n)
      : Packet(::getPacketSize(CLIENT_INIT_PACKET, (size_t[]){n.length()}),
               CLIENT_INIT_PACKET),
        name(n) {}
  Client_Init_Packet(const byte b[], const int& size) : Packet(b, size) {}
  void serialize() override {
    buffer.putByte(type);
    buffer.putString(name);
  }
  bool deSerialize() override {
    try {
      type = buffer.getByte();
      name = buffer.getString(packetSize - 1);

    } catch (const Buffer_Exception& e) {
      printf("Client_Init_Packet: deserialize error\n");
      return false;
    }
    return true;
  }
};

struct Server_Init_Packet : public Packet {
  byte status;
  std::string statusStr;
  Server_Init_Packet(const byte& s, const std::string& str)
      : Packet(
            ::getPacketSize(SERVER_INIT_PACKET, (int[]){s, (int)str.length()}),
            SERVER_INIT_PACKET),
        status(s),
        statusStr(str) {}
  Server_Init_Packet(const byte b[], const int& size) : Packet(b, size) {}
  void serialize() override {
    buffer.putByte(type);
    buffer.putByte(status);
    printf("status: %d\n", status);
    if (status == SERVER_INIT_PACKET_STATUS_FAIL) {
      buffer.putString(statusStr);
    }
  }
  bool deSerialize() override {
    try {
      type = buffer.getByte();
      status = buffer.getByte();
      if (status == SERVER_INIT_PACKET_STATUS_FAIL)
        statusStr = buffer.getString(packetSize - 2);

    } catch (Buffer_Exception& e) {
      printf("deSerialize error\n");
      return false;
    }
    return true;
  }
};
struct Client_Search_Room_Packet : public Packet {
  std::string roomName;
  Client_Search_Room_Packet(const std::string& n)
      : Packet(
            ::getPacketSize(CLIENT_SEARCH_ROOM_PACKET, (size_t[]){n.length()}),
            CLIENT_SEARCH_ROOM_PACKET),
        roomName(n) {}
  Client_Search_Room_Packet(const byte b[], const int& s) : Packet(b, s) {}
  void serialize() override {
    buffer.putByte(type);
    buffer.putString(roomName);
  }
  bool deSerialize() override {
    try {
      type = buffer.getByte();
      roomName = buffer.getString(packetSize - 1);

    } catch (const Buffer_Exception& e) {
      printf("deSerialize error\n");
      return false;
    }
    return true;
  }
};
struct Server_Search_Room_Packet : public Packet {
  byte status;
  uint32_t roomId;
  Server_Search_Room_Packet(const byte& s, const uint32_t& id)
      : Packet(::getPacketSize(SERVER_SEARCH_ROOM_PACKET, (byte[]){(byte)s}),
               SERVER_SEARCH_ROOM_PACKET),
        status(s),
        roomId(id) {}
  void serialize() override {
    buffer.putByte(type);
    buffer.putByte(status);
    if (status == SERVER_ADD_ROOM_SUCCESS) {
      std::cout << "roomId: " << roomId << std::endl;
      buffer.putInt(roomId);
    }
  }
  bool deSerialize() override {
    try {
      type = buffer.getByte();
      status = buffer.getByte();
      if (status == SERVER_ADD_ROOM_SUCCESS) {
        roomId = buffer.getInt();
      }
    } catch (const Buffer_Exception& e) {
      printf("deSerialize error\n");
      return false;
    }
    return true;
  }
};

struct Client_Send_Text_Message : public Packet {
  std::string msg;
  uint32_t roomId;
  std::vector<unsigned int> sendTo;
  Client_Send_Text_Message(const std::string& m, const uint32_t& id,
                           const std::vector<unsigned int>& s)
      : Packet(::getPacketSize(CLIENT_SEND_TEXT_MESSAGE,
                               (int[]){(int)m.length(), (int)s.size()}),
               CLIENT_SEND_TEXT_MESSAGE),
        msg(m),
        roomId(id),
        sendTo(s) {}
  Client_Send_Text_Message(const byte b[], const int& s) : Packet(b, s) {}
  void serialize() override {
    buffer.putByte(type);
    buffer.putInt(roomId);
    buffer.putInt(msg.length());
    buffer.putString(msg);
    // put number of user to send to
    buffer.putInt((int)sendTo.size());
    for (int i = 0; i < (int)sendTo.size(); i++) {
      int userId = sendTo[i];
      buffer.putInt(userId);
    }
  }
  bool deSerialize() override {
    try {
      type = buffer.getByte();
      printf("type: %d\n", type);
      roomId = buffer.getInt();
      printf("roomId: %ud\n", roomId);
      uint32_t msgLen = buffer.getInt();
      printf("msgLen: %d\n", msgLen);
      msg = buffer.getString(msgLen);
      printf("msg: %s\n", msg.c_str());
      uint32_t numberOfUsers = buffer.getInt();
      printf("numberOfUsers: %d\n", numberOfUsers);
      for (uint32_t i = 0; i < numberOfUsers; i++) {
        uint32_t userId = buffer.getInt();
        sendTo.push_back(userId);
      }
    } catch (const Buffer_Exception& e) {
      printf("deSerialize error\n");
      return false;
    }
    return true;
  }
};
struct Server_Send_Text_Message_Respond : public Packet {
  byte status;
  Server_Send_Text_Message_Respond(const byte& s)
      : Packet(::getPacketSize(SERVER_SEND_TEXT_MESSAGE_RESPOND, nullptr),
               SERVER_SEND_TEXT_MESSAGE_RESPOND),
        status(s) {}
  Server_Send_Text_Message_Respond(const byte b[], const int& s)
      : Packet(b, s) {}
  void serialize() override {
    buffer.putByte(type);
    buffer.putByte(status);
  }
  bool deSerialize() override {
    try {
      type = buffer.getByte();
      status = buffer.getByte();
    } catch (const std::exception& e) {
      printf("deserialize error\n");
      return false;
    }
    return true;
  }
};

struct Client_Send_Pixel_Message : public Packet {
  std::vector<byte> msg;
  int roomId;
  std::vector<unsigned int> sendTo;
  Client_Send_Pixel_Message(const std::vector<byte>& m, const int& id,
                            const std::vector<unsigned int>& t)
      : Packet(
            ::getPacketSize(CLIENT_SEND_PIXEL_MESSAGE, (int[]){(int)t.size()}),
            CLIENT_SEND_PIXEL_MESSAGE),
        msg(m),
        roomId(id),
        sendTo(t) {}
  Client_Send_Pixel_Message(const byte b[], const int& s)
      : Packet(b, s), msg(MAX_PIXEL_SIZE * 4) {}
  void serialize() override {
    buffer.putByte(type);
    buffer.putInt(roomId);
    int numberOfUsers = (int)sendTo.size();
    buffer.putInt(numberOfUsers);
    for (int i = 0; i < numberOfUsers; i++) {
      buffer.putInt(sendTo[i]);
    }
    for (int i = 0; i < MAX_PIXEL_SIZE * 4; i++) {
      buffer.putByte(msg[i]);
    }
  }
  bool deSerialize() override {
    try {
      type = buffer.getByte();
      roomId = buffer.getInt();
      int numberOfUsers = buffer.getInt();
      for (int i = 0; i < numberOfUsers; i++) {
        int id = buffer.getInt();
        sendTo.push_back(id);
      }
      for (int i = 0; i < MAX_PIXEL_SIZE * 4; i += 4) {
        byte r = buffer.getByte();
        byte g = buffer.getByte();
        byte b = buffer.getByte();
        byte a = buffer.getByte();
        msg[i] = r;
        msg[i + 1] = g;
        msg[i + 2] = b;
        msg[i + 3] = a;
      }
    } catch (const Buffer_Exception& e) {
      printf("deSerialize error\n");
      return false;
    }
    return true;
  }
};

struct Server_Send_New_Message : public Packet {
  std::string msg;
  unsigned int roomId;
  unsigned int fromId;
  unsigned int msgId;
  uint64_t timeStamp;
  Server_Send_New_Message(const std::string& m, const unsigned int& id,
                          const unsigned int& from, const unsigned int& msgid)
      : Packet(::getPacketSize(SERVER_NEW_TEXT_MESSAGE, (size_t[]){m.length()}),
               SERVER_NEW_TEXT_MESSAGE),
        msg(m),
        roomId(id),
        fromId(from),
        msgId(msgid) {}
  Server_Send_New_Message(const byte b[], const int& s) : Packet(b, s) {}
  void serialize() override {
    buffer.putByte(type);
    buffer.putInt(roomId);
    buffer.putInt(fromId);
    timeStamp = getServerCurrentTime();
    buffer.putLong(timeStamp);
    buffer.putInt(msgId);
    buffer.putString(msg);
  }
  bool deSerialize() override {
    try {
      type = buffer.getByte();
      roomId = buffer.getInt();
      fromId = buffer.getInt();
      timeStamp = buffer.getLong();
      msgId = buffer.getInt();
      msg = buffer.getString(packetSize - 1 - 4 - 4 - 8 - 4);

    } catch (const Buffer_Exception& e) {
      printf("deSerialize error\n");
      return false;
    }
    return true;
  }
};
struct Server_Send_Pixel_Message : public Packet {
  std::vector<byte> pixel;
  unsigned int roomId;
  unsigned int fromId;
  unsigned int msgId;
  uint64_t timeStamp;
  Server_Send_Pixel_Message(const std::vector<byte>& p, const unsigned int& rid,
                            const unsigned int& fid, const unsigned int mid)
      : Packet(::getPacketSize(SERVER_NEW_IMAGE_MESSAGE, nullptr),
               SERVER_NEW_IMAGE_MESSAGE),
        pixel(p),
        roomId(rid),
        fromId(fid),
        msgId(mid) {}
  Server_Send_Pixel_Message(const byte b[], const int& size)
      : Packet(b, size), pixel(MAX_PIXEL_SIZE * 4) {}
  void serialize() override {
    buffer.putByte(type);
    buffer.putInt(roomId);
    buffer.putInt(fromId);
    timeStamp = ::getServerCurrentTime();
    buffer.putLong(timeStamp);
    buffer.putInt(msgId);
    for (int i = 0; i < MAX_PIXEL_SIZE * 4; i++) {
      buffer.putByte(pixel[i]);
    }
  }
  bool deSerialize() override {
    try {
      type = buffer.getByte();
      roomId = buffer.getInt();
      fromId = buffer.getInt();
      timeStamp = buffer.getLong();
      msgId = buffer.getInt();
      for (int i = 0; i < MAX_PIXEL_SIZE * 4; i++) {
        pixel[i] = buffer.getByte();
      }
    } catch (const Buffer_Exception& e) {
      printf("deSerialize fail\n");
      return false;
    }
    return true;
  }
};
struct Server_New_User_Enter_Room : public Packet {
  std::string name;
  unsigned int id;
  unsigned int roomId;
  Server_New_User_Enter_Room(const std::string& n, const unsigned int& i,
                             const unsigned int& rid)
      : Packet(
            ::getPacketSize(SERVER_NEW_USER_ENTER_ROOM, (size_t[]){n.length()}),
            SERVER_NEW_USER_ENTER_ROOM),
        name(n),
        id(i),
        roomId(rid) {}
  Server_New_User_Enter_Room(const byte b[], const int& s) : Packet(b, s) {}
  void serialize() override {
    buffer.putByte(type);
    buffer.putInt(roomId);
    buffer.putInt(id);
    buffer.putString(name);
  }
  bool deSerialize() override {
    try {
      type = buffer.getByte();
      roomId = buffer.getInt();
      id = buffer.getInt();
      name = buffer.getString(packetSize - 9);
    } catch (const Buffer_Exception& e) {
      printf("deSerialize falil\n");
      return false;
    }
    return true;
  }
};
struct Server_User_Left_Room : public Packet {
  unsigned int roomId;
  unsigned int userId;
  Server_User_Left_Room(const unsigned int& rid, const unsigned int& uid)
      : Packet(::getPacketSize(SERVER_NEW_USER_LEAVE_ROOM, nullptr),
               SERVER_NEW_USER_LEAVE_ROOM),
        roomId(rid),
        userId(uid) {}
  Server_User_Left_Room(const byte b[], const int& s) : Packet(b, s) {}
  void serialize() override {
    buffer.putByte(type);
    buffer.putInt(roomId);
    buffer.putInt(userId);
  }
  bool deSerialize() override {
    try {
      type = buffer.getByte();
      roomId = buffer.getInt();
      userId = buffer.getInt();

    } catch (const Buffer_Exception& e) {
      printf("deserialize fail\n");
      return false;
    }
    return true;
  }
};
struct Server_users_in_a_room : public Packet {
  const std::map<uint32_t, User*>* users;
  uint32_t roomId;
  Server_users_in_a_room(const std::map<uint32_t, User*>* u, const uint32_t& id)
      : Packet(::getPacketSize(SERVER_USERS_IN_A_ROOM, u),
               SERVER_USERS_IN_A_ROOM),
        users(u),
        roomId(id) {}
  void serialize() override {
    buffer.putByte(type);
    buffer.putInt(roomId);
    buffer.putInt(users->size());
    for (auto it = users->begin(); it != users->end(); it++) {
      std::string name = it->second->getName();
      byte nameLength = (byte)name.length();
      uint32_t id = it->second->getId();
      buffer.putByte(nameLength);
      buffer.putString(name);
      buffer.putInt(id);
    }
  }
  bool deSerialize() override { return false; }
};
struct Client_Remove_Room : public Packet {
  uint32_t roomId;
  Client_Remove_Room(const uint32_t& rid)
      : Packet(::getPacketSize(CLIENT_REMOVE_ROOM, nullptr),
               CLIENT_REMOVE_ROOM),
        roomId(rid) {}
  void serialize() override {
    buffer.putByte(type);
    buffer.putInt(roomId);
  }
  bool deSerialize() override {
    try {
      type = buffer.getByte();
      roomId = buffer.getInt();

    } catch (const Buffer_Exception& e) {
      printf("deserialize false\n");
      return false;
    }
    return true;
  }
};
#endif

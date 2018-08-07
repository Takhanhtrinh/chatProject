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
const byte CLIENT_CREATE_A_ROOM = 'a';
const byte SERVER_CREATE_A_ROOM = 'A';

const byte SERVER_INIT_PACKET_STATUS_SUCCESS = 1;
const byte SERVER_INIT_PACKET_STATUS_FAIL = 2;

const byte SERVER_SEARCH_ROOM_ROOM_DOESNT_EXIST = 0;
const byte SERVER_SEARCH_ROOM_ROOM_EXIST = 1;

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
    case CLIENT_CREATE_A_ROOM:
      return length > 1 && length < MAX_ROOM_NAME + 1;
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
      int* array = (int*)data;
      int msgLen = array[0];
      int status = array[1];
      if (status == SERVER_SEARCH_ROOM_ROOM_EXIST) return 2;
      return 2 + msgLen;
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
    case CLIENT_CREATE_A_ROOM: {
      int roomNameLen = *(int*)data;
      return 1 + roomNameLen;
    }
    case SERVER_CREATE_A_ROOM: {
      int* array = (int*)data;
      int status = array[0];
      int nameLenth = array[1];
      if (status == SERVER_CREATE_A_ROOM_FAIL_ALREADY_EXIST)
        return 2;
      else {
        return 2 + 4 + nameLenth;
      }
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
  virtual void deSerialize() = 0;
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
  void deSerialize() override {
    type = buffer.getByte();
    name = buffer.getString(packetSize - 1);
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
  void deSerialize() override {
    type = buffer.getByte();
    status = buffer.getByte();
    if (status == SERVER_INIT_PACKET_STATUS_FAIL)
      statusStr = buffer.getString(packetSize - 2);
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
  void deSerialize() override {
    type = buffer.getByte();
    roomName = buffer.getString(packetSize - 1);
  }
};
struct Server_Search_Room_Packet : public Packet {
  std::string msgError;
  byte status;
  Server_Search_Room_Packet(const std::string& msg, const byte& s)
      : Packet(::getPacketSize(SERVER_SEARCH_ROOM_PACKET,
                               (int[]){(int)msg.length(), s}),
               SERVER_SEARCH_ROOM_PACKET),
        msgError(msg),
        status(s) {}
  Server_Search_Room_Packet(const byte b[], const int& s) : Packet(b, s) {}
  void serialize() override {
    buffer.putByte(type);
    buffer.putByte(status);
    if (status == SERVER_SEARCH_ROOM_ROOM_EXIST) return;
    buffer.putString(msgError);
  }
  void deSerialize() override {
    type = buffer.getByte();
    status = buffer.getByte();
    if (status == SERVER_SEARCH_ROOM_ROOM_EXIST) return;
    msgError = buffer.getString(packetSize - 2);
  }
};

struct Client_Send_Text_Message : public Packet {
  std::string msg;
  int roomId;
  std::vector<unsigned int> sendTo;
  Client_Send_Text_Message(const std::string& m, const int& id,
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
  void deSerialize() override {
    type = buffer.getByte();
    roomId = buffer.getInt();
    int msgLen = buffer.getInt();
    msg = buffer.getString(msgLen);
    int numberOfUsers = buffer.getInt();
    for (int i = 0; i < numberOfUsers; i++) {
      int userId = buffer.getInt();
      sendTo.push_back(userId);
    }
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
  void deSerialize() override {
    type = buffer.getByte();
    status = buffer.getByte();
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
  void deSerialize() override {
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
  void deSerialize() override {
    type = buffer.getByte();
    roomId = buffer.getInt();
    fromId = buffer.getInt();
    timeStamp = buffer.getLong();
    msgId = buffer.getInt();
    msg = buffer.getString(packetSize - 1 - 4 - 4 - 8 - 4);
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
  void deSerialize() override {
    type = buffer.getByte();
    roomId = buffer.getInt();
    fromId = buffer.getInt();
    timeStamp = buffer.getLong();
    msgId = buffer.getInt();
    for (int i = 0; i < MAX_PIXEL_SIZE * 4; i++) {
      pixel[i] = buffer.getByte();
    }
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
  void deSerialize() override {
    type = buffer.getByte();
    roomId = buffer.getInt();
    id = buffer.getInt();
    name = buffer.getString(packetSize - 9);
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
  void deSerialize() override {
    type = buffer.getByte();
    roomId = buffer.getInt();
    userId = buffer.getInt();
  }
};
struct Client_Create_Room : public Packet {
  std::string name;
  Client_Create_Room(const std::string& n)
      : Packet(::getPacketSize(CLIENT_CREATE_A_ROOM, (size_t[]){n.length()}),
               CLIENT_CREATE_A_ROOM),
        name(n) {}
  Client_Create_Room(const byte b[], const int& s) : Packet(b, s) {}
  void serialize() override {
    buffer.putByte(type);
    buffer.putString(name);
  }
  void deSerialize() override {
    type = buffer.getByte();
    name = buffer.getString(packetSize - 1);
  }
};
struct Server_Create_Room_Respond : public Packet {
  byte status;
  unsigned int roomId;
  std::string roomName;
  Server_Create_Room_Respond(const byte& s, const unsigned int& rid,
                             const std::string& rn)
      : Packet(::getPacketSize(SERVER_CREATE_A_ROOM,
                               (int[]){(int)s, (int)rn.length()}),
               SERVER_CREATE_A_ROOM),
        status(s),
        roomId(rid),
        roomName(rn) {}
  Server_Create_Room_Respond(const byte b[], const int& s) : Packet(b, s) {}
  void serialize() override {
    buffer.putByte(type);
    buffer.putByte(status);
    if (status == SERVER_CREATE_A_ROOM_FAIL_ALREADY_EXIST) return;
    buffer.putInt(roomId);
    buffer.putString(roomName);
  }
  void deSerialize() override {
    type = buffer.getByte();
    status = buffer.getByte();
    if (status != SERVER_CREATE_A_ROOM_FAIL_ALREADY_EXIST) {
      roomId = buffer.getInt();
      roomName = buffer.getString(packetSize - 6);
    }
  }
};

#endif

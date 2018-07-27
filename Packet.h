#ifndef PACKET_H
#define PACKET_H
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
const byte CLIENT_SEND_MESSAGE_ONLY_TO = 'i';
const byte SERVER_SEND_MESSAGE_ONLY_TO = 'I';
const byte SERVER_NEW_USER_ENTER_ROOM = 'o';
const byte SERVER_NEW_USER_LEAVE_ROOM = 'O';
const byte CLIENT_REMOVE_ROOM = 'p';
const byte SERVER_REMOVE_ROOM = 'P';
const byte CLIENT_CREATE_A_ROOM = 'a';
const byte SERVER_CREATE_A_ROOM = 'A';




struct Packet {
  int packetSize;
  Buffer buffer;
  byte type;
  Packet(const int& size, const byte& t): packetSize(size), buffer(size), type(t){}
  virtual void serialize() = 0;
  virtual void deSerialize() = 0;
  virtual ~Packet() {}
};
#endif

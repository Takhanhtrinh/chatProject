#ifndef ROOM_H
#define ROOM_H
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "Packet.h"
class User;
class Room {
 private:
  std::map<unsigned int, User*> users;
  std::string m_name;
  unsigned int m_roomId;
  unsigned int msgId = 0;

 public:
  Room(const std::string& name, const unsigned int& id);
  static Room* CREATE(const std::string& name, const unsigned int& id);
  bool JoinRoom(const unsigned int& uid, User* user);
  User* getUser(const unsigned int& uid);
  const std::string& getName() const { return m_name; }
  const unsigned int& getRoomId() const { return m_roomId; }
  const unsigned int& getNextRoomId() { return ++msgId; }
  void sendMsgTooAll(const std::string& msg, const unsigned int& from);
  void sendMsgTooUsers(const std::string& msg, const unsigned int& from,
                       const std::vector<unsigned int>& ids);
};
#endif

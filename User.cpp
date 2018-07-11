#include "User.h"

User::User(const std::string& n, const unsigned int& i,
           uWS::WebSocket<uWS::SERVER>* w)
    : name(n), id(i) {
  ws = w;
}
void User::sendMsg(byte* msg) {
  ws->send((const char*)msg, uWS::OpCode::BINARY);
}
uint8_t User::joinARoom(const unsigned int& id, Room* room) {
  if (room == nullptr) return ROOM_IS_NULLPTR;
  auto find = m_rooms.find(id);
  if (find == m_rooms.end()) return ROOM_ALREADY_EXIST;
  m_rooms[id] = room;
  SendUsers su;
  // empty sendusers for send all
  sendUsers[id] = su;
  return ADD_ROOM_SUCCESS;
}
uint8_t User::leaveARoom(const unsigned int& id) {
  auto find = m_rooms.find(id);
  if (find == m_rooms.end()) return ROOM_NOT_EXIST;
  m_rooms.erase(id);
  sendUsers.erase(id);
  return REMOVE_ROOM_SUCCESS;
}

uint8_t User::setSendUsers(const unsigned int& roomId, const SendUsers& users) {
  auto find = m_rooms.find(roomId);
  bool flag = false;
  if (find == m_rooms.end()) return USER_NOT_IN_THIS_ROOM;

  SendUsers& u = sendUsers.at(roomId);
  u.clear();
  for (auto it = users.begin(); it != users.end(); it++) {
    // check if the users are belong to this room
    User* user = it->second;
    auto findRoom = user->m_rooms.find(roomId);
    if (findRoom == user->m_rooms.end()) {
      flag = true;
    } else {
      u[it->first] = it->second;
    }
  }
  if (flag) return SOME_USERS_NOT_IN_THIS_ROOM;
  return SET_SEND_USERS_SUCCESS;
}
uint8_t User::removeSendUsers(const unsigned int& userid,
                              const unsigned int& roomId) {
  auto find = m_rooms.find(roomId);
  if (find == m_rooms.end()) return USER_NOT_IN_THIS_ROOM;
  SendUsers& u = sendUsers.at(roomId);
  if (u.find(userid) == u.end())
    return SEND_USER_NOT_BELONG_TO_THIS_ROOM_OR_DELETED;
  u.erase(userid);
  return REMOVE_USER_SUCCESS;
}

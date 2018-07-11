#include "Room.h"
#include "User.h"

Room ::Room(const std::string& roomName, const unsigned int& id)
    : room_tag(roomName), roomId(id) {}

void Room::sendAll(byte* msg, const unsigned int& from) const {
  auto find = m_users.find(from);
  if (find == m_users.end()) {
  } else {
    std::string data((const char*)msg);
    NewMsgFromUser* packet = new NewMsgFromUser(data, from, roomId);
    packet->serialization();
    for (auto it = m_users.begin(); it != m_users.end(); it++) {
      it->second->sendMsg(packet->getBuffer());
    }
    delete packet;
  }
}

uint8_t Room::userJoinRoom(const unsigned int& id, User* user) {
  if (m_users.find(id) == m_users.end()) {
    m_users[id] = user;
    user->joinARoom(roomId, this);
#ifdef DEBUG
    printf("a new user enter this room: %s", user->getName().c_str());
#endif
    return ADD_SUCCESS_USER_TO_THE_ROOM;
  }
  return USER_ALREADY_IN_THE_ROOM;
}

uint8_t Room::userLeaveRoom(const unsigned int& id) {
  if (m_users.find(id) == m_users.end()) return USER_NOT_IN_THIS_ROOM;
  m_users.erase(id);
  AnUserLeaveARoom* packet = new AnUserLeaveARoom(id);
  packet->serialization();
  for (auto it = m_users.begin(); it != m_users.end(); it++) {
    it->second->sendMsg(packet->getBuffer());
  }
  delete packet;
  return REMOVE_USER_SUCCESS;
}

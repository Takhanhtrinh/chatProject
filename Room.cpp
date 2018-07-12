#include "Room.h"

Room ::Room(const std::string& roomName, const unsigned int& id)
    : room_tag(roomName), roomId(id) {}

void Room::sendAll(byte* msg, const unsigned int& from) const {
  auto find = m_users.find(from);
  if (find == m_users.end()) {
#ifdef DEBUG
    printf("Room: sendAll the user doesn't exist in this room, userId: %d\n",
           from);
#endif
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
  m_users[id]->leaveARoom(roomId);
  m_users.erase(id);
  AnUserLeaveARoom* packet = new AnUserLeaveARoom(id);
  packet->serialization();
  for (auto it = m_users.begin(); it != m_users.end(); it++) {
    it->second->sendMsg(packet->getBuffer());
  }
  delete packet;
#ifdef DEBUG
  printf("a userid: %d left this room: %d\n", id, roomId);
#endif
  return REMOVE_USER_SUCCESS;
}
void Room::sendOnlyToUsers(byte* msg, const std::vector<unsigned int>& u,
                           const unsigned int& sendFrom) const {
  // check if this send from belong to this room
  auto find = m_users.find(sendFrom);
  // found it
  if (find != m_users.end()) {
    std::map<unsigned int, User*> cacheSendUsers;
    for (int i = 0; i < u.size(); i++) {
      // find these users need to send
      unsigned int id = u[i];
      auto it = m_users.find(id);
      if (it != m_users.end()) {
        cacheSendUsers[id] = it->second;
      }
    }
    find->second->setSendUsers(roomId, cacheSendUsers);
    for (auto it = cacheSendUsers.begin(); it != cacheSendUsers.end(); it++)
      it->second->sendMsg(msg);
  }
  // not found
  else {
#ifdef DEBUG
    printf("user with id: %d not in this room: %d\n", sendFrom, roomId);
#endif
  }
}

#include "Room.h"
#include "User.h"

Room::Room(const std::string& name, const unsigned int& id)
    : m_name(name), m_roomId(id) {}

Room* CREATE(const std::string& name, const unsigned int& id) {
  return new Room(name, id);
}

User* Room::getUser(const unsigned int& uid) {
  auto find = users.find(uid);
  if (find != users.end()) {
    return users.at(uid);
  }
  return nullptr;
}
void Room::sendMsgTooAll(const std::string& msg, const unsigned int& from) {
  auto find = users.find(from);
  if (find != users.end()) {
    std::unique_ptr<Server_Send_New_Message> packet =
        std::make_unique<Server_Send_New_Message>(msg, m_roomId, from, ++msgId);
    packet->serialize();
    for (auto it = users.begin(); it != users.end(); it++) {
      if (it->first == from) continue;
      it->second->sendMsg((const char*)packet->buffer.getBuffer());
    }
    return;
  }
#ifdef DEBUG
  printf(
      "cant send data to all users because this userId: %d doesn't belong to "
      "this roomId : %d",
      from, m_roomId);
#endif
}
bool Room::JoinRoom(const unsigned int& uid, User* user) {
  auto find  = users.find(uid);
  if (find != users.end()) {
    users.insert(std::make_pair<unsigned int, User*>((unsigned int)uid,(User*)user));
    return true;
  }
  return false;
}
void Room::sendMsgTooUsers(const std::string& msg, const unsigned int& from,
                           const std::vector<unsigned int>& ids) {
  if (ids.size() == 0) return;
  std::unique_ptr<Server_Send_New_Message> packet =
      std::make_unique<Server_Send_New_Message>(msg, m_roomId, from, ++msgId);
  packet->serialize();
  for (int i = 0; i < ids.size(); i++) {
    User* user = getUser(ids[i]);
    if (user == nullptr) continue;
    user->sendMsg((const char*)packet->buffer.getBuffer());
  }
}

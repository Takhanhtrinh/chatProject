#include "User.h"
User::User(const std::string& name, const unsigned int& id,
           uWS::WebSocket<uWS::SERVER>* nws)
    : m_name(name), m_id(id) {
  ws = nws;
}
User* CREATE(const std::string& name, const unsigned int& id,
             uWS::WebSocket<uWS::SERVER>* nws) {
  return new User(name, id, nws);
}
bool User::JoinRoom(const unsigned int& rid, Room* r) {
  auto find = rooms.find(rid);
  if (find != rooms.end()) {
    rooms.insert(
        std::make_pair<unsigned int, Room*>((unsigned int)rid, (Room*)r));
    return true;
  }
#ifdef DEBUG
  printf("this user: %s already in this roomId: %d\n", m_name, rid);
#endif
  return false;
}
Room* User::getRoom(const unsigned int& rid) {
  auto find = rooms.find(rid);
  if (find != rooms.end()) {
    return rooms.at(rid);
  } else
    return nullptr;
}
void User::sendMsg(const char* msg, const unsigned int& size) const {
  ws->send(msg, size, uWS::BINARY);
}

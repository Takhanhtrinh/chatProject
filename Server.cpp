#include "Server.h"

Server::Server(const int& port) : m_port(port) {
  server.onMessage(
      [&](uWS::WebSocket<uWS::SERVER>* ws, char* message, size_t length,
          uWS::OpCode opCode) { on_message(ws, message, length); });

  server.onConnection([&](uWS::WebSocket<uWS::SERVER>* ws,
                          uWS::HttpRequest req) { on_connection(ws); });
  server.onDisconnection([&](uWS::WebSocket<uWS::SERVER>* ws, int code,
                             char* message,
                             size_t length) { on_disconnect(ws, message); });
  if (server.listen(m_port)) server.run();
}

void Server::on_connection(void* ws) {
  uWS::WebSocket<uWS::SERVER>* websocket = (uWS::WebSocket<uWS::SERVER>*)ws;
  printf("new connection\n");
}
void Server::on_message(void* ws, const char* msg, const size_t& length) {
  uWS::WebSocket<uWS::SERVER>* websocket = (uWS::WebSocket<uWS::SERVER>*)ws;
  msgParse((const byte*)msg, length, websocket);
}
void Server::on_disconnect(void* ws, const char* msg) {
  uWS::WebSocket<uWS::SERVER>* websocket = (uWS::WebSocket<uWS::SERVER>*)ws;
  User* u = (User*)websocket->getUserData();
  if (u != nullptr) {
    std::cout << "user: " << u->getName() << " is disconected" << std::endl;
    username.erase(u->getName());
    m_users.erase(u->getId());
    std::cout << "total users: " << m_users.size() << std::endl;
  }
}

void Server::msgParse(const byte* data, const size_t& length,
                      uWS::WebSocket<uWS::SERVER>* ws) {
  User* u = (User*)ws->getUserData();
  byte type = ::getPacketType(data);
  if (u == nullptr && type != CREATE_USER_PACKET) {
    ws->close();
    return;
  }
  if (u != nullptr && type == CREATE_USER_PACKET) {
    ws->send("you tried to hack this system");
    ws->close();
    return;
  }
  switch (type) {
    case CREATE_USER_PACKET: {
      CreateUserPacket* packet = new CreateUserPacket(data, (int)length);
      packet->derserialization();
      std::string name = packet->getName();
      if (username.find(name) != username.end()) {
        ws->send("name already exist");
        ws->close();
        return;
      }
      unsigned int id = getNextUserId();
      std::unique_ptr<User> user = std::make_unique<User>(name, id, ws);
      m_users[id] = std::move(user);
      username.insert(name);
      ws->setUserData(m_users.at(id).get());
    }
  }
  std::cout << "type: " << type << std::endl;
}
const unsigned int& Server::getNextUserId() {
  numberOfUsers++;
  return numberOfUsers;
}
const unsigned int& Server::getNextRoomId() {
  numberOfRooms++;
  return numberOfRooms;
}

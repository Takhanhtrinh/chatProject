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
    ws->close();
    return;
  }
  switch (type) {
    case CREATE_USER_PACKET: {
      std::unique_ptr<CreateUserPacket> packet =
          std::make_unique<CreateUserPacket>(data, (int)length);
      packet->derserialization();
      std::string name = packet->getName();
      if (username.find(name) != username.end()) {
        std::unique_ptr<ServerErrorMsg> packet1 =
            std::make_unique<ServerErrorMsg>("This Name Already Exist");
        packet1->serialization();
        ws->send((const char*)packet1->getBuffer());
        ws->close();
        return;
      }
      // create user id
      unsigned int id = getNextUserId();
      std::unique_ptr<User> user = std::make_unique<User>(name, id, ws);
      m_users[id] = std::move(user);
      username.insert(name);
      ws->setUserData(m_users.at(id).get());
      break;
    }
    case CREATE_PROFILE_PICTURE: {
      if (length != ::getPacketSize(CREATE_PROFILE_PICTURE, nullptr)) {
        m_users.erase(u->getId());
        ws->close();
        break;
      }
      std::unique_ptr<CreateProfileImage> packet =
          std::make_unique<CreateProfileImage>(data, (int)length);
      packet->derserialization();
      u->setProfileImage(packet->getColors());
      break;
    }
    case CREATE_ROOM_PACKET: {
      if (length > 1 && length < MAX_ROOM_NAME) {
        std::unique_ptr<CreateARoomPacket> packet =
            std::make_unique<CreateARoomPacket>(data, length);
        packet->derserialization();
        // find if there is no room have a same name
        if (roomName.find(packet->getRoomName()) == roomName.end()) {
          roomName.insert(packet->getRoomName());
          int rid = getNextRoomId();
          m_rooms[rid] = std::make_unique<Room>(packet->getRoomName(), rid);
          std::unique_ptr<CreateARoomRespondPacket> pk =
              std::make_unique<CreateARoomRespondPacket>(CREATE_ROOM_OK);
          pk->serialization();
          ws->send((const char*)pk->getBuffer());
#ifdef DEBUG
          printf("create room %s ok\n", packet->getRoomName().c_str());
#endif
          break;
        }
        // the name already created
        else {
          std::unique_ptr<CreateARoomRespondPacket> epk =
              std::make_unique<CreateARoomRespondPacket>(CREATE_ROOM_FAILED);
          epk->serialization();
          ws->send((const char*)epk->getBuffer());
          break;
        }

      }
      // user tried to send a long room name
      else {
        ws->close();
      }
      break;
    }
    case JOIN_A_ROOM_PACKET: {
      if (::getPacketSize(JOIN_A_ROOM_PACKET, nullptr) != length) {
        ws->close();
        break;
      }

      break;
    }
    case SEND_DATA_TO_EVERYONE_IN_A_ROOM_PACKET: {
      break;
    }
    case LEAVE_A_ROOM_PACKET: {
      break;
    }
    case SEND_DATA_TO_USERS_IN_A_ROOM_PACKET: {
      break;
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

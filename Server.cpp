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
  if (u == nullptr && type != CLIENT_INIT_PACKET) {
    ws->close();
    return;
  }
  if (u != nullptr && type == CLIENT_INIT_PACKET) {
    ws->close();
    return;
  }
  if (!checkValidPacketSize(type, length)) {
    ws->close();
    return;
  }
  switch (type) {
    case CLIENT_INIT_PACKET: {
      std::cout << data << std::endl;
      std::unique_ptr<Client_Init_Packet> packet =
          std::make_unique<Client_Init_Packet>(data, length);
      packet->deSerialize();
      // find unser name if it is exist or not
      // case not exist, create User and push to container
      if (username.find(packet->name) == username.end()) {
#ifdef DEBUG
        printf("New User Joined The App %s\n", packet->name.c_str());
#endif
        std::unique_ptr<Server_Init_Packet> packetRespond =
            std::make_unique<Server_Init_Packet>(
                SERVER_INIT_PACKET_STATUS_SUCCESS, "");
        packetRespond->serialize();
        unsigned int uid = getNextUserId();
        m_users.insert(std::make_pair<unsigned int, std::unique_ptr<User>>(
            (unsigned int)uid, std::make_unique<User>(packet->name, uid, ws)));
        username.insert(packet->name);
        User* user = m_users.at(uid).get();
        ws->setUserData((void*)user);
        ws->send((const char*)packetRespond->buffer.getBuffer(), uWS::BINARY);
        // case exist send error msg and close connection
      } else {
#ifdef DEBUG
          printf("this user already exist %s\n", packet->name.c_str());
#endif
        std::unique_ptr<Server_Init_Packet> packetRespond =
            std::make_unique<Server_Init_Packet>(
                SERVER_INIT_PACKET_STATUS_FAIL,
                "This Name Already Exist Please Enter Another Name");
        packetRespond->serialize();
        ws->send((const char*)packetRespond->buffer.getBuffer(), uWS::BINARY);
        printf("buffer: %s\n", packetRespond->buffer.getBuffer());
        ws->close();
      }
      break;
    }
    case CLIENT_CREATE_A_ROOM: {
      std::unique_ptr<Client_Create_Room> packet =
          std::make_unique<Client_Create_Room>(data, length);
      packet->deSerialize();
      if (roomName.find(packet->name) != roomName.end()) {
        unsigned int roomId = getNextRoomId();
        m_rooms.insert(std::make_pair<unsigned int, std::unique_ptr<Room>>(
            (unsigned int)roomId,
            std::make_unique<Room>(packet->name, roomId)));
        roomName.insert(packet->name);
        Room* room = m_rooms.at(roomId).get();
#ifdef DEBUG
        assert(room != nullptr);
#endif
        room->JoinRoom(u->getId(), u);
        std::unique_ptr<Server_Create_Room_Respond> packetRespond =
            std::make_unique<Server_Create_Room_Respond>(
                SERVER_CREATE_A_ROOM_SUCCESS, roomId, packet->name);
        packetRespond->serialize();
        ws->send((const char*)packetRespond->buffer.getBuffer());
      } else {
        std::unique_ptr<Server_Create_Room_Respond> packetRespond =
            std::make_unique<Server_Create_Room_Respond>(
                SERVER_CREATE_A_ROOM_FAIL_ALREADY_EXIST, -1, "");
        packetRespond->serialize();
        ws->send((const char*)packetRespond->buffer.getBuffer());
        return;
      }
    }
    case CLIENT_SEND_TEXT_MESSAGE: {
      std::unique_ptr<Client_Send_Text_Message> packet =
          std::make_unique<Client_Send_Text_Message>(data, length);
      packet->deSerialize();
      auto roomFind = m_rooms.find(packet->roomId);
      if (roomFind != m_rooms.end()) {
        Room* room = m_rooms.at(packet->roomId).get();
        if (packet->sendTo.size() == 0) {
          room->sendMsgTooAll(packet->msg, u->getId());
        } else {
          room->sendMsgTooUsers(packet->msg, u->getId(), packet->sendTo);
        }
      }
    }
  }
}
const unsigned int& Server::getNextUserId() {
  numberOfUsers++;
  return numberOfUsers;
}
const unsigned int& Server::getNextRoomId() {
  numberOfRooms++;
  return numberOfRooms;
}

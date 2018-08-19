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

void Server::removeRooms(User* u) {
  for (auto it = u->rooms.begin(); it != u->rooms.end(); it++) {
    Room* room = it->second;
#ifdef DEBUG
    printf("delete %s from room: %s\n", u->getName().c_str(),
           room->getName().c_str());
#endif
    room->userLeftRoom(u->getId());
    std::unique_ptr<Server_User_Left_Room> packetRespond =
        std::make_unique<Server_User_Left_Room>(room->getRoomId(), u->getId());
    packetRespond->serialize();
    for (auto rit = room->users.begin(); rit != room->users.end(); rit++) {
      rit->second->sendMsg((const char*)packetRespond->buffer.getBuffer(),
                           packetRespond->buffer.getPacketSize());
    }
  }
}

void Server::on_disconnect(void* ws, const char* msg) {
  uWS::WebSocket<uWS::SERVER>* websocket = (uWS::WebSocket<uWS::SERVER>*)ws;
  User* u = (User*)websocket->getUserData();
  if (u != nullptr) {
    std::cout << "user: " << u->getName() << " is disconected" << std::endl;
    username.erase(u);
    removeRooms(u);
    m_users.erase(u->getId());
    // send users in this room to remove this user
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
      if (!packet->deSerialize()) {
        on_disconnect(ws, nullptr);
        return;
      }
      // find unser name if it is exist or not
      // case not exist, create User and push to container
      if (searchUserName(packet->name, (void*)&username) == false) {
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
        username.insert(m_users.at(uid).get());
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
    case CLIENT_SEARCH_ROOM_PACKET: {
      std::unique_ptr<Client_Search_Room_Packet> packet =
          std::make_unique<Client_Search_Room_Packet>(data, length);
      if (!packet->deSerialize()) {
        on_disconnect(ws, nullptr);
        return;
      }
#ifdef DEBUG
      printf("user: %s is searching for room: %s\n", u->getName().c_str(),
             packet->roomName.c_str());
#endif
      if (searchRoomName(packet->roomName, (void*)&roomName) == false) {
        uint32_t roomId = getNextRoomId();
        m_rooms.insert(std::make_pair<unsigned int, std::unique_ptr<Room>>(
            (uint32_t)roomId,
            std::make_unique<Room>(packet->roomName, roomId)));
        Room* roomTemp = m_rooms.at(roomId).get();
        assert(roomTemp != nullptr);
        roomName.insert(roomTemp);
#ifdef DEBUG
        printf("new room is created: %s\n", roomTemp->getName().c_str());
#endif
        if (roomTemp->JoinRoom(u->getId(), u)) {
          u->rooms.insert(std::make_pair<unsigned int, Room*>(
              (unsigned int)roomId, (Room*)roomTemp));
          std::unique_ptr<Server_Search_Room_Packet> packetRespond =
              std::make_unique<Server_Search_Room_Packet>(
                  SERVER_ADD_ROOM_SUCCESS, roomId);
          packetRespond->serialize();
          ws->send((char*)packetRespond->buffer.getBuffer(),
                   packetRespond->buffer.getPacketSize(), uWS::BINARY);
          std::unique_ptr<Server_users_in_a_room> packetUsers =
              std::make_unique<Server_users_in_a_room>(&roomTemp->users,
                                                       roomTemp->getRoomId());
          packetUsers->serialize();
          ws->send((char*)packetUsers->buffer.getBuffer(),
                   packetUsers->buffer.getPacketSize(), uWS::BINARY);
#ifdef DEBUG
          printf("add room success to user: %s roomName: %s totalRoom: %d\n",
                 u->getName().c_str(), roomTemp->getName().c_str(),
                 roomName.size());
#endif
        }
      } else {
        uint32_t roomId = getRoomId(packet->roomName, (void*)&roomName);
        assert(roomId != 0);
        Room* roomTemp = m_rooms.at(roomId).get();
        if (roomTemp->JoinRoom(u->getId(), u)) {
          u->rooms.insert(std::make_pair<unsigned int, Room*>(
              (unsigned int)roomId, (Room*)roomTemp));
          std::unique_ptr<Server_Search_Room_Packet> packetRespond =
              std::make_unique<Server_Search_Room_Packet>(
                  SERVER_ADD_ROOM_SUCCESS, roomId);
          packetRespond->serialize();
          std::unique_ptr<Server_users_in_a_room> packetUsers =
              std::make_unique<Server_users_in_a_room>(&roomTemp->users,
                                                       roomTemp->getRoomId());
          packetUsers->serialize();
          ws->send((const char*)packetRespond->buffer.getBuffer(),
                   packetRespond->buffer.getPacketSize(), uWS::BINARY);
          ws->send((char*)packetUsers->buffer.getBuffer(),
                   packetUsers->buffer.getPacketSize(), uWS::BINARY);
#ifdef DEBUG
          printf("add room success to user: %s roomName: %s totalRoom: %d\n",
                 u->getName().c_str(), roomTemp->getName().c_str(),
                 roomName.size());
#endif
          // broadcast in this room there is an user enter this room
          if (roomTemp->users.size() > 1) {
            std::unique_ptr<Server_New_User_Enter_Room> userJoinPacket =
                std::make_unique<Server_New_User_Enter_Room>(
                    u->getName(), u->getId(), roomId);
            userJoinPacket->serialize();
            for (auto it = roomTemp->users.begin(); it != roomTemp->users.end();
                 it++) {
              if (it->second->getId() == u->getId()) continue;
              it->second->sendMsg(
                  (const char*)userJoinPacket->buffer.getBuffer(),
                  userJoinPacket->buffer.getPacketSize());
            }
          }
        } else {
          std::unique_ptr<Server_Search_Room_Packet> packetRespond =
              std::make_unique<Server_Search_Room_Packet>(
                  SERVER_ADD_ROOM_FAILED, (uint32_t)0);
          packetRespond->serialize();
          ws->send((const char*)packetRespond->buffer.getBuffer(),
                   packetRespond->buffer.getPacketSize(), uWS::BINARY);
        }
      }
      break;
    }
    case CLIENT_SEND_TEXT_MESSAGE: {
#ifdef DEBUG
      printf("new message from user: %s\n", u->getName().c_str());
#endif
      std::unique_ptr<Client_Send_Text_Message> packet =
          std::make_unique<Client_Send_Text_Message>(data, length);
      if (!packet->deSerialize()) {
        on_disconnect(ws, nullptr);
        return;
      }
#ifdef DEBUG
      printf("User: %s just send a message: %s\n", u->getName().c_str(),
             packet->msg.c_str());
#endif
      auto roomFind = m_rooms.find(packet->roomId);
      if (roomFind != m_rooms.end()) {
        Room* room = m_rooms.at(packet->roomId).get();
        if (packet->sendTo.size() == 0) {
          room->sendMsgTooAll(packet->msg, u->getId());
#ifdef DEBUG
          printf("send message to all\n");
#endif
        } else {
          room->sendMsgTooUsers(packet->msg, u->getId(), packet->sendTo);
        }
      }
      break;
    }
  }
}
const uint32_t& Server::getNextUserId() {
  numberOfUsers++;
  return numberOfUsers;
}
const uint32_t& Server::getNextRoomId() {
  numberOfRooms++;
  return numberOfRooms;
}

#ifndef SERVER_H
#define SERVER_H
#include <uWS/uWS.h>
#include <iostream>
#include <memory>
#include <set>
#include <type_traits>
#include "Config.h"
#include "Room.h"
#include "User.h"
#include "ServerApi.h"
class Server {
 private:
  uint32_t numberOfUsers = 0;
  uint32_t numberOfRooms = 0;
  int m_port;
  uWS::Hub server;
  std::map<unsigned int, std::unique_ptr<Room> > m_rooms;
  std::map<unsigned int, std::unique_ptr<User> > m_users;
  std::set<User*, UserNameCmp> username;
  std::set<Room*, RoomNameCmp> roomName;
  const uint32_t& getNextUserId();
  const uint32_t& getNextRoomId();
  // return false if user try to hack the system
  void msgParse(const byte* data, const size_t& length,
                uWS::WebSocket<uWS::SERVER>* ws);
  void removeRooms(User * u);

 public:
  Server(const int& port);
  void on_connection(void* ws);
  void on_message(void* ws, const char* msg, const size_t& length);
  void on_disconnect(void* ws, const char* msg);
};
#endif

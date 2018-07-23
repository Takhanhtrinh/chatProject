#ifndef SERVER_H
#define SERVER_H
#include <uWS/uWS.h>
#include <iostream>
#include <memory>
#include <set>
#include "Config.h"
#include "Room.h"
#include "User.h"
class Server {
 private:
  unsigned int numberOfUsers = 0;
  unsigned int numberOfRooms = 0;
  int m_port;
  uWS::Hub server;
  std::map<unsigned int, std::unique_ptr<Room> > m_rooms;
  std::map<unsigned int, std::unique_ptr<User> > m_users;
  std::set<std::string> username;
  std::set<std::string> roomName;

  const unsigned int& getNextUserId();
  const unsigned int& getNextRoomId();
  // return false if user try to hack the system
  void msgParse(const byte* data, const size_t& length,
                uWS::WebSocket<uWS::SERVER>* ws);

 public:
  Server(const int& port);
  void on_connection(void* ws);
  void on_message(void* ws, const char* msg, const size_t& length);
  void on_disconnect(void* ws, const char* msg);
};
#endif

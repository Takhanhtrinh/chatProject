#ifndef SERVER_H
#define SERVER_H
#include <uWS/uWS.h>
#include <iostream>
#include <memory>
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
  const unsigned int& getNextUserId();
  const unsigned int& getNextRoomId();
  void msgParse(byte* data);

 public:
  Server(const int& port);
  void on_connection(void* ws);
  void on_message(void* ws, const char* msg, const size_t& length);
  void on_disconnect(void* ws, const char* msg);
};
#endif

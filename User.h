#ifndef USER_H
#define USER_H
#include <uWS/uWS.h>
#include <iostream>
#include <map>
#include <string>
class Room;
class User {
 protected:
  std::map<unsigned int, Room*> rooms;
  std::string m_name;
  unsigned int m_id;
  uWS::WebSocket<uWS::SERVER>* ws;
 public:
  User(const std::string& name, const unsigned int& id,
       uWS::WebSocket<uWS::SERVER>* nws);
  static User* CREATE(const std::string& name, const unsigned int& id, uWS::WebSocket<uWS::SERVER>*nws);
  const std::string& getName() const { return m_name; }
  const unsigned int& getId() const { return m_id; }
  bool JoinRoom(const unsigned int& rid, Room* r);
  Room* getRoom(const unsigned int& rid);
  void sendMsg(const char* msg) const;
  virtual ~User() {}
};

#endif

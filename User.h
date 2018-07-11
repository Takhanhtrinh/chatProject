#ifndef DEBUG
#define DEBUG
#include <uWS/uWS.h>
#include <map>
#include <string>
#include <vector>
#include "Config.h"

#define ROOM_IS_NULLPTR -1
#define ROOM_ALREADY_EXIST -2
#define ADD_ROOM_SUCCESS 1

#define ROOM_NOT_EXIST -1
#define REMOVE_ROOM_SUCCESS 1

#define USER_NOT_IN_THIS_ROOM -1
#define SOME_USERS_NOT_IN_THIS_ROOM -2
#define SET_SEND_USERS_SUCCESS 1

#define SEND_USER_NOT_BELONG_TO_THIS_ROOM_OR_DELETED -2
#define REMOVE_USER_SUCCESS 1

class Room;
class User {
 public:
  typedef std::map<unsigned int, User*> SendUsers;
 private:
  std::string name;
  std::vector<int> profileImage{1024};
  uWS::WebSocket<uWS::SERVER>* ws;
  unsigned int id;
  std::map<unsigned int, Room*> m_rooms;
  std::map<unsigned int, SendUsers> sendUsers;

 public:
  User(const std::string& n, const unsigned int &i, uWS::WebSocket<uWS::SERVER>* w);
  void setProfileImage(const std::vector<int> v) { profileImage = v; }
  void sendMsg(byte* msg);
  const std::string& getName() const { return name; }
  const std::vector<int>& getProfileImage() { return profileImage; }
  const unsigned int& getId() const { return id; }
  uint8_t joinARoom(const unsigned int& id,Room* room);
  uint8_t leaveARoom(const unsigned int& id);
  uint8_t setSendUsers(const unsigned int& roomId, const SendUsers& users);
  uint8_t removeSendUsers(const unsigned int& userid, const unsigned int& roomId);
  virtual ~User() {}
};
#endif

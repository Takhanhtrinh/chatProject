#ifndef ROOM_H
#define ROOM_H
#include "Config.h"
#include <map>
#include <iostream>
#include "Packet.h"
#define USER_ALREADY_IN_THE_ROOM -1
#define ADD_SUCCESS_USER_TO_THE_ROOM 1

#define USER_NOT_IN_THIS_ROOM -1
#define REMOVE_USER_SUCCESS 1
class User;
class Room {
  private:
    std::string room_tag;
    unsigned int roomId;
    std::map<unsigned int, User*> m_users;
    long create_time_stamp;

  public:
    Room(const std::string& roomName, const unsigned int& id);
    void sendAll(byte * msg, const unsigned int& from) const;
    void sendOnlyToUsers(byte * msg, const std::vector<unsigned int>& u, const unsigned int& sendFrom) const;
    uint8_t userJoinRoom(const unsigned int& id, User* user);
    uint8_t userLeaveRoom(const unsigned int& id);
    virtual ~Room(){}

    
};
#endif

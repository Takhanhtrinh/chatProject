#ifndef SERVER_API
#define SERVER_API
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <set>
#include <string>
#include "Room.h"
#include "User.h"
struct UserNameCmp {
  using is_transparent = void;
  bool operator()(const User* lhs, const User* rhs) const {
    return lhs->getName() < rhs->getName();
  }
  bool operator()(const std::string& name, const User* u) const {
    return name < u->getName();
  }
  bool operator()(const User* u, const std::string& name) const {
    return u->getName() < name;
  }
};
struct RoomNameCmp {
  using is_transparent = void;
  bool operator()(const Room* lhs, const Room* rhs) const {
    return lhs->getName() < rhs->getName();
  }
  bool operator()(const std::string& name,const Room* room) const {
    return name < room->getName();
  }
  bool operator()(const Room* room, const std::string& name) const {
    return room->getName() < name;
  }
};
inline bool searchRoomName(const std::string& roomName, void* data) {
  std::set<Room*, RoomNameCmp>* r = (std::set<Room*, RoomNameCmp>*)data;
  auto find = r->find(roomName);
  if (find != r->end()) {
#ifdef DEBUG
    printf("this room already exist: %s\n", roomName.c_str());
#endif
    return true;
  }
#ifdef DEBUG
  printf("this room is not exist: %s\n", roomName.c_str());
#endif
  return false;
}
inline unsigned int getRoomId(const std::string& roomName, void * data) { std::set<Room*, RoomNameCmp>* r = (std::set<Room*, RoomNameCmp>*)data;
  auto find = r->find(roomName);
  if (find != r->end()) {
#ifdef DEBUG
    printf("this userName: %s has id: %d\n", roomName.c_str(),(*find)->getRoomId());
#endif
    return (*find)->getRoomId();
  }
  return 0;

}
inline bool searchUserName(const std::string& userName, void* data) {
  std::set<User*, UserNameCmp>* r = (std::set<User*, UserNameCmp>*)data;
  auto find = r->find(userName);
  if (find != r->end()) {
#ifdef DEBUG
    printf("this user name already exist: %s\n", userName.c_str());
#endif
    return true;
  }
#ifdef DEBUG
  printf("this user name is not exist: %s\n", userName.c_str());
#endif
  return false;
}
#endif

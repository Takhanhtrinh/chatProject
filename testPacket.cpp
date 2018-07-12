#include <assert.h>
#include <iostream>
#include <string>
#include "Buffer.h"
#include "Packet.h"
#include "User.h"
void testCreateUserPacket() {
  std::string name = "vkl";
  CreateUserPacket *packet = new CreateUserPacket(name);
  packet->serialization();

  std::cout << "size: " << packet->getPacketSize() << std::endl;
  packet->startReading();
  packet->derserialization();
  std::cout << "packet type: " << packet->getPacketType() << std::endl;
  std::cout << "name: " << packet->getName() << std::endl;
  delete packet;
}
void testCreateRoomPacket() {
  CreateARoomPacket *packet = new CreateARoomPacket("http://vkl.com");
  std::cout << "packet size: " << packet->getPacketSize() << std::endl;
  packet->serialization();
  CreateARoomPacket *packet1 =
      new CreateARoomPacket(packet->getBuffer(), packet->getPacketSize());
  packet1->derserialization();

  std::cout << "packet type: " << packet1->getPacketType() << std::endl;
  std::cout << "room name: " << packet1->getRoomName() << std::endl;
  std::cout << "packet1 size: " << packet1->getPacketSize() << std::endl;
  delete packet1;
  delete packet;
}
void testJoinARoomPacket() {
  JoinARoomPacket *packet = new JoinARoomPacket("this room is perfect");
  packet->serialization();
  JoinARoomPacket *packet1 =
      new JoinARoomPacket(packet->getBuffer(), packet->getPacketSize());
  packet1->derserialization();
  std::string name = packet1->getName();
  std::cout << "packet type: " << packet1->getPacketType() << std::endl;
  std::cout << "packet size: " << packet1->getPacketSize() << std::endl;
  std::cout << "name: " << name << std::endl;
  delete packet;
  delete packet1;
}
void testSendDataToEveryoneInARoomPacket() {
  std::string temp = "e hom qua tao moi di ia";
  SendDataToEveryoneInARoomPacket *packet =
      new SendDataToEveryoneInARoomPacket(temp, temp.size(), 1024);
  packet->serialization();
  std::cout << "packet size: " << packet->getPacketSize() << std::endl;
  SendDataToEveryoneInARoomPacket *packet1 =
      new SendDataToEveryoneInARoomPacket(packet->getBuffer(),
                                          packet->getPacketSize());
  packet1->derserialization();
  std::cout << "packet1 size: " << packet1->getPacketSize() << std::endl;
  std::cout << "packet1 type: " << packet1->getPacketType() << std::endl;
  std::cout << "data: " << packet1->getData() << std::endl;
  std::cout << "length: " << packet1->getSize() << std::endl;
  std::cout << "roomid: " << packet->getRoomNumber() << std::endl;
  delete packet1;
  delete packet;
}
void testSendDataToUsersPacket() {
  std::string temp = "this is a test";
  std::vector<unsigned int> id;
  for (int i = 0; i < 10; i++) {
    id.push_back(100 + 10);
  }
  SendDataToUsersInARoomPacket *packet = new SendDataToUsersInARoomPacket(
      (byte *)temp.c_str(), temp.size(), 1024, id);
  std::cout << "packet size: " << packet->getPacketSize() << std::endl;
  packet->serialization();
  SendDataToUsersInARoomPacket *packet1 = new SendDataToUsersInARoomPacket(
      packet->getBuffer(), packet->getPacketSize());
  packet1->derserialization();
  std::cout << "packet type: " << packet1->getPacketType() << std::endl;
  std::cout << "packet Size: " << packet1->getPacketSize() << std::endl;
  std::cout << "room number: " << packet1->getRoomNumber() << std::endl;
  std::cout << "dataSize: " << packet1->getSize() << std::endl;
  std::cout << "data: " << packet1->getData() << std::endl;
  std::cout << "user size: " << packet1->getUserSize() << std::endl;
  std::vector<unsigned int> users = packet1->getUsers();
  for (int i = 0; i < users.size(); i++) {
    std::cout << "i: " << i << " userid: " << users[i] << std::endl;
  }
  delete packet;
  delete packet1;
}
void testLeaveARoomPacket() {
  LeaveARoomPacket *packet = new LeaveARoomPacket(100222);
  packet->serialization();
  std::cout << "packet size: " << packet->getPacketSize() << std::endl;
  LeaveARoomPacket *packet1 =
      new LeaveARoomPacket(packet->getBuffer(), packet->getPacketSize());
  packet1->derserialization();
  std::cout << "packet type: " << packet1->getPacketType() << std::endl;
  std::cout << "packet size: " << packet1->getPacketSize() << std::endl;
  std::cout << "room number: " << packet1->getRoomId() << std::endl;

  delete packet;
  delete packet1;
}
void testAnUserLeaveARoom() {
  AnUserLeaveARoom *packet = new AnUserLeaveARoom(10022111);
  packet->serialization();
  assert(packet->getPacketSize() == 5);
  AnUserLeaveARoom *packet1 =
      new AnUserLeaveARoom(packet->getBuffer(), packet->getPacketSize());
  packet1->derserialization();
  assert(packet->getId() == packet1->getId());
  assert(packet->getPacketType() == packet1->getPacketType());
  assert(packet->getPacketSize() == packet1->getPacketSize());
  delete packet;
  delete packet1;
}
void testNewMsgFromUser() {
  std::string str = "hello world :))";
  NewMsgFromUser *packet = new NewMsgFromUser(str, 1024, 222);
  packet->serialization();
  std::cout << "packetSize: " << packet->getPacketSize() << std::endl;
  NewMsgFromUser *packet1 =
      new NewMsgFromUser(packet->getBuffer(), packet->getPacketSize());
  packet1->derserialization();
  assert(packet1->getData() == str);
  assert(packet1->getSendFrom() == 1024);
  assert(packet1->getRoomId() == 222);
  assert(packet1->getPacketType() == packet->getPacketType());
  assert(packet1->getPacketSize() == packet->getPacketSize());
  delete packet;
  delete packet1;
}
void testJoinARoomRespondPacket() {
  JoinARoomRespondPacket *packet =
      new JoinARoomRespondPacket(ROOM_RESPOND_NOT_EXIST, nullptr);
  packet->serialization();
  assert(packet->getStatus() == ROOM_RESPOND_NOT_EXIST);
  assert(packet->getPacketSize() == 2);
  delete packet;
  std::map<unsigned int, User *> users;
  User *a = new User("trinh ta", 1023, nullptr);
  User *b = new User("truc ta", 1024, nullptr);
  users[a->getId()] = a;
  users[b->getId()] = b;
  JoinARoomRespondPacket *packet1 =
      new JoinARoomRespondPacket(ROOM_EXIST, &users);
  packet1->serialization();
  JoinARoomRespondPacket *packet2 = new JoinARoomRespondPacket(
      packet1->getBuffer(), packet1->getPacketSize());
  packet2->derserialization();
  assert(packet1->getPacketType() == packet2->getPacketType());
  assert(packet1->getPacketSize() == packet2->getPacketSize());
  std::map<unsigned int, std::string> users1 = packet2->getUsers();
  for (auto it = users1.begin(); it != users1.end(); it++)
    std::cout << "id " << it->first << " name: " << it->second << std::endl;
  delete packet1;
  delete packet2;
  delete a;
  delete b;
}
void testCreateRoomRespondPacket() {
  CreateARoomRespondPacket *packet =
      new CreateARoomRespondPacket(CREATE_ROOM_OK);
  packet->serialization();
  CreateARoomRespondPacket *packet1 = new CreateARoomRespondPacket(
      packet->getBuffer(), packet->getPacketSize());
  packet1->derserialization();
  assert(packet->getPacketSize() == packet1->getPacketSize());
  assert(packet->getPacketType() == packet1->getPacketType());
  assert(packet->getStatus() == packet1->getStatus());
  delete packet;
  delete packet1;
}
int main() {
  testCreateRoomPacket();
  testJoinARoomPacket();
  testSendDataToEveryoneInARoomPacket();
  testSendDataToUsersPacket();
  testLeaveARoomPacket();
  testAnUserLeaveARoom();
  testNewMsgFromUser();
  testJoinARoomRespondPacket();
  testCreateRoomRespondPacket();
}

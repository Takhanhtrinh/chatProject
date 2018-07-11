#include <iostream>
#include <string>
#include "Buffer.h"
#include "Packet.h"
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
  JoinARoomPacket *packet = new JoinARoomPacket(56);
  packet->serialization();
  JoinARoomPacket *packet1 =
      new JoinARoomPacket(packet->getBuffer(), packet->getPacketSize());
  packet1->derserialization();
  std::cout << "packet type: " << packet1->getPacketType() << std::endl;
  std::cout << "roomid: " << packet1->getRoomId() << std::endl;
  delete packet;
  delete packet1;
}
void testSendDataToEveryoneInARoomPacket() {
  std::string temp = "e hom qua tao moi di ia";
  SendDataToEveryoneInARoomPacket *packet = new SendDataToEveryoneInARoomPacket(
      (byte *)temp.c_str(), temp.size(), 1024);
  packet->serialization();
  std::cout << "packet size: " << packet->getPacketSize() << std::endl;
  SendDataToEveryoneInARoomPacket *packet1 =
      new SendDataToEveryoneInARoomPacket(packet->getBuffer(),
                                          packet->getPacketSize());
  packet1->derserialization();
  std::cout << "packet type" << packet1->getPacketType() << std::endl;
  std::cout << "data: " << packet1->getData() << std::endl;
  std::cout << "room number: " << packet1->getRoomNumber() << std::endl;
  std::cout << "string size: " << packet1->getSize() << std::endl;
  std::cout << "packet1 size: " << packet1->getPacketSize() << std::endl;
  delete packet;
  delete packet1;
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
int main() {
  testCreateUserPacket();
  testCreateRoomPacket();
  testJoinARoomPacket();
  testSendDataToEveryoneInARoomPacket();
  testSendDataToUsersPacket();
  testLeaveARoomPacket();
}

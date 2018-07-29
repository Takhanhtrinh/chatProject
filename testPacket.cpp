#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "Buffer.h"
#include "Packet.h"
void testClientInitPacket() {
  Client_Init_Packet* packet = new Client_Init_Packet("Trinh Ta");
  packet->serialize();
  Client_Init_Packet* packet1 =
      new Client_Init_Packet(packet->buffer.getBuffer(), packet->packetSize);
  packet1->deSerialize();
  assert(packet1->name == "Trinh Ta");
  assert(packet1->packetSize == 1 + std::string("Trinh Ta").length());
  delete packet1;
  delete packet;
}
void testServerInitPacket() {
  Server_Init_Packet* packet =
      new Server_Init_Packet(SERVER_INIT_PACKET_STATUS_SUCCESS, " ");
  packet->serialize();
  Server_Init_Packet* packet1 =
      new Server_Init_Packet(packet->buffer.getBuffer(), packet->packetSize);
  packet1->deSerialize();
  assert(packet1->packetSize == 2);
  assert(packet1->status == SERVER_INIT_PACKET_STATUS_SUCCESS);
  std::string status = "please pick another name";
  Server_Init_Packet* packet2 =
      new Server_Init_Packet(SERVER_INIT_PACKET_STATUS_FAIL, status);
  packet2->serialize();

  Server_Init_Packet* packet3 =
      new Server_Init_Packet(packet2->buffer.getBuffer(), packet2->packetSize);
  packet3->deSerialize();
  assert(packet3->status == SERVER_INIT_PACKET_STATUS_FAIL);
  std::cout << packet3->statusStr << std::endl;
  assert(packet3->statusStr == status);
  assert(packet3->packetSize == 1 + 1 + status.length());
  delete packet;
  delete packet2;
  delete packet3;
  delete packet1;
}
void testClientSearchRoom() {
  std::string roomName = "HaHaHaha";
  Client_Search_Room_Packet* packet = new Client_Search_Room_Packet(roomName);
  packet->serialize();
  Client_Search_Room_Packet* packet1 = new Client_Search_Room_Packet(
      packet->buffer.getBuffer(), packet->packetSize);
  packet1->deSerialize();
  assert(packet1->roomName == roomName);
  assert(packet1->packetSize == roomName.length() + 1);
  delete packet;
  delete packet1;
}
void testServerSearchRoom1() {
  Server_Search_Room_Packet* packet =
      new Server_Search_Room_Packet("", SERVER_SEARCH_ROOM_ROOM_EXIST);
  packet->serialize();
  Server_Search_Room_Packet* packet1 = new Server_Search_Room_Packet(
      packet->buffer.getBuffer(), packet->packetSize);
  packet1->deSerialize();
  assert(packet1->packetSize == 2);
  assert(packet1->status == SERVER_SEARCH_ROOM_ROOM_EXIST);
  delete packet;
  delete packet1;
}
void testServerSearchRoom2() {
  std::string msg = "this room already created";
  Server_Search_Room_Packet* packet =
      new Server_Search_Room_Packet(msg, SERVER_SEARCH_ROOM_ROOM_DOESNT_EXIST);
  packet->serialize();
  Server_Search_Room_Packet* packet1 = new Server_Search_Room_Packet(
      packet->buffer.getBuffer(), packet->packetSize);
  packet1->deSerialize();
  assert(packet1->packetSize == msg.length() + 2);
  assert(packet1->status == SERVER_SEARCH_ROOM_ROOM_DOESNT_EXIST);
  assert(packet1->msgError == msg);
  delete packet;
  delete packet1;
}
void testClientSendTextMessage() {
  ::srand(NULL);
  std::string msg = "Hello every one in this room";
  int id = 5155;
  std::vector<unsigned int> userId;
  for (int i = 0; i < 100; i++) {
    unsigned int v = ::rand() % 90000;
    userId.push_back(v);
  }
  Client_Send_Text_Message* packet =
      new Client_Send_Text_Message(msg, id, userId);
  packet->serialize();
  Client_Send_Text_Message* packet1 = new Client_Send_Text_Message(
      packet->buffer.getBuffer(), packet->packetSize);
  packet1->deSerialize();
  assert(packet1->msg == msg);
  assert(packet1->roomId == id);
  assert(userId.size() == packet1->sendTo.size());
  for (int i = 0; i < userId.size(); i++) {
    assert(userId[i] == packet->sendTo[i]);
  }
  assert(packet1->packetSize ==
         1 + 4 + 4 + msg.length() + 4 + userId.size() * 4);

  assert(packet1->type == CLIENT_SEND_TEXT_MESSAGE);
  delete packet;
  delete packet1;
}
void testServerSendTextMessageRespond() {
  Server_Send_Text_Message_Respond* packet =
      new Server_Send_Text_Message_Respond(SERVER_SEND_TEXT_MESSAGE_RESPOND_OK);
  packet->serialize();
  Server_Send_Text_Message_Respond* packet1 =
      new Server_Send_Text_Message_Respond(packet->buffer.getBuffer(),
                                           packet->packetSize);
  packet1->deSerialize();
  assert(packet1->type == SERVER_SEND_TEXT_MESSAGE_RESPOND);

  assert(packet1->status == SERVER_SEND_TEXT_MESSAGE_RESPOND_OK);
  delete packet;
  delete packet1;
}
void testClientSendPixelMessage() {
  std::vector<byte> pixel(MAX_PIXEL_SIZE * 4);
  for (int i = 0; i < pixel.size(); i += 4) {
    byte r = rand() % 255;
    byte g = rand() % 255;
    byte b = rand() % 255;
    byte a = rand() % 255;
    pixel[i] = r;
    pixel[i + 1] = g;
    pixel[i + 2] = b;
    pixel[i + 3] = a;
  }
  std::vector<unsigned int> users(1024);
  for (int i = 0; i < 1024; i++) {
    int id = rand() % 5000;
    users[i] = id;
  }
  Client_Send_Pixel_Message* packet =
      new Client_Send_Pixel_Message(pixel, 1024, users);
  packet->serialize();
  Client_Send_Pixel_Message* packet1 = new Client_Send_Pixel_Message(
      packet->buffer.getBuffer(), packet->packetSize);
  packet1->deSerialize();
  assert(packet1->packetSize ==
         1 + 4 + MAX_PIXEL_SIZE * 4 + 4 + users.size() * 4);
  assert(packet1->type == CLIENT_SEND_PIXEL_MESSAGE);
  assert(packet1->roomId = 1024);
  assert(packet1->sendTo.size() == 1024);
  for (int i = 0; i < 1024; i++) {
    assert(packet1->sendTo[i] == users[i]);
  }
  for (int i = 0; i < MAX_PIXEL_SIZE * 4; i++) {
    assert(pixel[i] == packet1->msg[i]);
  }
  delete packet;
  delete packet1;
}
void testServerSendNewMessage() {
  std::string msg = "this is a sample text";
  unsigned int roomId = 77731;
  unsigned int userId = 888773;
  unsigned int msgId = 7762;
  Server_Send_New_Message* packet =
      new Server_Send_New_Message(msg, roomId, userId, msgId);
  packet->serialize();
  Server_Send_New_Message* packet1 = new Server_Send_New_Message(
      packet->buffer.getBuffer(), packet->packetSize);
  packet1->deSerialize();
  assert(packet1->type == SERVER_NEW_TEXT_MESSAGE);
  assert(packet1->msg == msg);
  assert(packet1->roomId == roomId);
  assert(packet1->fromId == userId);
  assert(packet1->msgId = msgId);
  printf("timeStamp: %llu\n", packet1->timeStamp);
  assert(packet1->timeStamp == packet->timeStamp);
  delete packet;
  delete packet1;
}
void testServerSendImageMessage() {
  std::vector<byte> msg(MAX_PIXEL_SIZE * 4);
  for (int i = 0; i < MAX_PIXEL_SIZE * 4; i++) {
    msg[i] = rand() % 255;
  }
  unsigned int roomId = 2222424;
  unsigned int fromId = 23482389;
  unsigned int msgId = 238423984;
  Server_Send_Pixel_Message* packet =
      new Server_Send_Pixel_Message(msg, roomId, fromId, msgId);
  packet->serialize();
  Server_Send_Pixel_Message* packet1 = new Server_Send_Pixel_Message(
      packet->buffer.getBuffer(), packet->packetSize);
  packet1->deSerialize();
  assert(packet1->type == SERVER_NEW_IMAGE_MESSAGE);
  assert(packet1->roomId == roomId);
  assert(packet1->fromId == fromId);
  assert(packet1->msgId == msgId);
  assert(packet->timeStamp == packet1->timeStamp);
  assert(packet1->packetSize == 1 + 4 + 4 + 4 + 8 + MAX_PIXEL_SIZE * 4);
  for (int i = 0; i < MAX_PIXEL_SIZE * 4; i++) {
    assert(msg[i] == packet1->pixel[i]);
  }
  delete packet;
  delete packet1;
}
void testNewUserEnterRoom() {
  std::string name = "TrinhTa";
  int roomId = 234234;
  int userId = 2312321;
  Server_New_User_Enter_Room* packet =
      new Server_New_User_Enter_Room(name, userId, roomId);
  packet->serialize();
  Server_New_User_Enter_Room* packet1 = new Server_New_User_Enter_Room(
      packet->buffer.getBuffer(), packet->packetSize);
  packet1->deSerialize();
  assert(packet1->name == name);
  assert(packet1->roomId == roomId);
  assert(packet1->id == userId);
  assert(packet1->type == SERVER_NEW_USER_ENTER_ROOM);
  assert(packet1->packetSize == 1 + 4 + 4 + name.length());
  delete packet;
  delete packet1;
}
void testUserLeftRoom() {
  unsigned int roomId = 124123;
  unsigned int userId = 1231244;
  Server_User_Left_Room* packet = new Server_User_Left_Room(roomId, userId);
  packet->serialize();
  Server_User_Left_Room* packet1 =
      new Server_User_Left_Room(packet->buffer.getBuffer(), packet->packetSize);
  packet1->deSerialize();
  assert(packet1->type == SERVER_NEW_USER_LEAVE_ROOM);
  assert(packet1->packetSize == 1 + 4 + 4);
  assert(packet1->roomId == roomId);
  assert(packet1->userId == userId);
  delete packet;
  delete packet1;
}
void testClientCreateRoom() {
  std::string name = "this room is for test";
  Client_Create_Room* packet = new Client_Create_Room(name);
  packet->serialize();
  Client_Create_Room* packet1 =
      new Client_Create_Room(packet->buffer.getBuffer(), packet->packetSize);
  packet1->deSerialize();
  assert(packet1->type == CLIENT_CREATE_A_ROOM);
  assert(packet1->name == name);
  assert(packet1->packetSize == 1 + name.length());
  delete packet;
  delete packet1;
}
void testServerCreateARoom1() {
  Server_Create_Room_Respond* packet = new Server_Create_Room_Respond(
      SERVER_CREATE_A_ROOM_FAIL_ALREADY_EXIST, 0, "");
  packet->serialize();
  Server_Create_Room_Respond* packet1 = new Server_Create_Room_Respond(
      packet->buffer.getBuffer(), packet->packetSize);
  packet1->deSerialize();
  assert(packet1->packetSize == 2);
  assert(packet1->type == SERVER_CREATE_A_ROOM);
  assert(packet1->status == SERVER_CREATE_A_ROOM_FAIL_ALREADY_EXIST);
  delete packet;
  delete packet1;
}
void testServerCreateARoom2() {
  std::string name = "this is the room test";
  unsigned int roomId = 341231;
  byte status = SERVER_CREATE_A_ROOM_SUCCESS;
  Server_Create_Room_Respond* packet =
      new Server_Create_Room_Respond(status, roomId, name);
  packet->serialize();
  Server_Create_Room_Respond* packet1 = new Server_Create_Room_Respond(
      packet->buffer.getBuffer(), packet->packetSize);
  packet1->deSerialize();
  assert(packet1->type == SERVER_CREATE_A_ROOM);
  assert(packet1->status = SERVER_CREATE_A_ROOM_SUCCESS);
  assert(packet1->packetSize == 1 + 1 + 4 + name.length());
  assert(packet1->roomId == roomId);
  assert(packet1->roomName == name);
  printf("roomName: %s\n", name.c_str());
  delete packet1;
  delete packet;
}

int main() {
  // testClientInitPacket();
  // testServerInitPacket();
  // testClientSearchRoom();
  // testServerSearchRoom1();
  // testServerSearchRoom2();
  // testClientSendTextMessage();
  // testServerSendTextMessageRespond();
  // testClientSendPixelMessage();
  // testServerSendNewMessage();
  // testServerSendImageMessage();
  // testNewUserEnterRoom();
  // testUserLeftRoom();
  // testClientCreateRoom();
  // testServerCreateARoom1();
  // testServerCreateARoom2();
  return 0;
}

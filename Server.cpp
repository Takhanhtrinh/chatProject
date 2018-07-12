#include "Server.h"

Server::Server(const int& port) : m_port(port) {
  server.onMessage(
      [&](uWS::WebSocket<uWS::SERVER>* ws, char* message, size_t length,
          uWS::OpCode opCode) { on_message(ws, message, length); });

  server.onConnection([&](uWS::WebSocket<uWS::SERVER>* ws,
                          uWS::HttpRequest req) { on_connection(ws); });
  server.onDisconnection([&](uWS::WebSocket<uWS::SERVER>* ws, int code,
                             char* message,
                             size_t length) { on_disconnect(ws, message); });
  if (server.listen(m_port)) server.run();
}

void Server::on_connection(void* ws) {
  uWS::WebSocket<uWS::SERVER>* websocket = (uWS::WebSocket<uWS::SERVER>*)ws;
  printf("new user connected\n");
  websocket->send("may la thang ngu\n");
}
void Server::on_message(void* ws, const char* msg, const size_t& length) {
  uWS::WebSocket<uWS::SERVER>* websocket = (uWS::WebSocket<uWS::SERVER>*)ws;
}
void Server::on_disconnect(void* ws, const char* msg) {
  uWS::WebSocket<uWS::SERVER>* websocket = (uWS::WebSocket<uWS::SERVER>*)ws;
  printf("new user disconnected\n");
}

void Server::msgParse(byte* data) {}
const unsigned int& Server::getNextUserId() {
  numberOfUsers++;
  return numberOfUsers;
}
const unsigned int& Server::getNextRoomId() {
  numberOfRooms++;
  return numberOfRooms;
}

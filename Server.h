#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include "Config.h"
#include <uWS/uWS.h>
#include "Buffer.h"
#include "Packet.h"
class Server {
  private:
    int m_port;
  public:
    void on_connection(void * ws);
    void on_message(void * ws, const char* msg);
    void on_disconnect(void * ws, const char* msg);
};
#endif 

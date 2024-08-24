#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__
#include "SocketIO.h"
#include <string>

using std::string;

class TcpConnection
{
public:
    explicit TcpConnection(int fd); 
    ~TcpConnection();
    void send(const string & msg);
    string receive();
    string toString();
private:
    SocketIO _socketio;
};

#endif

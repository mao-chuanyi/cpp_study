#include "TcpConnection.h"

TcpConnection::TcpConnection(int fd)
:_socketio(fd)
{

}

TcpConnection::~TcpConnection()
{

}

void TcpConnection::send(const string & msg)
{
    _socketio.writen(msg.c_str(), msg.size());
}

string TcpConnection::receive()
{
    char buff[65536] = {0};
    _socketio.readLine(buff, sizeof(buff));

    return string(buff);
}

string TcpConnection::toString()
{
    
}

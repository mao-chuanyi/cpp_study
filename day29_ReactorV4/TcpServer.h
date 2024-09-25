#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__
#include "Acceptor.h"
#include "EventLoop.h"

class TcpServer
{
public:
    TcpServer(const string &ip, unsigned short port);
    ~TcpServer();
    void setAllCallback(TcpConnectionCallback &&cb1, TcpConnectionCallback &&cb2, TcpConnectionCallback &&cb3);
    void start();
    void stop();

private:
    Acceptor _acceptor;
    EventLoop _eventLoop;
};

#endif
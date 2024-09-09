#include "TcpServer.h"

TcpServer::TcpServer(const string &ip, unsigned short port)
    : _acceptor(ip, port), _eventLoop(_acceptor)
{
}

TcpServer::~TcpServer()
{
}

void TcpServer::setAllCallback(TcpConnectionCallback &&cb1, TcpConnectionCallback &&cb2, TcpConnectionCallback &&cb3)
{
    _eventLoop.setNewConnectionCallback(std::move(cb1));
    _eventLoop.setMessageCallback(std::move(cb2));
    _eventLoop.setCloseCallback(std::move(cb3));
}

void TcpServer::start()
{
    _acceptor.ready();
    _eventLoop.loop();
}

void TcpServer::stop()
{
    _eventLoop.unloop();
}

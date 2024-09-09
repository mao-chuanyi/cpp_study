#include "TcpConnection.h"
#include "EventLoop.h"
#include <sys/socket.h>
#include <iostream>

using std::cout;
using std::endl;

TcpConnection::TcpConnection(int fd, EventLoop *loop)
    : _socketio(fd), _loop(loop)
{
}

TcpConnection::~TcpConnection()
{
}

void TcpConnection::send(const string &msg)
{
    _socketio.writen(msg.c_str(), msg.size());
}

string TcpConnection::receive()
{
    char buff[65536] = {0};
    _socketio.readLine(buff, sizeof(buff));

    return string(buff);
}

void TcpConnection::setNewConnectionCallback(const TcpConnectionCallback &cb)
{
    _onNewConnectionCb = cb;
}
void TcpConnection::setMessageCallback(const TcpConnectionCallback &cb)
{
    _onMessageCb = cb;
}
void TcpConnection::setCloseCallback(const TcpConnectionCallback &cb)
{
    _onCloseCb = cb;
}

void TcpConnection::handleNewConnectionCallback()
{
    if (_onNewConnectionCb)
    {
        _onNewConnectionCb(shared_from_this());
    }
    else
    {
        cout << "_onNewConnectionCb == nullptr" << endl;
    }
}

void TcpConnection::handleMessageCallback()
{
    if (_onMessageCb)
    {
        _onMessageCb(shared_from_this());
    }
    else
    {
        cout << "_onNewConnectionCb == nullptr" << endl;
    }
}

void TcpConnection::handleCloseCallback()
{
    if (_onCloseCb)
    {
        _onCloseCb(shared_from_this());
    }
    else
    {
        cout << "_onCloseCb == nullptr" << endl;
    }
}

bool TcpConnection::isClose()
{
    char buf[10] = {0};
    int ret = ::recv(_socketio.fd(), buf, sizeof(buf), MSG_PEEK);

    return (ret == 0);
}

// 将send函数和处理好之后的数据一起打包交给EventLoop
void TcpConnection::sendInLoop(const string &msg)
{
    if (_loop)
    {
        _loop->runInLoop(bind(&TcpConnection::send, this, msg)); // 这里很巧妙，在一个类中用类一个类的对象调用成员函数
                                                                 // 从而实现将这个类中的数据传递给另一个类
    }
}
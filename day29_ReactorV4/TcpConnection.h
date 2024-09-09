#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__
#include "SocketIO.h"
#include <string>
#include <memory>
#include <functional>

using std::bind;
using std::function;
using std::shared_ptr;
using std::string;

class TcpConnection;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;

class EventLoop;

class TcpConnection
    : public std::enable_shared_from_this<TcpConnection>
{
public:
    explicit TcpConnection(int fd, EventLoop *loop);
    ~TcpConnection();
    void send(const string &msg);
    string receive();
    void setNewConnectionCallback(const TcpConnectionCallback &cb);
    void setMessageCallback(const TcpConnectionCallback &cb);
    void setCloseCallback(const TcpConnectionCallback &cb);

    void handleNewConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

    bool isClose();

    void sendInLoop(const string &msg);

private:
    SocketIO _socketio;
    TcpConnectionCallback _onNewConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;

    EventLoop *_loop;
};

#endif

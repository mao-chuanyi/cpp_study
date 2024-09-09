#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include <vector>
#include <memory>
#include <map>
#include <functional>
#include <mutex>

using std::function;
using std::map;
using std::mutex;
using std::shared_ptr;
using std::vector;

class Acceptor;
class TcpConnection;

using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;
using Functor = function<void()>;

class EventLoop
{
public:
    EventLoop(Acceptor &acceptor);
    ~EventLoop();
    void loop();
    void unloop();
    void setNewConnectionCallback(TcpConnectionCallback &&cb);
    void setMessageCallback(TcpConnectionCallback &&cb);
    void setCloseCallback(TcpConnectionCallback &&cb);

    void wakeUp();
    void runInLoop(Functor &&cb);

private:
    void waitEpollFd();
    void handleNewConnection();
    void handleMessage(int fd);
    int createEpollFd();
    void addEpollReadFd(int fd);
    void delEpollReadFd(int fd);

    void handleRead();
    void doPendingFunctor(); // 将存放在vector中的任务遍历执行
    int createEventFd();

private:
    int _epfd;
    vector<struct epoll_event> _evtList;
    bool _isLooping;
    Acceptor &_acceptor;
    map<int, TcpConnectionPtr> _conns;
    TcpConnectionCallback _onNewConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;

    int _evtfd;                // 用于线程池和eventLoop之间通信的文件描述符
    vector<Functor> _pendings; // 待执行的任务，即发送给客户端处理好之后的数据的函数对象，即回调函数
    mutex _mutex;              // 控制对共享资源pendings的互斥访问

};

#endif

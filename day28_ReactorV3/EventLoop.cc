#include "EventLoop.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include <sys/epoll.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;
using std::make_shared;

EventLoop::EventLoop(Acceptor &acceptor)
    : _epfd(createEpollFd()), _evtList(1024), _isLooping(false), _acceptor(acceptor)
{
    // 将listenfd添加到红黑树上进行监听
    addEpollReadFd(_acceptor.fd());
}

EventLoop::~EventLoop()
{
    close(_epfd);
}

void EventLoop::loop()
{
    _isLooping = true;
    while (_isLooping)
    {
        waitEpollFd();
    }
}

void EventLoop::unloop()
{
    _isLooping = false;
}

void EventLoop::waitEpollFd()
{
    int nready = 0;
    do
    {
        nready = ::epoll_wait(_epfd, &*_evtList.begin(), _evtList.size(), 3000);
    } while (nready == -1 && errno == EINTR);

    if (nready == -1)
    {
        cerr << "-1 == nready" << endl;
        return;
    }
    else if (nready == 0)
    {
        cout << ">>epoll_wait timeout" << endl;
    }
    else
    {
        // 扩容
        if (nready == (int)_evtList.size())
        {
            _evtList.reserve(2 * nready);
        }

        for (size_t idx = 0; (int)idx < nready; ++idx)
        {
            int fd = _evtList[idx].data.fd;

            if (!(_evtList[idx].events & EPOLLIN))
            {
                continue;
            }

            if (fd == _acceptor.fd()) // 有新的连接
            {
                handleNewConnection();
            }
            else // 老的连接
            {
                handleMessage(fd);
            }
        }
    }
}

void EventLoop::handleNewConnection()
{
    int connfd = _acceptor.accept();
    if (connfd < 0)
    {
        perror("handleNewConnection accept");
        return;
    }

    // 将新的连接放在红黑树上进行监听
    addEpollReadFd(connfd);

    // 创建TCP连接对象的智能指针
    TcpConnectionPtr conptr = make_shared<TcpConnection>(connfd);

    // 将回调函数传递给TcpConnection对象
    conptr->setNewConnectionCallback(_onNewConnectionCb);
    conptr->setMessageCallback(_onMessageCb);
    conptr->setCloseCallback(_onCloseCb);

    // 加入map键值对
    _conns[connfd] = conptr;

    // 连接建立的事件触发
    conptr->handleNewConnectionCallback();
}

void EventLoop::handleMessage(int fd)
{
    // 根据fd在map键值对中找出对应的TCP连接的对象
    auto it = _conns.find(fd);

    // 找到
    if (it != _conns.end())
    {
        bool flag = it->second->isClose(); // 检查客户端是否与服务器断开连接
        if (flag)
        {
            // 连接断开的事件触发
            it->second->handleCloseCallback();
            // 从红黑树监听中移除
            delEpollReadFd(fd);
            // 从map键值对中移除
            _conns.erase(it);
        }
        else
        {
            it->second->handleMessageCallback(); // 消息到达事件触发
        }
    }
    else
    {
        cout << "连接不存在" << endl;
        return;
    }
}

int EventLoop::createEpollFd()
{
    int epfd = ::epoll_create(100);
    if (epfd == -1)
    {
        perror("epoll_creat error");
        return epfd;
    }
    return epfd;
}

void EventLoop::addEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.events = EPOLLIN;
    evt.data.fd = fd;
    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &evt);
    if (ret == -1)
    {
        perror("epoll_ctl error");
        return;
    }
}

void EventLoop::delEpollReadFd(int fd)
{

    struct epoll_event evt;
    evt.events = EPOLLIN;
    evt.data.fd = fd;
    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &evt);
    if (ret == -1)
    {
        perror("epoll_ctl error");
        return;
    }
}

void EventLoop::setNewConnectionCallback(TcpConnectionCallback &&cb)
{
    _onNewConnectionCb = std::move(cb);
}
void EventLoop::setMessageCallback(TcpConnectionCallback &&cb)
{
    _onMessageCb = std::move(cb);
}
void EventLoop::setCloseCallback(TcpConnectionCallback &&cb)
{
    _onCloseCb = std::move(cb);
}

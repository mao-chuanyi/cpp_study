#include "EventLoop.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include <sys/epoll.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sys/eventfd.h>

using std::cerr;
using std::cout;
using std::endl;
using std::make_shared;
using std::unique_lock;

EventLoop::EventLoop(Acceptor &acceptor)
    : _epfd(createEpollFd()), _evtList(1024), _isLooping(false), _acceptor(acceptor), _evtfd(createEventFd())
{
    // 将listenfd添加到红黑树上进行监听
    addEpollReadFd(_acceptor.fd());

    // 将_evtfd添加到红黑树上进行监听
    addEpollReadFd(_evtfd);
}

EventLoop::~EventLoop()
{
    close(_epfd);
    close(_evtfd);
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
            else if (fd == _evtfd) // 就绪的是通信的文件描述符，即线程池将数据处理好了
            {
                handleRead();       // 可能陷入阻塞
                doPendingFunctor(); // 执行任务，即发送数据给客户端
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
    TcpConnectionPtr conptr = make_shared<TcpConnection>(connfd, this);

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

void EventLoop::handleRead()
{
    uint64_t two;
    ssize_t ret = read(_evtfd, &two, sizeof(uint64_t)); // 在消费者线程中读取事件，
                                                        // 读取值为 1，并且将计数器清零。
    if (ret != sizeof(uint64_t))
    {
        perror("read");
        return;
    }
}
void EventLoop::doPendingFunctor() // 将存放在vector中的任务遍历执行
{
    vector<Functor> tmp;
    {
        unique_lock<mutex> mutx(_mutex); // 加锁
        tmp.swap(_pendings);             // 先取出之后再执行，减小锁的粒度提高并发
    }
    for (auto &cb : tmp)
    {
        cb(); // 回调，即send函数和msg的打包，即可将处理之后的数据发送给客户端
    }
}

int EventLoop::createEventFd()
{
    int evtfd = eventfd(10, 0); // 创建通信文件描述符，计数器初始值为10
    if (_evtfd == -1)
    {
        perror("eventfd");
    }
    return evtfd;
}
void EventLoop::wakeUp()
{
    uint64_t one = 1;
    // std::cout << "Writing to evtfd: " << _evtfd << std::endl;
    ssize_t ret = write(_evtfd, &one, sizeof(uint64_t)); // 生产者线程中写入事件，值增加1
    if (ret != sizeof(uint64_t))
    {
        perror("write in EventLoop");
        return;
    }
}
void EventLoop::runInLoop(Functor &&cb)
{
    unique_lock<mutex> mutx(_mutex);    // 加锁
    _pendings.push_back(std::move(cb)); // 完成传递给EventLoop

    // 通知EventLoop
    mutx.unlock();
    wakeUp();
}
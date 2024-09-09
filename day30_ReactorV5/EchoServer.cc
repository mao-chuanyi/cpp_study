#include "EchoServer.h"
#include "TcpConnection.h"
#include "MyTask.h"

EchoServer::EchoServer(size_t threadNum, size_t queSize, const string &ip, unsigned short port)
    : _pool(threadNum, queSize), _server(ip, port)
{
}

EchoServer::~EchoServer()
{
}

void EchoServer::start()
{
    _pool.start(); // 启动线程池
    using namespace std::placeholders;
    _server.setAllCallback(std::bind(&EchoServer::onNewConnection, this, _1),
                           std::bind(&EchoServer::onMessage, this, _1),
                           std::bind(&EchoServer::onClose, this, _1));
    _server.start();
}

void EchoServer::stop()
{
    _pool.stop();
    _server.stop();
}

void EchoServer::onNewConnection(const TcpConnectionPtr &con)
{
    cout << "have connected!" << endl;
}

void EchoServer::onMessage(const TcpConnectionPtr &con)
{
    // 接收信息
    string msg = con->receive();
    cout << "<< receive message: " << msg << endl;
    // 将信息的处理交给线程池
    MyTask task(msg, con);
    _pool.addTask(bind(&MyTask::process, &task));
}

void EchoServer::onClose(const TcpConnectionPtr &con)
{
    cout << "have closed!" << endl;
}
#include "EventLoop.h"
#include "TcpConnection.h"
#include "Acceptor.h"
#include <iostream>
#include <string>
#include <utility>

using std::cout;
using std::endl;
using std::string;

void onNewConnection(const TcpConnectionPtr &con)
{
    cout << "have connected!" << endl;
}

void onMessage(const TcpConnectionPtr &con)
{
    // 接收信息
    cout << "<< receive message: " << con->receive() << endl;
    // 处理信息
    // 。。。
    // 。。。
    // 。。。
    string backmessage = "I have receive your message";
    con->send(backmessage);
}

void onClose(const TcpConnectionPtr &con)
{
    cout << "have closed!" << endl;
}

void test()
{
    Acceptor acceptor("127.0.0.1", 8888);
    acceptor.ready();

    EventLoop el(acceptor);
    el.setNewConnectionCallback(std::move(onNewConnection));
    el.setMessageCallback(std::move(onMessage));
    el.setCloseCallback(std::move(onClose));

    el.loop();
}

int main()
{
    test();
    return 0;
}

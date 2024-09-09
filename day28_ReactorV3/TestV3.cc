#include "TcpConnection.h"
#include "TcpServer.h"
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
    string backmessage = "I have receive your message\n";
    con->send(backmessage);
}

void onClose(const TcpConnectionPtr &con)
{
    cout << "have closed!" << endl;
}

void test()
{
    TcpServer tcp("127.0.0.1", 8888);
    tcp.setAllCallback(onNewConnection, onMessage, onClose);
    tcp.start();
}

int main()
{
    test();
    return 0;
}

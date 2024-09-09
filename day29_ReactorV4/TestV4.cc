#include "TcpConnection.h"
#include "ThreadPool.h"
#include "TcpServer.h"
#include <iostream>
#include <string>
#include <utility>

using std::cout;
using std::endl;
using std::string;

ThreadPool *gpool = nullptr;
class MyTask
{
public:
    MyTask(const string &msg, const TcpConnectionPtr &con)
    :_msg(msg)
    ,_con(con)
    {}
    ~MyTask()
    {}
    void process()
    {
        //处理msg
        //...
        //...
        //将处理好的msg交给EventLoop
        _con->sendInLoop(_msg);
    }

private:
    string _msg;
    TcpConnectionPtr _con;//TcpConnection对象的智能指针
};

void onNewConnection(const TcpConnectionPtr &con)
{
    cout << "have connected!" << endl;
}

void onMessage(const TcpConnectionPtr &con)
{
    // 接收信息
    string msg = con->receive();
    cout << "<< receive message: " << msg << endl;
    // 将信息的处理交给线程池
    MyTask task(msg, con);
    gpool->addTask(bind(&MyTask::process, &task));
}

void onClose(const TcpConnectionPtr &con)
{
    cout << "have closed!" << endl;
}

void test()
{
    //启动线程池
    ThreadPool pool(3, 10);
    pool.start();
    gpool = &pool;

    TcpServer tcp("127.0.0.1", 8888);
    tcp.setAllCallback(onNewConnection, onMessage, onClose);
    tcp.start();
}

int main()
{
    test();
    return 0;
}

#include "MyTask.h"

MyTask::MyTask(const string &msg, const TcpConnectionPtr &con)
    : _msg(msg), _con(con)
{
}

void MyTask::process()
{
    // 处理msg
    //...
    //...
    // 将处理好的msg交给EventLoop
    _con->sendInLoop(_msg);
}
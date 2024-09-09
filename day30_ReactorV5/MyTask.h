#ifndef __MYTASK_H__
#define __MYTASK_H__
#include "TcpConnection.h"
#include <string>

using std::string;

class MyTask
{
public:
    MyTask(const string &msg, const TcpConnectionPtr &con);
    void process();

private:
    string _msg;
    TcpConnectionPtr _con; // TcpConnection对象的智能指针
};
#endif
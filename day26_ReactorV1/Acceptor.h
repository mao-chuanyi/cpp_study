#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__
#include "Socket.h"
#include "InteAddress.h"
#include <string>

using std::string;

class Acceptor
{
public:
    Acceptor(const string & ip, unsigned short port);
    ~Acceptor();
    void ready();
    int accept();
private:
    void setReuseAddr();
    void setReusePort();
    void bind();
    void listen();  
private:
    Socket _socket;
    InteAddress _addr;
};

#endif

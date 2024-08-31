#include "Acceptor.h"

Acceptor::Acceptor(const string &ip, unsigned short port)
:_socket()
,_addr(ip, port)
{    
}

Acceptor::~Acceptor()
{

}

void Acceptor::setReuseAddr()
{
    int opt = 1;
    int ret = ::setsockopt(_socket.fd(), SOL_SOCKET, SO_REUSEADDR,&opt, sizeof(opt));
    if(ret < 0)
    {
        perror("setsockopt");
        return;
    }
}

void Acceptor::setReusePort()
{
    int opt = 1;
    int ret = ::setsockopt(_socket.fd(), SOL_SOCKET, SO_REUSEPORT,&opt, sizeof(opt));
    if(ret < 0)
    {
        perror("setsockopt");
        return;
    }

}

void Acceptor::bind()
{
    int ret = ::bind(_socket.fd(), (struct sockaddr *)_addr.getInteAddressPtr(), sizeof(struct sockaddr));
    if(ret)
    {
        perror("bind");
        return;
    }
}   


void Acceptor::listen()
{
    int ret = ::listen(_socket.fd(), 128);
    if(ret < 0)
    {
        perror("listen");
        return;
    }
}

void Acceptor::ready()
{
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}

int Acceptor::accept()
{
    int confd =  ::accept(_socket.fd(), nullptr, nullptr);
    if(-1 == confd)
    {
        perror("accept");
        return -1;
    }
    return confd;
}

int Acceptor::fd()
{
    return _socket.fd();
}

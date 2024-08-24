#include "InteAddress.h"
#include <strings.h>

InteAddress::InteAddress(const string & ip,  unsigned short port)
{
    ::bzero(&_addr, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
    _addr.sin_port = htons(port);
}
InteAddress::InteAddress(const struct sockaddr_in & addr)
:_addr(addr)
{
}
InteAddress::~InteAddress()
{}

string InteAddress::ip() const
{
    return string(inet_ntoa(_addr.sin_addr));
}
unsigned short InteAddress::port() const
{
    return ntohs(_addr.sin_port);
}
const struct sockaddr_in * InteAddress::getInteAddressPtr() const
{
    return &_addr;
}


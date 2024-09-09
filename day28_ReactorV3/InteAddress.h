#ifndef __INTEADDRESS_H__
#define __INTEADDRESS_H__
#include <arpa/inet.h>
#include <string>

using std::string;

class InteAddress
{
public:
    InteAddress(const string & ip,  unsigned short port);
    InteAddress(const struct sockaddr_in & addr);
    ~InteAddress();
    string ip() const;
    unsigned short port() const;
    const struct sockaddr_in * getInteAddressPtr() const;
private:
    struct sockaddr_in _addr;
};

#endif

#ifndef __SOCKET_H__
#define __SOCKET_H__

class Socket
{
public:
    Socket();
    explicit Socket(int fd);
    ~Socket(); 
    int fd() const;

private:
    int _fd;
};

#endif

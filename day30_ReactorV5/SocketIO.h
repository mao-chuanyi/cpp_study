#ifndef __SOCKETIO_H__
#define __SOCKETIO_H__

class SocketIO
{
public:
    SocketIO(int fd);
    ~SocketIO();
    int readn(char * buf, int len);
    int readLine(char *buf, int len);//接收一行
    int writen(const char * buf, int len);//发送
    int fd();
private:
    int _fd;
};

#endif

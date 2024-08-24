#include "SocketIO.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>

SocketIO::SocketIO(int fd)
:_fd(fd)
{

}

SocketIO::~SocketIO()
{
    close(_fd);
}

int SocketIO::readn(char * buf, int len)
{
    int left = len;//剩余未读取的字节数
    char *pstr = buf;
    int ret  = 0;//接收read的返回值
                 
    while(left > 0)
    {
        ret = read(_fd, pstr, left);//将内核缓冲区的内容读出并清空
        if(-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("read error -1");
            return -1;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            pstr += ret;
            left -= ret;
        }
    }
    return len - left;

}

int SocketIO::readLine(char *buf, int len)//接收一行
{
    int left = len - 1;//略过最后一个换行符
    char *pstr = buf;
    int ret = 0, total = 0;

    while (left > 0)
    {
        ret = recv(_fd, pstr, left, MSG_PEEK);//MSG_PEEK的作用在于接收的数据不从接收缓冲区中删除数据
        if(-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("readLine error -1");
            return -1;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            for (int idx = 0; idx < ret; ++idx)//将读入的内容进行遍历找到\n
            {
                if(pstr[idx] == '\n')
                {
                    int sz = idx + 1;
                    //将\n以及之前的内容再读入内存
                    readn(pstr, sz);
                    pstr += sz;
                    *pstr = '\0';

                    return total + sz;//因为只读一行，所以读到'\n'就返回
                }
            }

            readn(pstr, ret);//若没有发现'\n'，则从内核态拷贝到用户态，并且删除内存缓冲区中的这部分
            total += ret;
            pstr += ret;
            left -= ret;
        }
    }
    *pstr = '\0';

    return total - left;
}

int SocketIO::writen(const char * buf, int len)//发送
{
    int left = len;
    const char * pstr = buf;
    int ret = 0;
    while (left > 0)
    {
        ret = write(_fd, pstr, left);
        if(-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("write error -1");
            return -1;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            pstr += ret;
            left -= ret;
        }
    }
    return len - left;
}

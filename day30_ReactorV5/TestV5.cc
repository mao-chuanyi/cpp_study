#include "EchoServer.h"


void test()
{
    EchoServer echoserver(4, 10, "127.0.0.1", 8888);
    echoserver.start();
}

int main()
{
    test();
    return 0;
}

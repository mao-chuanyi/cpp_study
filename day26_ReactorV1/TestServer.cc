#include "Acceptor.h"
#include "TcpConnection.h"
#include <iostream>
using std::cout;
using std::endl;

void test(){
    Acceptor acceptor("127.0.0.1", 8888);
    acceptor.ready();
    
    TcpConnection con(acceptor.accept());//三次握手成功

    while(1)
    {
        cout << ">>recv msg from client:" << con.receive() << endl;
        con.send("hello bady\n");
    }
}

int main(){
    test();
    return 0;
}


#include <workflow/WFFacilities.h>
#include <signal.h>
#include <iostream>

using std::cout;

//全局静态变量：作用域只在本文件中
static WFFacilities::WaitGroup gWaitGroup(1);//设置计数器为1


void handler(int signum)//信号到达时要处理的函数
{
    printf("signum = %d\n", signum);
    gWaitGroup.done();//计数器减1
}

int main(){
    signal(SIGINT, handler);//注册信号和信号函数
    cout << "1\n" ;
    gWaitGroup.wait();//等待（阻塞）直到计数器为0
    return 0;
}


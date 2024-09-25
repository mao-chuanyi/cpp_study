#include <workflow/WFFacilities.h>
#include <workflow/HttpMessage.h>
#include <workflow/HttpUtil.h>
#include <signal.h>
#include <iostream>

using std::cout;

// 全局静态变量：作用域只在本文件中
static WFFacilities::WaitGroup gWaitGroup(1); // 设置计数器为1

void handler(int signum) // 信号到达时要处理的函数
{
    printf("signum = %d\n", signum);
    gWaitGroup.done(); // 计数器减1
}

int main()
{
    signal(SIGINT, handler); // 注册信号和信号函数
    cout << "1\n";

    // 利用工厂函数创建一个任务
    WFHttpTask *httpTask = WFTaskFactory::create_http_task("http://192.168.88.150:1234", 10, 0, nullptr);
    cout << "2\n";

    // 任务派给框架前设置httpTask的属性 请求
    protocol::HttpRequest *req = httpTask->get_req(); // 获取请求的指针
    req->add_header_pair("Accept", "*/*");            // 添加一个首部字段

    // 将任务派给框架，由框架异步调用
    httpTask->start();
    cout << "3\n"; // 由于异步的特点，所有这个3不会等到上面的任务执行完了再打印，会直接打印

    gWaitGroup.wait(); // 等待（阻塞）直到计数器为0
    return 0;
}

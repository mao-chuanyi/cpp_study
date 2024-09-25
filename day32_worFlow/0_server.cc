#include <workflow/WFFacilities.h>
#include <workflow/HttpMessage.h>
#include <workflow/WFHttpServer.h>
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

void process(WFHttpTask *serverTask)
{
    protocol::HttpResponse *resp = serverTask->get_resp();
    resp->set_http_version("HTTP/1.1");
    resp->set_status_code("200");
    resp->set_reason_phrase("OK");
    resp->set_header_pair("Content-Type", "text/html");
    // workflow框架会自动设置connection content-length
    resp->append_output_body("<html>hello</html>");
}

int main()
{
    signal(SIGINT, handler); // 注册信号和信号函数
    WFHttpServer server(process);
    server.start(1234); // listen 1234端口
    gWaitGroup.wait();  // 等待（阻塞）直到计数器为0
    server.stop();
    return 0;
}

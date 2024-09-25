#include <workflow/WFFacilities.h>
#include <workflow/HttpMessage.h>
#include <workflow/HttpUtil.h>
#include <signal.h>
#include <iostream>
#include <string>

using std::cout;
using std::string;

// 全局静态变量：作用域只在本文件中
static WFFacilities::WaitGroup gWaitGroup(1); // 设置计数器为1

void handler(int signum) // 信号到达时要处理的函数
{
    printf("signum = %d\n", signum);
    gWaitGroup.done(); // 计数器减1
}

// 任务做完收到响应后会执行该回调函数
void httpCallback(WFHttpTask *httpTask) // 这个httpTask和main函数中的httpTask是同一个
{
    cout << "4\n";
    protocol::HttpRequest *req = httpTask->get_req();
    protocol::HttpResponse *resp = httpTask->get_resp();

    int state = httpTask->get_state();
    if (state != WFT_STATE_SUCCESS)
    {
        cout << "state error\n";
        return;
    }

    // 提取请求首部字段
    protocol::HttpHeaderCursor cursorReq(req);
    string key;
    string value;
    while (cursorReq.next(key, value))
    {
        cout << "request key = " << key << ", value = " << value << "\n";
    }

    // 提取响应的首部字段
    protocol::HttpHeaderCursor cursorResp(resp);
    while (cursorResp.next(key, value))
    {
        cout << "response key = " << key << ", value = " << value << "\n";
    }

    // 提取响应体内容
    const void *body;
    size_t size;
    resp->get_parsed_body(&body, &size);
    cout << string((char *)body, size) << "\n";
}

int main()
{
    signal(SIGINT, handler); // 注册信号和信号函数
    cout << "1\n";

    // 利用工厂函数创建一个任务
    WFHttpTask *httpTask = WFTaskFactory::create_http_task(
        "http://192.168.88.150:1234", 10, 0, httpCallback);
    // "http://baidu.com", 10, 0, httpCallback);
    cout << "2\n";

    // 任务派给框架前设置httpTask的请求属性（输入）
    protocol::HttpRequest *req = httpTask->get_req(); // 获取请求的指针
    req->add_header_pair("Accept", "*/*");            // 添加一个首部字段

    // 将任务派给框架，由框架异步调用
    httpTask->start();
    cout << "3\n"; // 由于异步的特点，所有这个3不会等到上面的任务执行完了再打印，会直接打印

    gWaitGroup.wait(); // 等待（阻塞）直到计数器为0
    return 0;
}

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

void redisCallback(WFRedisTask *redisTask)
{
    protocol::RedisValue value;
    redisTask->get_resp()->get_result(value);        // 获取redis命令的返回结果
    if (redisTask->get_state() == WFT_STATE_SUCCESS) // 确定没有系统错误，且返回的是字符串类型
    {
        if (value.is_string())
        {
            string result = value.string_value(); // 提取value的字符串类型的值
            if (result == "100")
            {
                cout << result << "\n";
                gWaitGroup.done(); // 结束任务
            }
            else
            {
                // 继续递归
                cout << result << "\n";
                WFRedisTask *next_redisTask = WFTaskFactory::create_redis_task(
                    "redis://127.0.0.1:6379", 0, redisCallback);
                next_redisTask->get_req()->set_request("GET", {result});
                series_of(redisTask)->push_back(next_redisTask);
            }
        }
        else if (value.is_nil())
        {
            cout << "key dosent exist in redis\n";
        }
        else
        {
            cout << "Unexpected data type returned!\n";
        }
    }
    else
    {
        cout << "Failed\n";
    }
}

int main()
{
    signal(SIGINT, handler); // 注册信号和信号函数

    // 利用工厂函数创建一个任务
    WFRedisTask *redisTask = WFTaskFactory::create_redis_task(
        "redis://127.0.0.1:6379", 0, redisCallback);

    // 任务派给框架前设置Task的请求属性（输入）
    protocol::RedisRequest *req = redisTask->get_req(); // 获取请求的指针
    req->set_request("GET", {"x1"});

    // 将任务派给框架，由框架异步调用
    redisTask->start();

    gWaitGroup.wait(); // 等待（阻塞）直到计数器为0
    return 0;
}

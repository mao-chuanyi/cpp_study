#include <workflow/WFFacilities.h>
#include <workflow/WFTaskFactory.h>
#include <workflow/HttpMessage.h>
#include <workflow/HttpUtil.h>
#include <iostream>
#include <string>

using namespace protocol;
using std::string;

WFFacilities::WaitGroup wait_group(1);

void redis_callback(WFRedisTask *task)
{
    if (task->get_state() == WFT_STATE_SUCCESS)
    {
        std::cout << "Image successfully stored in Redis." << std::endl;
    }
    else
    {
        std::cerr << "Failed to store image in Redis." << std::endl;
    }
    wait_group.done();
}

void http_callback(WFHttpTask *task)
{
    auto *resp = task->get_resp();
    int state = task->get_state();
    int error = task->get_error();

    if (state == WFT_STATE_SUCCESS)
    {
        std::cout << "HTTP request succeeded." << std::endl;
        const void *body;
        size_t body_len;
        resp->get_parsed_body(&body, &body_len);

        // 图片下载成功，准备存入 Redis
    WFRedisTask *redis_task = WFTaskFactory::create_redis_task(
        "redis://127.0.0.1:6379", 0, redis_callback  
    );

        redis_task->get_req()->set_request("SET", {"image:my_image", string((const char *)body, body_len)});
        redis_task->start(); // 启动 Redis 存储任务
    }
    else
    {
        std::cerr << "HTTP request failed, state = " << state << ", error = " << error << std::endl;
        wait_group.done();
    }
}

int main()
{
    // 创建 HTTP 任务，请求图片
    WFHttpTask *task = WFTaskFactory::create_http_task("http://192.168.88.150:1234", 3, 2, http_callback);
    task->start();

    // 等待任务完成
    wait_group.wait();
    return 0;
}

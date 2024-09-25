#include <workflow/WFFacilities.h>
#include <workflow/HttpMessage.h>
#include <workflow/WFHttpServer.h>
#include <nlohmann/json.hpp>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>

using nlohmann::json;
using std::cerr;
using std::cout;
using std::endl;
using std::ofstream;
using std::string;

// 全局静态变量：作用域只在本文件中
static WFFacilities::WaitGroup gWaitGroup(1); // 设置计数器为1

void handler(int signum) // 信号到达时要处理的函数
{
    printf("signum = %d\n", signum);
    gWaitGroup.done(); // 计数器减1
}
class Callback
{
public:
    void process(WFHttpTask *serverTask)
    {
        resp = serverTask->get_resp(); // 设置成员变量
        req = serverTask->get_req();
        series = series_of(serverTask);
        string method = req->get_method();
        string uri = req->get_request_uri();
        path = uri.substr(0, uri.find('?'));
        cout << method << endl;
        cout << path << endl;
        // 初始化的工作
        if (method == "POST" && path == "/file/mupload/init")
        {
            // 获取bady,解析json
            req->get_parsed_body(&body, &body_len);
            string body_json((const char *)body, body_len);
            json json_obj = json::parse(body_json);
            string filename = json_obj["filename"];
            string filehash = json_obj["filehash"];
            size_t filesize = json_obj["filesize"];
            // cout << filename << endl;
            // cout << filehash << endl;
            // cout << filesize << endl;

            // 生成分片信息 uploadID:username+date  chunksize  count
            string uploadID = get_file_id();
            size_t chunksize = 1024 * 1024 * 5; // 5MB
            size_t chunkcount = (filesize + chunksize - 1) / chunksize;

            // 将分片信息存入redis
            // HSET uploadID {fliename xxx, hash xxx, size xxx, chunksize xxx, chunkcount xxx}
            WFRedisTask *redisTask = WFTaskFactory::create_redis_task(
                "redis://127.0.0.1:6379", 0, std::bind(&Callback::redisCallback, this, std::placeholders::_1));
            protocol::RedisRequest *redis_req = redisTask->get_req();
            redis_req->set_request("HSET",
                                   {uploadID,
                                    "filename", filename,
                                    "filehash", filehash,
                                    "filesize", std::to_string(filesize),
                                    "chunksize", std::to_string(chunksize),
                                    "chunkcount", std::to_string(chunkcount)});
            resp_json = {
                {"status:", "OK"},
                {"uploadID", uploadID},
                {"chunkcount", chunkcount},
                {"chunksize", chunksize},
                {"filesize", filesize},
                {"filehash", filehash}};
            series->push_back(redisTask);

            // 回复给客户端,在redis任务的回调函数中回复
        }
        // 上传工作
        else if (method == "POST" && path == "/file/mupload/uppart")
        {
            //1.截取出uploadID 和 chunkidx 
            //  /file/mupload/uppart?uploadID=user_20240916015913&chkidx=1
            loadID = uri.substr(uri.find('=') + 1, uri.find('&') - uri.find('=') - 1);
            cout << loadID << endl;
            string chkidx_key = "chkidx=";
            chukidx = uri.substr(uri.find(chkidx_key) + chkidx_key.size());
            cout << "chukidx = " << chukidx << endl;
            //检查分片是否已经存在，防止重复上传

            //2.将报文体存在文件系统中
            req->get_parsed_body(&body, &body_len);
            //创建保存的文件目录,先在redis中用uploadID查找到filehash
            //然后用filehash创建文件夹,在redis的回调函数中找到filehash后再创建文件夹
            WFRedisTask *redisTaskHGET =
                WFTaskFactory::create_redis_task("redis://127.0.0.1:6379", 0, 
                std::bind(&Callback::redisCallback, this, std::placeholders::_1)
            );
            redisTaskHGET->get_req()->set_request("HGET", {loadID, "filehash"});
            series->push_back(redisTaskHGET);

            //3.redis中保存上传进度,在redisHGET的回调函数中进行 
        }
    }

    void redisCallback(WFRedisTask *redisTask)
    {
        protocol::RedisResponse *redis_resp = redisTask->get_resp();
        int state = redisTask->get_state();
        int error = redisTask->get_error();
        protocol::RedisValue val;
        if (state != WFT_STATE_SUCCESS)
        {
            // 错误处理
            if (state == WFT_STATE_SYS_ERROR)
                fprintf(stderr, "system error: %s\n", strerror(error));
            else if (state == WFT_STATE_DNS_ERROR)
                fprintf(stderr, "DNS error: %s\n", gai_strerror(error));

            resp->set_status_code("500");
            resp->set_reason_phrase("Internal Server Error");
            resp->append_output_body("Redis error or system failure.");
            return;
        }

        redis_resp->get_result(val);

        // Redis 返回错误或没有找到 filehash
        if (val.is_error() || val.is_nil())
        {
            cout << "Redis error or filehash not found\n";
            resp->set_status_code("404");
            resp->set_reason_phrase("Not Found");
            resp->append_output_body("filehash not found in Redis.");
            return;
        }
        // 获取 filehash 并创建相应的目录和文件
        if (val.is_string())
        {
            string filehash = val.string_value();
            cout << "filehash = " << filehash << "\n";

            // 创建目录
            mkdir(filehash.c_str(), 0777);
            string datapath = filehash + "/" + chukidx;
            ofstream ofs(datapath, std::ios::binary | std::ios::app);

            if (ofs.is_open())
            {
                cout << "Saving to path = " << datapath << endl;
                ofs.write(static_cast<const char *>(body), body_len);
                ofs.close();
            }
            else
            {
                cerr << "Failed to open file for writing.\n";
                resp->set_status_code("500");
                resp->set_reason_phrase("Internal Server Error");
                resp->append_output_body("File write error.");
                return;
            }

            // 保存上传进度到 Redis 中
            WFRedisTask *redisTaskHSET =
                WFTaskFactory::create_redis_task("redis://127.0.0.1:6379", 0,
                std::bind(&Callback::redisCallback, this, std::placeholders::_1)
            );
            protocol::RedisRequest *redis_req = redisTaskHSET->get_req();
            string chunk_val = "chunkidx_" + chukidx;
            redis_req->set_request("HSET", {loadID, chunk_val, chukidx});
            series->push_back(redisTaskHSET);

            // 返回成功消息
            resp->set_status_code("200");
            resp->set_reason_phrase("OK");
            resp->append_output_body(chukidx + " upload success!");
        }
        else
        {
            cerr << "Unexpected Redis response type\n";
            resp->set_status_code("500");
            resp->set_reason_phrase("Internal Server Error");
            resp->append_output_body("Unexpected Redis response.");
        }
    }

    // 获取当前时间戳
    string get_file_id()
    {
        // 获取当前时间点
        auto now = std::chrono::system_clock::now();

        // 转换为time_t类型
        std::time_t current_time = std::chrono::system_clock::to_time_t(now);

        // 使用std::put_time进行格式化
        std::stringstream ss;
        ss << std::put_time(std::localtime(&current_time), "%Y%m%d%H%M%S");

        // 生成唯一的文件ID
        return "user_" + ss.str();
    }

private:
    char buf[4096] = {0};
    protocol::HttpResponse *resp;
    protocol::HttpRequest *req;
    SeriesWork *series;
    json resp_json;
    string path;
    string chukidx;
    const void *body;
    size_t body_len;
    string loadID;
};

int main()
{
    signal(SIGINT, handler); // 注册信号和信号函数
    Callback callback;
    WFHttpServer server(std::bind(&Callback::process, &callback, std::placeholders::_1));
    if(server.start(1234) == 0) // listen 1234端口, 生成一个序列
    {
        gWaitGroup.wait();  // 等待（阻塞）直到计数器为0
        server.stop();
    }
    else
    {
        perror("server start error\n");
        exit(-1);
    }
    return 0;
}

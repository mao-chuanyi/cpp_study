#include <workflow/WFFacilities.h>
#include <workflow/HttpMessage.h>
#include <workflow/WFHttpServer.h>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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
class Task
{
public:
    void process(WFHttpTask *serverTask)
    {
        cout << "process callback\n";
        resp = serverTask->get_resp(); // 设置成员变量
        resp->set_http_version("HTTP/1.1");
        resp->set_status_code("200");
        resp->set_reason_phrase("OK");
        resp->set_header_pair("Content-Type", "text/html");
        // workflow框架会自动设置connection content-length
        protocol::HttpRequest *req = serverTask->get_req();
        string method = req->get_method();
        if (method == "GET")
        {
            int fd = open("login.html", O_RDONLY);
            if (fd == -1)
            {
                perror("Failed to open file");
                resp->append_output_body("Failed to open file");
                return;
            }
            WFFileIOTask *preadTask = WFTaskFactory::create_pread_task(fd, buf, 4096, 0, std::bind(&Task::fileReadCallback, this, std::placeholders::_1));
            series_of(serverTask)->push_back(preadTask);
        }
        else
        {
            resp->append_output_body("<html>hello</html>");
        }
    }

    void fileReadCallback(WFFileIOTask *preadTask)
    {
        cout << "fileread callback\n";
        if (preadTask->get_retval() > 0)
        {
            cout << "buf = \n"
                 << buf << "\n";
            resp->append_output_body(buf);
        }
        else
        {
            resp->append_output_body("Error reading file");
        }
        int fd = preadTask->get_args()->fd; // 获取文件描述符
        close(fd);                          // 关闭文件
    }

private:
    char buf[4096] = {0};
    protocol::HttpResponse *resp;
};
int main()
{
    signal(SIGINT, handler); // 注册信号和信号函数
    Task task;
    WFHttpServer server(std::bind(&Task::process, &task, std::placeholders::_1));
    server.start(1234); // listen 1234端口
    gWaitGroup.wait();  // 等待（阻塞）直到计数器为0
    server.stop();
    return 0;
}

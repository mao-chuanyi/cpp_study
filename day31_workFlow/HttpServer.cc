#include <arpa/inet.h>
#include <fstream>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <vector>

using std::cerr;
using std::string;
using std::to_string;
using std::vector;

class HttpServer
{
public:
    HttpServer(const string &ip, unsigned short port)
        : _ip(ip), _port(port), _sockfd(-1)
    {
    }
    ~HttpServer()
    {
        if (_sockfd > 0)
        {
            close(_sockfd);
        }
    }

    void start();       // socket->bind->listen
    void recvAndShow(); // accept->recv->cerr

    string response(); // 响应一个回复

private:
    string _ip;
    unsigned short _port;
    int _sockfd;
};

void HttpServer::start() // socket->bind->listen
{
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd < 0)
    {
        perror("socket");
        exit(1);
    }
    int reuse = 1;
    int ret = setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    if (ret < 0)
    {
        perror("setsockopt");
        exit(1);
    }
    ret = setsockopt(_sockfd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse));
    if (ret < 0)
    {
        perror("setsockopt");
        exit(1);
    }
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(_ip.c_str());
    serverAddr.sin_port = htons(_port);

    ret = ::bind(_sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (ret < 0)
    {
        perror("bind");
        exit(1);
    }

    listen(_sockfd, 10); // 最大发起连接数为10
}

void HttpServer::recvAndShow() // accept->recv->cerr
{
    while (true)
    {
        int netfd = accept(_sockfd, nullptr, nullptr);
        char buf[4096] = {0};
        ssize_t sret = recv(netfd, buf, sizeof(buf), 0);
        printf("sret = %ld, buf = %s\n", sret, buf);

        string resp = response(); // 回复一个响应
        send(netfd, resp.c_str(), resp.size(), 0);

        close(netfd);
    }
}

// string HttpServer::response() // 响应一个回复
// {
//     string startLine = "HTTP/1.1 200 OK\r\n";
//     string headers = "Server: MyHttpServer\r\n";
//     string body = "<html>hello world</html>";
//     headers += "Content-Type: text/html\r\n";
//     headers += "Content-Length: " + to_string(body.size()) + "\r\n";
//     string emptyLine = "\r\n";
//     return startLine + headers + emptyLine + body;
// }

string HttpServer::response()
{
    string startLine = "HTTP/1.1 200 OK\r\n";
    string headers = "Server: MyHttpServer\r\n";
    headers += "Content-Type: image/jpeg\r\n"; // 图片类型
    headers += "Connection: close\r\n";        // 连接关闭后立即断开连接
    string emptyLine = "\r\n";

    // 读取图片文件
    // binary二进制、ate追加到末尾
    ifstream imageFile("./ml.jpg", std::ios::binary | std::ios::ate);
    if (!imageFile.is_open())
    {
        cerr << "Failed to open image file." << std::endl;
        return "";
    }

    // 获取图片大小
    std::streampos imageSize = imageFile.tellg();
    imageFile.seekg(0, std::ios::beg); // 到文件开头

    // 读取图片数据到缓冲区
    vector<char> imageData(imageSize);
    imageFile.read(imageData.data(), imageSize);

    // 构建响应
    string response = startLine + headers + emptyLine;
    response.insert(response.end(), imageData.begin(), imageData.end());

    return response;
}

int main()
{
    HttpServer http("192.168.88.150", 1234);
    http.start();
    http.recvAndShow();
    return 0;
}
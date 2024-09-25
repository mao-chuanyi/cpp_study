#ifndef __WD_CloudiskServer_HPP__ 
#define __WD_CloudiskServer_HPP__ 

#include <workflow/WFFacilities.h>
#include <wfrest/HttpServer.h>
#include <memory>
#include <string>

using namespace wfrest;
using std::string;
using std::unique_ptr;

struct MQInfo
{
    string Exchange = "Exchange_test";
    string RoutingKey = "key1";
};

class CloudiskServer
{
public:
    CloudiskServer(int cnt)
    : _waitGroup(cnt)
    {
    }

    ~CloudiskServer() 
    {
    }

    void start(unsigned short port);

    void loadModules();

private:
    //模块化的思维方式写代码
    void loadStaticResourceModule();
    void loadUserRegisterModule();
    void loadUserLoginModule();
    void loadUserInfoModule();
    void loadFileQueryModule();
    void loadFileUploadModule();
    void loadFileDownloadModule();

    //回调函数
    void loadUserRegisterCallback(const HttpReq *req, HttpResp *resp, SeriesWork *series);
    void loadUserLoginCallback(const HttpReq *req, HttpResp *resp, SeriesWork *series);
    void loadUserInfoCallback(const HttpReq *req, HttpResp *resp, SeriesWork *series);
    void loadFileQueryCallback(const HttpReq *req, HttpResp *resp, SeriesWork *series);
    void loadFileUploadCallback(const HttpReq *req, HttpResp *resp, SeriesWork *series);
    void loadFileDownloadCallback(const HttpReq *req, HttpResp *resp, SeriesWork *series);

private:
    WFFacilities::WaitGroup _waitGroup;
    wfrest::HttpServer _httpserver;
    MQInfo _mqinfo;
};

#endif


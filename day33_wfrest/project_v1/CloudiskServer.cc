#include "CloudiskServer.h"
#include "Token.h"
#include "Hash.h"
#include "unixHeader.h"
#include "rpc_signup/signup.srpc.h"
#include "rpc_signin/signin.srpc.h"

#include <workflow/MySQLMessage.h>
#include <workflow/MySQLResult.h>
#include <wfrest/json.hpp>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

#include <iostream>

using namespace wfrest;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using namespace srpc;

void CloudiskServer::start(unsigned short port)
{
    if (_httpserver.track().start(port) == 0)
    {
        _httpserver.list_routes();
        _waitGroup.wait();
        _httpserver.stop();
    }
    else
    {
        printf("Cloudisk Server Start Failed!\n");
    }
}

void CloudiskServer::loadModules()
{
    loadStaticResourceModule();
    loadUserRegisterModule();
    loadUserLoginModule();
    loadUserInfoModule();
    loadFileQueryModule();
    // loadFileUploadModule();
    // loadFileDownloadModule();
}

void CloudiskServer::loadStaticResourceModule()
{
    _httpserver.GET("/user/signup", [](const HttpReq*, HttpResp* resp)
        { resp->File("static/view/signup.html"); });

    _httpserver.GET("/static/view/signin.html", [](const HttpReq*, HttpResp* resp)
        { resp->File("static/view/signin.html"); });

    _httpserver.GET("/static/view/home.html", [](const HttpReq*, HttpResp* resp)
        { resp->File("static/view/home.html"); });

    _httpserver.GET("/static/js/auth.js", [](const HttpReq*, HttpResp* resp)
        { resp->File("static/js/auth.js"); });

    _httpserver.GET("/static/img/avatar.jpeg", [](const HttpReq*, HttpResp* resp)
        { resp->File("static/img/avatar.jpeg"); });

    _httpserver.GET("/file/upload", [](const HttpReq*, HttpResp* resp)
        { resp->File("static/view/index.html"); });
    _httpserver.Static("/file/upload_files", "static/view/upload_files");
}

void CloudiskServer::loadUserRegisterModule()
{
    _httpserver.POST("/user/signup", std::bind(&CloudiskServer::loadUserRegisterCallback, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void CloudiskServer::loadUserLoginModule()
{
    _httpserver.POST("/user/signin", std::bind(&CloudiskServer::loadUserLoginCallback, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void CloudiskServer::loadUserInfoModule()
{
    _httpserver.GET("/user/info", std::bind(&CloudiskServer::loadUserInfoCallback, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void CloudiskServer::loadFileQueryModule()
{
    _httpserver.POST("/file/query", std::bind(&CloudiskServer::loadFileQueryCallback, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void CloudiskServer::loadFileUploadModule()
{
    _httpserver.POST("/file/upload", std::bind(&CloudiskServer::loadFileUploadCallback, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void CloudiskServer::loadFileDownloadModule()
{
    _httpserver.GET("/file/downloadurl", std::bind(&CloudiskServer::loadFileDownloadCallback, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

// 注册的回调函数
void CloudiskServer::loadUserRegisterCallback(const HttpReq* req, HttpResp* resp, SeriesWork* series)
{
    if (req->content_type() == APPLICATION_URLENCODED)
    {
        // 1. 解析请求, 获取用户名和密码
        auto formKV = req->form_kv();
        string username = formKV["username"];
        string password = formKV["password"];
        cout << "username:" << username << endl;
        cout << "password:" << password << endl;
        //2. 从注册中心找到微服务端所对应的ip:port
        // 向consul发送http请求获得一个json
        string consul_url = "http://192.168.88.150:8500/v1/agent/services";
        auto* httpTask = WFTaskFactory::create_http_task(consul_url, 10, 0,
            [=](WFHttpTask* httpTask)
            {
                int state = httpTask->get_state();
                int error = httpTask->get_error();
                if (state == WFT_STATE_SUCCESS)
                {
                    protocol::HttpResponse* httpresp = httpTask->get_resp();
                    const void* body;
                    size_t body_len;
                    httpresp->get_parsed_body(&body, &body_len);
                    string resp_body(static_cast<const char*>(body), body_len);
                    //解析json
                    using Json = nlohmann::json;
                    Json json = Json::parse(resp_body);
                    string address = json["SignupService1"]["Address"];
                    unsigned short port = json["SignupService1"]["Port"];
                    cout << "address: " << address << "\n";
                    cout << "port: " << port << "\n";
                    //3. client对象调用rpc
                    UserService::SRPCClient client(address.c_str(), port);
                    ReqSignup signup_req;
                    signup_req.set_username(username);
                    signup_req.set_password(password);

                    // 创建一个 promise 和 future 来等待 RPC 调用完成
                    auto promise = std::make_shared<std::promise<std::string>>();//用于rpc异步完成时在回调函数中存储结果
                    auto future = promise->get_future().share();//创建一个与promise关联的future，这个future会等待promise的值

                    client.Signup(&signup_req,
                        [promise, series](RespSignup* response, RPCContext* context)
                        {
                            // 获得rpc响应回复给客户端
                            if (context->success()) {
                                // RPC 调用成功，返回响应
                                string message = response->message();
                                cout << "message = " << message << "\n";
                                promise->set_value(message);//会将结果传递给future
                            }
                            else {
                                // RPC 调用失败，处理错误
                                cout << "RPC error: " << context->get_error() << endl;
                                promise->set_value("RPC failed.");
                            }
                        }
                    );

                    // 创建一个 Go 任务来等待 RPC 调用完成
                    //虽然这里是同步的，但是因为这是所有任务执行完毕的最后一步，因此不会阻塞之前的异步任务
                    auto* go_task = WFTaskFactory::create_go_task(
                        "wait_rpc",
                        [future, resp]() {
                            std::string result = future.get();//这里会阻塞等待rpc的异步回调中将值放入promise
                            resp->String(result);
                        });

                    series->push_back(go_task);
                }
                else
                {
                    // 处理请求失败的情况
                    cout << "HTTP request failed. State: " << state << " Error: " << error << endl;
                    resp->String("Failed to contact Consul.");
                }
            }
        );
        series->push_back(httpTask);

    }
}

// 登录的回调函数
void CloudiskServer::loadUserLoginCallback(const HttpReq* req, HttpResp* resp, SeriesWork* series)
{
    if (req->content_type() == APPLICATION_URLENCODED)
    {
        // 1. 解析请求, 获取用户名和密码
        auto formKV = req->form_kv();
        string username = formKV["username"];
        string password = formKV["password"];
        cout << "username:" << username << endl;
        cout << "password:" << password << endl;
        // 2. 对密码进行加密
        string salt("12345678"); // 这里应该是随机生成
        string encodedPassword(crypt(password.c_str(), salt.c_str()));
        cout << "mi wen:" << encodedPassword << endl;

        string consul_url = "http://192.168.88.150:8500/v1/agent/services";
        auto* httpTask = WFTaskFactory::create_http_task(consul_url, 10, 0,
            [=](WFHttpTask* httpTask)
            {
                int state = httpTask->get_state();
                int error = httpTask->get_error();
                if (state == WFT_STATE_SUCCESS)
                {
                    protocol::HttpResponse* httpresp = httpTask->get_resp();
                    const void* body;
                    size_t body_len;
                    httpresp->get_parsed_body(&body, &body_len);
                    string resp_body(static_cast<const char*>(body), body_len);
                    //解析json
                    using Json = nlohmann::json;
                    Json json = Json::parse(resp_body);
                    string address = json["SigninService1"]["Address"];
                    unsigned short port = json["SigninService1"]["Port"];
                    cout << "address: " << address << "\n";
                    cout << "port: " << port << "\n";
                    //3. client对象调用rpc
                    UserSigninService::SRPCClient client(address.c_str(), port);
                    ReqSignin signin_req;
                    signin_req.set_username(username);
                    signin_req.set_encodedpassword(encodedPassword);

                    auto promise = std::make_shared<std::promise<string>>();
                    auto future = promise->get_future().share();

                    client.Signin(&signin_req,
                        [promise](RespSignin* response, RPCContext* context)
                        {
                            // 获得rpc响应回复给客户端
                            if (context->success()) {
                                // RPC 调用成功，返回响应
                                string message = response->message();
                                cout << message << "\n";
                                promise->set_value(message);
                            }
                            else {
                                // RPC 调用失败，处理错误
                                cout << "RPC error: " << context->get_error() << endl;
                                promise->set_value("RPC failed.");
                            }
                        }
                    );

                    auto waitTask = WFTaskFactory::create_go_task(
                        "wait_rpc",
                        [future, resp]() {
                            resp->String(future.get());
                        }
                    );
                    series->push_back(waitTask);
                }
                else
                {
                    // 处理请求失败的情况
                    cout << "HTTP request failed. State: " << state << " Error: " << error << endl;
                    resp->String("Failed to contact Consul.");
                }
            }
        );
        series->push_back(httpTask);
    }
}

void CloudiskServer::loadUserInfoCallback(const HttpReq* req, HttpResp* resp, SeriesWork* series)
{
    // 1. 解析请求
    string username = req->query("username");
    string tokenStr = req->query("token");
    cout << "username:" << username << endl;
    cout << "token:" << tokenStr << endl;
    // 2. 登录之后做业务之前必须进行token校验
    //    通过计算（消耗cpu）的方式来验证
    string salt("12345678");
    Token token(username, salt);
    cout << "token.genToken() = " << token.genToken() << "\n";
    if (tokenStr != token.genToken())
    {
        resp->String("Invalid token in User Info!");
        return;
    }
    // 3. 执行信号获取的操作
    string mysqlurl("mysql://root:112358@localhost");
    auto mysqlTask = WFTaskFactory::create_mysql_task(mysqlurl, 1,
        [=](WFMySQLTask* mysqltask)
        {
            //...检测
            using namespace protocol;
            auto mysqlResp = mysqltask->get_resp();
            MySQLResultCursor cursor(mysqlResp);
            if (cursor.get_cursor_status() == MYSQL_STATUS_GET_RESULT)
            {
                // 读操作,获取用户的注册时间
                vector<vector<MySQLCell>> matrix;
                cursor.fetch_all(matrix);
                string signupAt = matrix[0][0].as_string();
                using Json = nlohmann::json;
                Json msg;
                Json data;
                data["Username"] = username;
                data["SignupAt"] = signupAt;
                msg["data"] = data;
                resp->String(msg.dump());
            }
            else
            {
                // 没有读取到正确的信息
                resp->String("error");
            }
        });
    string sql("select signup_at from cloudisk.tbl_user where user_name = '");
    sql += username + "'";
    mysqlTask->get_req()->set_query(sql);
    series->push_back(mysqlTask);
}

void CloudiskServer::loadFileQueryCallback(const HttpReq* req, HttpResp* resp, SeriesWork* series)
{
    // 1. 解析请求: 查询词
    string username = req->query("username");
    string tokenStr = req->query("token");
    cout << "username:" << username << endl;
    cout << "token:" << tokenStr << endl;
    string salt("12345678");
    Token token(username, salt);
    cout << "token.genToken() = " << token.genToken() << "\n";
    if (tokenStr != token.genToken())
    {
        resp->String("Invalid token in file query!");
        return;
    }

    // 2. 解析请求： 消息体
    string limitCnt = req->form_kv()["limit"]; // 查询的限制条数

    string mysqlurl("mysql://root:112358@localhost");
    auto mysqlTask = WFTaskFactory::create_mysql_task(mysqlurl, 1,
        [=](WFMySQLTask* mysqltask)
        {
            //...检测
            using namespace protocol;
            auto mysqlResp = mysqltask->get_resp();
            MySQLResultCursor cursor(mysqlResp);
            if (cursor.get_cursor_status() == MYSQL_STATUS_GET_RESULT)
            {
                // 读操作,获取用户的
                vector<vector<MySQLCell>> matrix;
                cursor.fetch_all(matrix);
                using Json = nlohmann::json;
                Json msgArr;
                for (size_t i = 0; i < matrix.size(); ++i)
                {
                    Json row;
                    row["FileHash"] = matrix[i][0].as_string();
                    row["FileName"] = matrix[i][1].as_string();
                    row["FileSize"] = matrix[i][2].as_ulonglong();
                    row["UploadAt"] = matrix[i][3].as_datetime();
                    row["LastUpdated"] = matrix[i][4].as_datetime();
                    msgArr.push_back(row); // 在数组中添加一个元素,使用push_back即可
                }
                resp->String(msgArr.dump());
            }
            else
            {
                // 没有读取到正确的信息
                resp->String("error");
            }
        });
    string sql("select file_sha1, file_name, file_size, upload_at, last_update from cloudisk.tbl_user_file where user_name = '");
    sql += username + "' limit " + limitCnt;
    cout << "\nsql:\n"
        << sql << endl;
    mysqlTask->get_req()->set_query(sql);
    series->push_back(mysqlTask);
}

void CloudiskServer::loadFileUploadCallback(const HttpReq* req, HttpResp* resp, SeriesWork* series)
{
    // 1. 解析请求
    string username = req->query("username");
    string tokenStr = req->query("token");
    cout << "username:" << username << endl;
    cout << "token in upload:" << tokenStr << endl;
    // 2. 对token进行验证
    string salt("12345678");
    Token token(username, salt);
    cout << "token.genToken() = " << token.genToken() << "\n";
    if (tokenStr != token.genToken())
    {
        resp->String("Invalid token in file upload!");
        return;
    }
    // 3. 解析请求：消息体
    if (req->content_type() == MULTIPART_FORM_DATA)
    {
        auto form = req->form();
        string filename = form["file"].first;
        string content = form["file"].second;
        cout << "filename : " << filename << endl;
        // 4. 将数据写入服务器本地(这个地方应该用IOTask)
        mkdir("tmp", 0755);
        string filepath = "tmp/" + filename;
        int fd = open(filepath.c_str(), O_CREAT | O_RDWR, 0664);
        if (fd < 0)
        {
            cout << "test" << endl;
            perror("open");
            return;
        }
        WFFileIOTask* pwriteTask = WFTaskFactory::create_pwrite_task(fd, content.c_str(), content.size(), 0,
            [fd, resp](WFFileIOTask* pwriteTask)
            {
                if (pwriteTask->get_state() == WFT_STATE_SUCCESS)
                {
                    close(fd);
                    resp->String("Upload Success");
                }
                else
                {
                    close(fd);
                    resp->String("Upload Failed");
                }
            });
        series->push_back(pwriteTask);

        // 用消息队列rabbitmq生产一个json消息（用于备份）(生产者)
        using namespace AmqpClient;
        Channel::ptr_t channel = Channel::Create();
        BasicMessage::ptr_t message = BasicMessage::Create(filepath);
        channel->BasicPublish(_mqinfo.Exchange, _mqinfo.RoutingKey, message);

        // 5. 生成SHA1值
        Hash hash(filepath);
        string filehash = hash.sha1();
        cout << "filehash:" << filehash << endl;
        // 6. 将文件相关信息写入数据库MySQL中
        string mysqlurl("mysql://root:112358@localhost");
        auto mysqlTask = WFTaskFactory::create_mysql_task(mysqlurl, 1, nullptr);
        string sql("INSERT INTO cloudisk.tbl_user_file(user_name,file_sha1,file_size,file_name)VALUES('");
        sql += username + "','" + filehash + "', " + std::to_string(content.size()) + ",'" + filename + "')";
        cout << "\nsql:\n"
            << sql << endl;
        mysqlTask->get_req()->set_query(sql);
        series->push_back(mysqlTask);
    }
}

void CloudiskServer::loadFileDownloadCallback(const HttpReq* req, HttpResp* resp, SeriesWork* series)
{
    // 1. 解析请求
    string username = req->query("username");
    string tokenStr = req->query("token");
    cout << "username:" << username << endl;
    cout << "token:" << tokenStr << endl;
    // 2. 对token进行验证
    string salt("12345678");
    Token token(username, salt);
    if (tokenStr != token.genToken())
    {
        resp->String("Invalid token in file download!");
        return;
    }

    string filename = req->query("filename");
    cout << "filename: " << filename << endl;

    // 将下载业务从服务器中分离出去，之后只需要产生一个下载链接就可以了
    // 这要求我们还需要去部署一个下载服务器
    string downloadURL = "http://192.168.88.150:8080/" + filename;
    resp->String(downloadURL);
}
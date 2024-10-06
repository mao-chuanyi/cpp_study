#include "signin.srpc.h"
#include "../Token.h"
#include "workflow/WFFacilities.h"
#include <ppconsul/ppconsul.h>
#include <iostream>
#include <string>
#include <workflow/MySQLMessage.h>
#include <workflow/MySQLResult.h>
#include <workflow/MySQLUtil.h>
#include <vector>
#include <nlohmann/json.hpp>

using namespace srpc;
using namespace ppconsul;
using std::cout;
using std::string;
using std::vector;

static WFFacilities::WaitGroup wait_group(1);

void sig_handler(int signo)
{
	wait_group.done();
}

class UserSigninServiceServiceImpl : public UserSigninService::Service
{
public:

	void Signin(ReqSignin* request, RespSignin* response, srpc::RPCContext* ctx) override
	{
		string username = request->username();
		string encodedPassword = request->encodedpassword();

		// 3. 从数据库MySQL中读取用户信息进行登录验证
		string mysqlurl("mysql://root:112358@localhost");

		auto mysqlTask = WFTaskFactory::create_mysql_task(mysqlurl, 1,
			[=](WFMySQLTask* mysqltask)
			{
				// 1. 检查任务状态
				int state = mysqltask->get_state();
				int error = mysqltask->get_error();
				if (state != WFT_STATE_SUCCESS)
				{
					printf("Task state error: %s\n", WFGlobal::get_error_string(state, error));
					response->set_code(1);
					response->set_message("Login Failed: Database connection error");
					return;
				}

				// 2. 获取MySQL响应
				auto mysqlResp = mysqltask->get_resp();

				// 3. 检查MySQL数据包类型
				if (mysqlResp->get_packet_type() == MYSQL_PACKET_ERROR)
				{
					printf("MySQL error %d: %s\n", mysqlResp->get_error_code(),
						mysqlResp->get_error_msg().c_str());
					response->set_code(1);
					response->set_message("Login Failed: Database query error");
					return;
				}

				// 4. 使用结果游标
				using namespace protocol;
				MySQLResultCursor cursor(mysqlResp);

				// 5. 检查游标状态
				if (cursor.get_cursor_status() != MYSQL_STATUS_GET_RESULT)
				{
					printf("Cursor status error: %d\n", cursor.get_cursor_status());
					response->set_code(1);
					response->set_message("Login Failed: Invalid query result");
					return;
				}

				// 6. 检查结果集是否为空
				vector<vector<MySQLCell>> matrix;
				cursor.fetch_all(matrix);
				if (matrix.empty() || matrix[0].empty())
				{
					printf("No user found with username: %s\n", username.c_str());
					response->set_code(1);
					response->set_message("Login Failed: User not found");
					return;
				}

				// 7. 获取密码并比较
				string storedPassword = matrix[0][0].as_string();
				if (storedPassword.empty())
				{
					printf("Retrieved password is empty for user: %s\n", username.c_str());
					response->set_code(1);
					response->set_message("Login Failed: Invalid user data");
					return;
				}

				if (encodedPassword == storedPassword)
				{
					// 登录成功，生成Token
					Token token(username, "12345678");
					string tokenStr = token.genToken();

					// 创建Redis任务
					WFRedisTask* redisTask = WFTaskFactory::create_redis_task("redis://127.0.0.1:6379", 0,
						[=](WFRedisTask* task) {
							// Redis任务的回调函数
							if (task->get_state() != WFT_STATE_SUCCESS)
							{
								printf("Redis task failed. Error: %d\n", task->get_error());
								response->set_code(1);
								response->set_message("Login Failed: Unable to store session");
								return;
							}

							// protocol::RedisReply *reply = task->get_resp();
							// if (reply->type() == REDIS_REPLY_ERROR)
							// {
							//     printf("Redis error: %s\n", reply->as_string().c_str());
							//     response->set_code(1);
							//     response->set_message("Login Failed: Session storage error");
							//     return;
							// }

							// 成功存储Token后的响应
							response->set_code(0);
							using Json = nlohmann::json;
							Json msg;
							Json data;
							data["Token"] = tokenStr;
							data["Username"] = username;
							data["Location"] = "/static/view/home.html";
							msg["data"] = data;
							response->set_message(msg.dump());
						}
					);

					redisTask->get_req()->set_request("SETEX", { username, "3600", tokenStr });
					ctx->get_series()->push_back(redisTask);
				}
				else
				{
					// 密码不匹配
					printf("Password mismatch for user: %s\n", username.c_str());
					response->set_code(1);
					response->set_message("Login Failed: Incorrect password");
				}
			});
		string sql = ("select user_pwd from cloudisk.tbl_user where user_name = '");
		sql += username + "' limit 1";
		cout << "sql:\n"
			<< sql << "\n";
		mysqlTask->get_req()->set_query(sql);
		ctx->get_series()->push_back(mysqlTask);
	}
};

void timerCallback(WFTimerTask* timerTask)
{
	cout << "now= " << time(nullptr) << "\n";
	agent::Agent* pagent = static_cast<agent::Agent*>(timerTask->user_data);
	//使用定时任务发送存活信息
	pagent->servicePass("SigninService1");
	WFTimerTask* nextTask = WFTaskFactory::create_timer_task(7, 0, timerCallback);
	nextTask->user_data = pagent;
	series_of(timerTask)->push_back(nextTask);
}

int main()
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	unsigned short port = 1413;
	SRPCServer server;

	UserSigninServiceServiceImpl usersigninservice_impl;
	server.add_service(&usersigninservice_impl);

	//注册到注册中心consul
	Consul consul("127.0.0.1:8500", ppconsul::kw::dc = "dc1");
	agent::Agent agent(consul);
	//注册一个服务
	agent.registerService(
		agent::kw::name = "SigninService1",
		agent::kw::address = "127.0.0.1",
		agent::kw::id = "SigninService1",
		agent::kw::port = 1413,
		agent::kw::check = agent::TtlCheck(std::chrono::seconds(10))//10s内发送心跳表示存活
	);
	WFTimerTask* timerTask = WFTaskFactory::create_timer_task(7, 0, timerCallback);
	timerTask->user_data = &agent;
	timerTask->start();

	server.start(port);
	wait_group.wait();
	server.stop();
	google::protobuf::ShutdownProtobufLibrary();
	return 0;
}

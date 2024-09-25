#include "rpc.srpc.h"
#include "workflow/WFFacilities.h"
#include <workflow/MySQLMessage.h>
#include <workflow/MySQLResult.h>
#include <workflow/MySQLUtil.h>
#include <ppconsul/ppconsul.h>
#include <string>
#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;
using std::string;
using namespace ppconsul;

using namespace srpc;

static WFFacilities::WaitGroup wait_group(1);

void sig_handler(int signo)
{
	wait_group.done();
}

class UserServiceServiceImpl : public UserService::Service
{
public:

	void Signup(ReqSignup *request, RespSignup *response, srpc::RPCContext *ctx) override
	{
		string username = request->username();
		string password = request->password();
		//2. 对密码进行加密
        string salt("12345678");//这里应该是随机生成
        string encodedPassword(crypt(password.c_str(), salt.c_str()));
        cout << "mi wen:" << encodedPassword << endl;
        //将用户信息存储到数据库MySQL中
        string mysqlurl("mysql://root:112358@localhost");
        auto mysqlTask = WFTaskFactory::create_mysql_task(mysqlurl, 1, 
        [response](WFMySQLTask * mysqltask){
            //0. 对任务的状态进行检测
            int state = mysqltask->get_state();
            int error = mysqltask->get_error();
            if(state != WFT_STATE_SUCCESS) {
                printf("%s\n", WFGlobal::get_error_string(state, error));
				response->set_code(1);
				response->set_message("net failed");
                return;
            }
            //1. 检测SQL语句是否存在语法错误
            auto mysqlResp = mysqltask->get_resp();
            if(mysqlResp->get_packet_type() == MYSQL_PACKET_ERROR) {
                printf("ERROR %d: %s\n", mysqlResp->get_error_code(),
                       mysqlResp->get_error_msg().c_str());
				response->set_code(1);
				response->set_message("mysql failed");
				return;
			}
            using namespace protocol;
            MySQLResultCursor cursor(mysqlResp);
            if(cursor.get_cursor_status() == MYSQL_STATUS_OK) {
                //2. 成功写入数据库了
                printf("Query OK. %llu row affected.\n",cursor.get_affected_rows());
				response->set_code(0);
				response->set_message("Signup success");
			}
        });
        string sql("INSERT INTO cloudisk.tbl_user(user_name, user_pwd) VALUES('");
        sql += username + "', '" + encodedPassword + "')";
        cout << "sql:\n" << sql << endl;
        mysqlTask->get_req()->set_query(sql);
		ctx->get_series()->push_back(mysqlTask);
	}
};

void timerCallback(WFTimerTask *timerTask)
{
    cout << "now= " << time(nullptr) << endl;
    agent::Agent *pagent = static_cast<agent::Agent*>(timerTask->user_data);
    //使用定时任务发送存活信息
    pagent->servicePass("SignupService1");
    WFTimerTask *nextTask = WFTaskFactory::create_timer_task(7, 0, timerCallback);
    nextTask->user_data = pagent;
    series_of(timerTask)->push_back(nextTask);
}

int main()
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	unsigned short port = 1412;//端口
	SRPCServer server;//底层用workflow实现的服务端

	UserServiceServiceImpl userservice_impl;//rpc被调函数的实现类
	server.add_service(&userservice_impl);//注册

    //注册到注册中心consul
    Consul consul("127.0.0.1:8500", ppconsul::kw::dc = "dc1");
    agent::Agent agent(consul);
    //注册一个服务
    agent.registerService(
        agent::kw::name = "SignupService1",
        agent::kw::address = "127.0.0.1",
        agent::kw::id = "SignupService1",
        agent::kw::port = 1412,
        agent::kw::check = agent::TtlCheck(std::chrono::seconds(10))//发给服务端的信息存活维持10s
    );

    WFTimerTask *timerTask = WFTaskFactory::create_timer_task(7, 0, timerCallback);
    timerTask->user_data = &agent;
    timerTask->start();

	server.start(port);
	wait_group.wait();
	server.stop();
	google::protobuf::ShutdownProtobufLibrary();
	return 0;
}

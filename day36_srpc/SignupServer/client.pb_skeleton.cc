#include "rpc.srpc.h"
#include "workflow/WFFacilities.h"
#include <iostream>

using std::cout;
using std::endl;
using namespace srpc;

static WFFacilities::WaitGroup wait_group(1);

void sig_handler(int signo)
{
	wait_group.done();
}

static void signup_done(RespSignup *response, srpc::RPCContext *context)
{
	cout << "code = " << response->code() << endl;
	cout << "message =" << response->message() << endl;
}

int main()
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	//从注册中心获得rpc的ip和端口号

	UserService::SRPCClient client(ip, port);//client_stub服务端留在客户端的存根

	// example for RPC method call
	ReqSignup signup_req;
	signup_req.set_username("admin");
	signup_req.set_password("123");
	client.Signup(&signup_req, signup_done);

	wait_group.wait();
	google::protobuf::ShutdownProtobufLibrary();
	return 0;
}

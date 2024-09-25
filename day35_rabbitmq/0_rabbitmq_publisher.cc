#include <iostream>
#include <string>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

using std::cout;
using std::string;

struct MQInfo
{
    string Exchange = "Exchange_test";
    string RoutingKey = "key1";
};

//生产者
int main()
{
    using namespace AmqpClient;
    MQInfo mqinfo;
    // 为channel申请内存
    Channel::ptr_t channel = Channel::Create();
    //创建一个消息
    BasicMessage::ptr_t message = BasicMessage::Create("howareyou");
    //发送消息
    channel->BasicPublish(mqinfo.Exchange, mqinfo.RoutingKey,message);
    return 0;
}
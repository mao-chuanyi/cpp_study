#include <iostream>
#include <string>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

using std::cout;
using std::string;

struct MQInfo
{
    string Exchange = "Exchange_test";
    string RoutingKey = "key1";
    string Queue = "queue1";
};

//消费者
int main()
{
    using namespace AmqpClient;
    MQInfo mqinfo;
    // 为channel申请内存
    Channel::ptr_t channel = Channel::Create();
    //调用basicconsume
    channel->BasicConsume(mqinfo.Queue);
    Envelope::ptr_t envelope;
    while(true)
    {
        if(channel->BasicConsumeMessage(envelope, -1))
        {
            cout << "message: " << envelope->Message()->Body() << "\n"; 
        }
        else
        {
            cout << "timeout\n"
                 << "\n";
        }
    }
    return 0;
}
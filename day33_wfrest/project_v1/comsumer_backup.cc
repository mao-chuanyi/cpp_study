#include <iostream>
#include <string>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <alibabacloud/oss/OssClient.h>

using std::cout;
using std::string;
using namespace AlibabaCloud::OSS;
using std::cerr;

struct OSSInfo
{
    string EndPoint = "oss-cn-chengdu.aliyuncs.com";
    string AccessKeyID = "";
    string AccessKeySecret = "";
    string Bucket = "mao-9527";
};

struct MQInfo
{
    string Exchange = "Exchange_test";
    string RoutingKey = "key1";
    string Queue = "queue1";
};

//消费者
int main()
{
    OSSInfo ossinfo;
    InitializeSdk();

    //配置客户端属性（技术：依赖注入，将依赖和实例分开）
    ClientConfiguration conf;
    conf.maxConnections = 30;
    conf.requestTimeoutMs = 3000;
    conf.connectTimeoutMs = 3000;

    OssClient client(ossinfo.EndPoint, ossinfo.AccessKeyID, ossinfo.AccessKeySecret, conf);

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
            string filepath = envelope->Message()->Body();
            cout << "filepath: " << filepath << "\n";
            PutObjectOutcome outcome = client.PutObject(ossinfo.Bucket, filepath, filepath);
            if(!outcome.isSuccess())
            {
                cerr << "code = " << outcome.error().Code()
                     << ", message = " << outcome.error().Message() << "\n";
            }
        }
        else
        {
            cout << "timeout\n"
                 << "\n";
        }
    }

    //释放网络连接
    ShutdownSdk();
    return 0;
}
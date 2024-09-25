#include <alibabacloud/oss/OssClient.h>
#include <iostream>

using namespace AlibabaCloud::OSS;
using std::cerr;
using std::cout;
using std::string;
struct OSSInfo
{
    string EndPoint = "oss-cn-chengdu.aliyuncs.com";
    string AccessKeyID = "";
    string AccessKeySecret = "";
    string Bucket = "mao-9527";
};

int main()
{
    OSSInfo info;
    //初始化网络资源
    InitializeSdk();

    //配置客户端属性（技术：依赖注入，将依赖和实例分开）
    ClientConfiguration conf;
    conf.maxConnections = 30;
    conf.requestTimeoutMs = 3000;
    conf.connectTimeoutMs = 3000;

    //创建客户端
    OssClient client(info.EndPoint, info.AccessKeyID, info.AccessKeySecret, conf);

    //上传操作
    // PutObjectOutcome outcome = client.PutObject(info.Bucket, "dir1/file1.txt", "local.txt");
    // if(!outcome.isSuccess())
    // {
    //     cerr << "code = " << outcome.error().Code()
    //          << ", message = " << outcome.error().Message() << "\n";
    // }

    //下载操作
    time_t expire = time(nullptr) + 600;//设置时效10分钟
    StringOutcome outcome = client.GeneratePresignedUrl(info.Bucket, "dir1/file1.txt", expire, Http::Get);
    if(outcome.isSuccess())
    {
        cout << outcome.result() << "\n";//成功返回下载链接
    }
    else
    {
        cerr << "error\n";
    }
    //释放网络连接
    ShutdownSdk();

    return 0;
}
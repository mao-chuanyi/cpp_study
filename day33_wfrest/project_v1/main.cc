#include "CloudiskServer.h"
#include <alibabacloud/oss/OssClient.h>
#include <iostream>

using namespace AlibabaCloud::OSS;

int main()
{
    //配置客户端属性（技术：依赖注入，将依赖和实例分开）
    ClientConfiguration conf;
    conf.maxConnections = 30;
    conf.requestTimeoutMs = 3000;
    conf.connectTimeoutMs = 3000;
    CloudiskServer cloudiskServer(1, conf);
    cloudiskServer.loadModules();
    cloudiskServer.start(1234);
    return 0;
}


#include "CloudiskServer.h"
#include <iostream>


int main()
{
    //配置客户端属性（技术：依赖注入，将依赖和实例分开）
    CloudiskServer cloudiskServer(1);
    cloudiskServer.loadModules();
    cloudiskServer.start(1234);
    return 0;
}


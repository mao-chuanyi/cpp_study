#include <ppconsul/ppconsul.h>
#include <workflow/WFFacilities.h>
#include <iostream>
#include <signal.h>
using std::cout;
using std::endl;
using namespace ppconsul;

static WFFacilities::WaitGroup WaitGroup(1);

void handler(int signum)
{
    WaitGroup.done();
}

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

//注册
int main(){
    signal(SIGINT, handler);
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
    WaitGroup.wait();
    return 0;
}


#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/PatternLayout.hh"
#include "log4cpp/RollingFileAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/LayoutAppender.hh"
#include "log4cpp/Priority.hh"
#include <iostream>
using namespace log4cpp;
using namespace std;

int main(){
    //设计个性化的布局PatternLayout
    //1.设置日志布局
    PatternLayout * ptn1 = new PatternLayout();
    ptn1->setConversionPattern("%d %c [%p] %m%n");
    PatternLayout * ptn2 = new PatternLayout();
    ptn2->setConversionPattern("%d %c [%p] %m%n");

    //2.创建输出器对象
    OstreamAppender *pos = new OstreamAppender("console", &cout);
    auto rollpos = new RollingFileAppender("rollingfile", "rollingfile.log", 5*1024, 3);
    //一个输出器绑定一个布局（一对一），否则段错误
    pos->setLayout(ptn1);
    rollpos->setLayout(ptn2);

    //3.创建日志记录器
    Category & salesDepart = Category::getInstance("salesDepart");

    //4.设置优先级
    //REEOR优先级以上的都有包括ERROR
    salesDepart.setPriority(Priority::INFO);

    //5.设置输出器
    salesDepart.addAppender(pos);
    salesDepart.addAppender(rollpos);

    //6.记录日志
    salesDepart.emerg("this is a emerg msg");
    salesDepart.error("this is a error msg");
    salesDepart.info("this is a info msg");
    salesDepart.warn("this is a warn msg");

    //7.日志系统退出时回收资源
    Category::shutdown();

    return 0;
}


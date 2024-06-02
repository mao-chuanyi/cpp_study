#include "log4cpp/Category.hh"
#include "log4cpp/PatternLayout.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/RollingFileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Priority.hh"
#include <iostream>
using namespace std;
using namespace log4cpp;
void test(){
    //设置布局
    PatternLayout * ptn1 =new PatternLayout();
    ptn1->setConversionPattern("%d %c [%p] %m%n");
    PatternLayout * ptn2 =new PatternLayout();
    ptn2->setConversionPattern("%d %c [%p] %m%n");
    PatternLayout * ptn3 =new PatternLayout();
    ptn3->setConversionPattern("%d %c [%p] %m%n");

    //创建输出器
    //终端
    OstreamAppender * conApp = new OstreamAppender("console", &cout);
    //绑定布局
    conApp->setLayout(ptn1);
    //文件
    FileAppender *fileApp = new FileAppender("file", "file.log");
    fileApp->setLayout(ptn2);
    //回卷文件
    RollingFileAppender *rollApp = new RollingFileAppender("rollFile", "rillingFile.log", 5*1024, 9);
    rollApp->setLayout(ptn3);

    //创建日志记录器（主要)
    Category & sub = Category::getInstance("sub");
    
    //设置优先级
    sub.setPriority(Priority::Value);

    //设置输出器
    sub.addAppender(conApp);
    sub.addAppender(fileApp);
    sub.addAppender(rollApp);
}

int main(){
    test();
    return 0;
}


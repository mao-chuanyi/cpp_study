#include <iostream>
#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"
using namespace log4cpp;

int main(int argc, char** argv) {
    //Appender代表输出器（目的地）
    //基类指针(Appender)指向派生类(OstreamAppender)
	Appender *appender1 = new OstreamAppender("console", &std::cout);
    //输出器绑定布局器（格式化器）
	appender1->setLayout(new  BasicLayout());

    Appender *appender2 = new  FileAppender("default", "program.log");
	appender2->setLayout(new  BasicLayout());

    //日志记录器
	Category& root =  Category::getRoot();
    //设置root的优先级（过滤器）
	root.setPriority( Priority::WARN);
    //设置root的目的地
	root.addAppender(appender1);

	Category& sub1 =  Category::getInstance(std::string("sub1"));
	sub1.addAppender(appender2);

	// use of functions for logging messages
	root.error("root error");
	root.info("root info");
	sub1.error("sub1 error");
	sub1.warn("sub1 warn");

	// printf-style for logging variables
	root.warn("%d + %d == %s ?", 1, 1, "two");

	// use of streams for logging messages
	root <<  Priority::ERROR << "Streamed root error";
	root <<  Priority::INFO << "Streamed root info";
	sub1 <<  Priority::ERROR << "Streamed sub1 error";
	sub1 <<  Priority::WARN << "Streamed sub1 warn";

	// or this way:
	root.errorStream() << "Another streamed error";

	return 0;
}

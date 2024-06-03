#include "log4cpp/Category.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/PatternLayout.hh"
#include <iostream>
using namespace std;
using namespace log4cpp;

class Mylogger
{
public:
    static Mylogger * getInstance(){
        if(_log == nullptr)
        {
            _log = new Mylogger();
        }
        return _log;

    }
    static void destroy(){
        if(_log)
        {
            delete _log;
            _log = nullptr;
        }
    }
    void warn(const char *msg){
        _sub.warn(msg);
    }
    void error(const char *msg){
        _sub.error(msg);
    }
    void debug(const char *msg){
        _sub.debug(msg);
    }
    void info(const char *msg){
        _sub.info(msg);
    }
    void fatal(const char *msg){
        _sub.fatal(msg);
    }
    void crit(const char *msg){
        _sub.crit(msg);
    }
	
private:
    Mylogger():
        _sub(Category::getInstance("sub"))
    {
        PatternLayout * ptn1 = new PatternLayout();
        ptn1->setConversionPattern("%d %c [%p] %m%n");
        OstreamAppender * oApp = new OstreamAppender("console", &cout);
        oApp->setLayout(ptn1);
        _sub.setPriority(Priority::INFO);
        _sub.addAppender(oApp);
    }
    ~Mylogger(){
        Category::shutdown();
    }
    
private:
    Category & _sub;
    static Mylogger *_log;
};

Mylogger* Mylogger::_log = nullptr;

void test(){
    Mylogger *log = Mylogger::getInstance();

    log->info("The log is info essage");
    log->error("The log is error message");
    log->fatal("The log is fatal message");
    log->crit("The log is crit message");
    Mylogger::destroy();
}

int main(){
    test();
    return 0;
}


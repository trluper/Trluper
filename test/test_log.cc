#include "log.h"

int main(){
    Trluper::LoggerManager* lm= LOG_GET_MANAGER;
    Trluper::Logger::ptr mainLogger = lm->getMainLogger();
    Trluper::Logger::ptr system = lm->getLogger("system");
    std::string path = "../Log/log.txt";
    //mainLogger->setFormatter("[%d]-[%p]-%f: %m%n");
    Trluper::LogAppender::ptr p(new Trluper::FileLogAppender(path));
    p->setLogLevel(Trluper::LogLevel::ERROR);
    mainLogger->addAppender(p); //logger的level是DEBUF，但FileLogAppende为ERROR
    if(!system->hashAppender()){
        system->addAppender(p);
    }
    LOG_SS_DEBUG(mainLogger)<<"Hello Trluper log";
    LOG_SS_ERROR(mainLogger)<<"ERROR IN LINE 61";
    LOG_FMT_DEBUG(mainLogger,"This is %s, Line:%d","Trluper log",61);
    LOG_FMT_ERROR(system,"system test %s","18");
    LOG_SS_DEBUG(mainLogger)<<"\n"<<lm->toYAMLString();
    return 0;
}
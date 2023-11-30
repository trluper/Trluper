/*
 * @Author: wuwenjie wenjiewu2017@163.com
 * @Date: 2023-09-05 20:31:44
 * @LastEditors: wuwenjie wenjiewu2017@163.com
 * @LastEditTime: 2023-09-14 16:51:04
 * @FilePath: /Trluper/TEST/main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include "factory.h"
#include "timer.h"
#include "log.h"
#include "thread.h"



#ifdef _TIMER_TEST
void timerTest(void* arg){

   std::cout<<"Timer Test,当前时间："<<clock()/CLOCKS_PER_SEC<<std::endl;
}

void test(){
 uint64_t ms = Trluper::GetCurrentMs();
    sleep(2);
    ms=Trluper::GetCurrentMs()-ms;
    std::cout<<ms<<std::endl;

    Trluper::TimerManager manager(4,3,20000);
    std::function<void(void*)> cb(timerTest);
    manager.addTimer(40000,cb,NULL,true);
    manager.addTimer(2000,cb,NULL,true);
    //manager.addTimer(120000,cb,NULL,true);
    while(true){
        sleep(2);
        std::list<Trluper::Timer::ptr> tlist;
        manager.listExpiredCb(tlist);
        for(auto p:tlist){
            p->callBack();
            if(p->isRecur()){
                p->resetTimer(false,2000);
            }
        }
    }
}
#endif

int main(){
    
#ifdef _TIMER_TEST 
    test();
#endif
    Trluper::LoggerManager* lm= Trluper::LoggerManager::getSingle();
    Trluper::Logger::ptr mainLogger = lm->getMainLogger();
    std::string path = "../Log/log.txt";
    //mainLogger->setFormatter("[%d]-[%p]-%f: %m%n");
    Trluper::LogAppender::ptr p(new Trluper::FileLogAppender(path));
    p->setLogLevel(Trluper::LogLevel::ERROR);
    mainLogger->addAppender(p); //logger的level是DEBUF，但FileLogAppende为ERROR
    LOG_SS_DEBUG(mainLogger)<<"Hello Trluper log";
    LOG_SS_ERROR(mainLogger)<<"ERROR IN LINE 61";
    LOG_FMT_DEBUG(mainLogger,"This is %s, Line:%d","Trluper log",61);
    /*
    //C/S回显测试代码
    std::string configPath = "/home/project/Trluper/config/config.json";
    Trluper::AbstractFactory* factorySingle = Factory::GetSingle();
    Trluper::Server::ServerExceptionStop();
    Trluper::Server::ServerInit(configPath,factorySingle);
    Trluper::Server::ServerRun();
    */
}

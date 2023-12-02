#include "timer.h"

void timerTest(void* arg){

   std::cout<<"Timer Test,当前时间："<<clock()/CLOCKS_PER_SEC<<std::endl;
}

void test(){
    uint64_t ms = Trluper::GetCurrentMs();
    sleep(2);
    ms=Trluper::GetCurrentMs()-ms;
    std::cout<<ms<<std::endl;

    Trluper::TimerManager manager(4,3,2000);
    std::function<void(void*)> cb(timerTest);
    manager.addTimer(10000,cb,NULL,true);
    //manager.addTimer(2000,cb,NULL,true);
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

int main(){
    test();
    return 0;
}
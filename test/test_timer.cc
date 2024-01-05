#include "timer.h"

void timerTest(void* arg){

   std::cout<<"Timer Test,当前时间："<<clock()/CLOCKS_PER_SEC<<std::endl;
}

void test(){
    /*
    uint64_t ms = Trluper::GetCurrentMs();
    sleep(2);
    ms=Trluper::GetCurrentMs()-ms;
    std::cout<<ms<<std::endl;
    */

    Trluper::TimerManager manager(200,60,300);
    std::function<void(void*)> cb(timerTest);
    manager.addTimer(9000,cb,NULL,true);
     manager.addTimer(1000,cb,NULL,false);
   // manager.addTimer(5000,cb,NULL,true);
   // manager.addTimer(1000,cb,NULL,true);
   // manager.addTimer(500,cb,NULL,true);
    //manager.addTimer(1000,cb,NULL,true);
    //manager.addTimer(2000,cb,NULL,true);
    //manager.addTimer(120000,cb,NULL,true);
    while(true){
        //sleep(2);
        Trluper::LinkedList<Trluper::Timer> tlist;
        manager.listExpiredCb(tlist);
        auto head = tlist.getHead();
        while(head!=nullptr){
            auto p = head;
            head = head->next;
            p->data.callBack();
            if(p->data.isRecur()){
                p->data.resetTimer(p,false,2000);
            }else{
                delete p;
            }
        }
        tlist.setNullptr();
    }
}

int main(){
    test();
    return 0;
}
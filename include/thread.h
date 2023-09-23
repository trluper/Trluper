#ifndef _THREAD_H_
#define _THREAD_H_
#include <pthread.h>
#include <functional>
#include <thread>
#include <memory>
#include "noncopyable.h"



namespace Trluper{

class Thread:Noncopyable{

private:
    pid_t id = -1;  //线程id
    pthread_t m_thread = 0; //线程结构
    std::function<void()> m_cb; //线程的执行函数
};


}

#endif 
#ifndef _THREAD_H_
#define _THREAD_H_
#include <pthread.h>
#include <functional>
#include <thread>
#include <memory>
#include <string>
#include <string.h>
#include <vector>
#include <unistd.h>
#include <sys/syscall.h>
#include "noncopyable.h"



namespace Trluper{


/// @brief 封装线程
class Thread:Noncopyable
{
public:
    Thread(std::function<void*(void*)> cb,void* arg,std::string& name);
    ~Thread();
    pid_t getId()const{return m_id;}
    const std::string& getName()const{return m_name;}
    //回收线程
    void join();
    //静态方法获得当前线程的指针
    static Thread* Getthis();
    static const std::string& GetName();
    static void SetName(const std::string& name);
private:
    //线程的执行函数
    static void* run(void* arg);
private:
    pid_t m_id = -1; //线程id
    pthread_t m_thread = 0; //线程结构，重要变量
    std::function<void*(void*)> m_cb; //线程类回调函数
    void * m_arg = nullptr;            //线程类回调函数的参数
    std::string m_name;     //线程名称
};

}

#endif 
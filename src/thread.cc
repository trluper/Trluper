#include "thread.h"

namespace Trluper{

    static thread_local Thread* t_thread = nullptr; //thread_local是c++11添加的关键句，器指示该变量在每一个线程都有自己的副本，不会相互影响
    static thread_local std::string t_thread_name = "Trluper";


    Thread::Thread(std::function<void*(void*)> cb, void * arg , std::string &name):m_cb(cb),m_arg(arg),m_name(name)
    {
        if(name.empty()){
            m_name ="Trluper";
        }
        //创建线程，当前的线程回调函数为run，run内部调用m_cb，传入的参数为this
        int ret = pthread_create(& m_thread,nullptr,& Thread::run,this);
        if(ret){
            printf("Thread error:%s\n",strerror(ret));
            throw std::exception();
        }
    }
    Thread::~Thread()
    {
        if(m_thread){
            pthread_detach(m_thread);
        }
    }
    void Thread::join()
    {
        if(m_thread){
            int ret = pthread_join(m_thread,nullptr);
            if(ret){
                printf("join:%s\n",strerror(ret));
                throw std::exception();
            }
            m_thread = 0;
        }
    }
    Thread *Thread::Getthis()
    {
        return t_thread;
    }
    const std::string &Thread::GetName()
    {
        return t_thread_name;
    }
    void Thread::SetName(const std::string &name)
    {
        if(name.empty()){
            return;
        }
        if(t_thread){
            t_thread ->m_name = name;
        }
        t_thread_name = name;
    }

    //创建Thread线程后该线程会执行的函数，内部要调用真正的回调函数cb
    void *Thread::run(void *arg)
    {
        Thread * thread =(Thread *)arg;
        t_thread = thread;
        t_thread_name = thread->m_name;
        thread->m_id = syscall(SYS_gettid);
        pthread_setname_np(pthread_self(),thread->m_name.substr(0,16).c_str());
        std::function<void*(void*)> cb;
        cb.swap(thread->m_cb);
        void* _arg = thread->m_arg;
        cb(_arg);
        return 0;
    }
}
#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_
#include <list>
#include <sys/epoll.h>
#include "thread.h"
#include "mutex.h"

namespace Trluper{

/// @brief 线程池管理类

class ThreadPool
{
public:
    ThreadPool(int thread_nums = 8,int max_handle = 8000);
    ~ThreadPool();
    bool pushTask(struct epoll_event* task);

private:
    static void* work(void* arg);
    void _run();
private:
    int m_worker_nums;  //线程池中的线程数量
    int m_max_handle;   //每次epoll_wait返回后的最大可读socket数量
    std::vector<Thread*> m_workers; //管理线程池中的线程
    std::list<struct epoll_event*> m_taskQueue;  //任务队列
    Mutex
};


}

#endif
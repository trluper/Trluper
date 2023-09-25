#include "threadPool.h"

namespace Trluper{
    ThreadPool::ThreadPool(int worker_nums, int max_handle):m_worker_nums(worker_nums),m_max_handle(max_handle)
    {

    }

    ThreadPool::~ThreadPool()
    {
    }
    bool ThreadPool::pushTask(epoll_event *task)
    {
        return false;
    }
    void *ThreadPool::work(void *arg)
    {
        return nullptr;
    }
    void ThreadPool::_run()
    {
        
    }
}
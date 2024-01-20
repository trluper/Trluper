#include "threadPool.h"

namespace Trluper{
    static Logger::ptr logger = nullptr;

    ThreadPool::ThreadPool(size_t worker_nums, size_t max_handle):m_worker_nums(worker_nums),m_max_handle(max_handle)
    {
        logger = LOG_GET_MANAGER->getMainLogger();
        //LOG_SS_INFO(logger)<<"Initial threadpool worker_nums:"<< m_worker_nums<<" max_handle: "<<m_max_handle<<std::endl;
        if(worker_nums<=0||max_handle<=0){

            LOG_SS_ERROR(logger)<<"Initial threadpool failed.Worker_nums or max_handle is less than zero."<<std::endl;
        }
        std::function<void*(void*)> _work(work);
        for(size_t i = 0;i<worker_nums;++i){
            std::string name="Trluper-"+std::to_string(i);
            Thread* thread = new Thread(_work,this,name);
            m_workers.push_back(thread);
        }
    }

    ThreadPool::~ThreadPool()
    {
        for(auto thread:m_workers){
            delete thread;
        }
    }
    bool ThreadPool::pushTask(epoll_event *task)
    {
        m_taskQueue.push_back(task);
        return true;
    }
    void *ThreadPool::work(void *arg)
    {
        ThreadPool* pool = (ThreadPool*)arg;
        pool->_run();
        return pool;
    }
    void ThreadPool::_run()
    {
        bool newEpoll = true;
        while(true){
            if(newEpoll){
                m_sem_taskQueue.wait();
                newEpoll = false;
            }
            m_lock_taskQueue.lock();
            if(m_taskQueue.empty()){
                newEpoll = true;
                m_lock_taskQueue.unlock();
                Server::m_serversem.notify();
                continue;
            }
            struct epoll_event* ev = m_taskQueue.front();
            m_taskQueue.pop_front();
            m_lock_taskQueue.unlock(); 

            if(ev->data.fd==Server::ServerGetListenFd()&&ev->events&EPOLLIN){ 
                Server::ServerAccept();
            }
            //*dataFd读事件，服务器epoll_wait只获取读事件，写事件在处理完请求后，直接响应发送
            else if(ev->events&EPOLLIN){
                IOState state(IO_Direction::IN);
                Connections* conn =(Connections*)ev->data.ptr;
                //std::cout<<conn->GetFd()<<std::endl;
                conn->Handle(state);
                //判断客户端的连接状态
                if(conn->ConnectionNeedClose()==true){
                    Server::ServerDelConn(*ev);
                }
            }
        }
    }
}
#ifndef _MUTEX_H_
#define _MUTEX_H_

#include <mutex.h>
#include <pthread.h>
#include <atomic>
#include <semaphore.h>
#include "noncopyable.h"
/*
*应用RAII，该头文件实现对POSIX互斥锁、读写锁、自旋锁、信号量的封装,c++11 lock_guard的RAII实现
*/
namespace Trluper{

/// @brief 互斥锁
class Mutex:Noncopyable{
public:
    Mutex(){
        pthread_mutex_init(&m_mutex,nullptr);
    }
    ~Mutex(){
        pthread_mutex_destroy(&m_mutex);
    }
    void lock(){
        pthread_mutex_lock(&m_mutex);
    }
    void unlock(){
        pthread_mutex_unlock(&m_mutex);
    }

private:
    pthread_mutex_t m_mutex;
};

/// @brief 读写锁
class RWMutex:Noncopyable{
public:
    RWMutex(){
        pthread_rwlock_init(&m_rwlock,nullptr);
    }
    ~RWMutex(){
        pthread_rwlock_destroy(&m_rwlock);
    }

    /// @brief 上读锁
    void rdlock(){
        pthread_rwlock_rdlock(&m_rwlock);
    }

    /// @brief 上写锁
    void wrlock(){
        pthread_rwlock_wrlock(&m_rwlock);
    }
    void unlock(){
        pthread_rwlock_unlock(&m_rwlock);
    }
private:
    pthread_rwlock_t m_rwlock;
};

/// @brief 自旋锁
class Spinlock:Noncopyable{
public:
    Spinlock(){
        pthread_spin_init(&m_spainlock,0);
    }
    ~Spinlock(){
        pthread_spin_destroy(&m_spainlock);
    }
    void lock(){
        pthread_spin_lock(&m_spainlock);
    }
    void unlock(){
        pthread_spin_unlock(&m_spainlock);
    }
private:
    pthread_spinlock_t m_spainlock;
};

/// @brief 原子锁：atomic_flag实现的自旋锁
class CASlock:Noncopyable{
public:
    CASlock(){
        m_mutex.clear();
    }
    ~CASlock(){

    }
    /// @brief 上锁，不满足时自旋(while语句)
    void lock(){
        while(m_mutex.test_and_set(std::memory_order_acquire));
    }
    void unlock(){
        m_mutex.clear(std::memory_order_release);
    }
private:
    //原子状态,atomic_flag是C++标准库提供的原子布尔标志，可以确保多线程环境下对其操作的原子性，通常用它来实现自旋锁
    volatile std::atomic_flag m_mutex = ATOMIC_FLAG_INIT;
};

/// @brief 信号量
class Semaphore:Noncopyable{
public:
    Semaphore(uint32_t count = 0){
        if(sem_init(&m_semaphore,0,count)){

        }
    }
    ~Semaphore(){
        sem_destroy(&m_semaphore);
    }
    void wait(){
        if(sem_wait(&m_semaphore)){

        }
    }
    void notify(){
        if(sem_post(&m_semaphore)){

        }
    }
private:
    sem_t m_semaphore;
};

/// @brief lock_guard<>模板实现，该模板只适用于互斥锁、自旋锁和原子锁
/// @tparam T 锁的类型:
template<class T>
class Lockguard{
public:
    Lockguard(T& _mutex):m_mutex(_mutex){
        m_mutex.lock();
        m_locked = true;
    }
    ~Lockguard(){
        unlock();
    }
    void lock(){
        if(!m_locked){
            m_mutex.lock();
            m_locked = true;
        }
    }
    void unlock(){
        if(m_locked){
            m_mutex.unlock();
            m_locked = false;
        }
    }
private:
    T& m_mutex;
    bool m_locked;
};

/// @brief lock_guard<>模板实现，该模板只适用于读写锁的读操作上锁
/// @tparam T 读写锁类
template<typename T>
class ReadLockguard{
public:
    ReadLockguard(T& _rdmutex):m_rdmutex(_rdmutex){
        m_rdmutex.rdlock();
        m_locked = true;
    }
    ~ReadLockguard(){
        unlock();
    }
    void lock(){
        if(!m_locked){
            m_rdmutex.rdlock();
            m_locked = true;
        }
    }
    void unlock(){
        if(m_locked){
            m_rdmutex.unlock();
            m_locked = false;
        }
    }
private:
    T& m_rdmutex;
    bool m_locked;
};


/// @brief lock_guard<>模板实现，该模板只适用于读写锁的读操作上锁
/// @tparam T 读写锁类
template<typename T>
class WriteLockguard{
public:
    WriteLockguard(T& _wrmutex):m_wrmutex(_wrmutex){
        m_wrmutex.wrlock();
        m_locked = true;
    }
    ~WriteLockguard(){
        unlock();
    }
    void lock(){
        if(!m_locked){
            m_wrmutex.wrlock();
            m_locked = true;
        }
    }
    void unlock(){
        if(m_locked){
            m_wrmutex.unlock();
            m_locked = false;
        }
    }
private:
    T& m_wrmutex;
    bool m_locked;
};

}
#endif
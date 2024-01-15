/**
* @file timer.h
* @brief 对定时器的封装，提供了单定时器类和定时器管理类，使用多级时间轮的形式来管理定时器
* @author Trluper
* @date 2023/10/5
*/

#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>
#include <exception>
#include <iostream>
#include <vector>
#include <memory>
#include <set>
#include <list>
#include "mutex.h"
#include "util.h"


namespace Trluper{

class TimerManager;
template<typename T> class Node;
struct LinkedList;


class Timer:public std::enable_shared_from_this<Timer>{//C++11新特性，异步调用保活(shared_ptr的使用注意)
private:
    //声明定时器管理类为友元类
    friend class LinkedList;
    friend class TimerManager;
    template<typename T>friend class Node;
public:
    //定时器的智能指针类型
    typedef std::shared_ptr<Timer>  ptr;
    typedef Timer* _ptr;

    /**
    * @brief 私有化构造函数
    * @param ms 定时器的执行间隔
    * @param cb void()类型的回调函数
    * @param recurring 是否循环
    * @param manager 定时器管理类
    */
    Timer(uint64_t ms,std::function<void(void*)>cb,void* arg,bool recurring,TimerManager* manager);
    /// @brief 设置为触发状态,当为false相当于定时器的删除，只不过延迟到触发时才统一释放
    void SetTriggerState(bool trigger){this->m_trigger = trigger;}
    
    /// @brief 获得定时器的触发状态，判断是否触发
    /// @return 若触发，返回true
    bool getTriggerState(){return m_trigger;}
    
    /// @brief 判断当前定时器是否为循环定时器
    /// @return true为循环定时器应重上时间轮，false为非循环
    bool isRecur(){return m_recurring;}
    /// @brief 回调函数调用接口
    void callBack(){m_cb(m_arg);}

private:
    /// @brief 重新设置定时器在时间轮的哪一级,哪一可刻度上,如果需要重置定时器触发reset则置为true,并设置新的ms
    /// @param ms 定时器执行的间隔时间
    /// @param from_now 是否从当前时间开始计算
    bool resetTimer(Node<Timer>* timer, bool reset,uint64_t ms,bool from_now = true);
    /// @brief 重新设置定时器在时间轮的哪一级,哪一可刻度上
    void refresh(Node<Timer>* timer);
    Timer(){}
private:
    //是否循环定时器
    bool m_recurring = false;
    //是否触发(该属性用于定时器可重置场景,当被重置是置为false,管理器的时间轮轮询到后不会触发,但重新设置为true)
    bool m_trigger = true;
    //执行周期
    uint64_t m_ms = 0;
    //回调函数
    std::function<void(void*)> m_cb;
    void* m_arg = nullptr;
    //定时器管理器
    TimerManager* m_manager = nullptr;

};

/// @brief 定时器管理类,以多级时间轮实现定时器的管理调度(二级时间轮)
class TimerManager{
private:
    friend class Timer;
public:
    /// @brief 创建定时器管理类
    /// @param workScale 工作轮的刻度数量
    /// @param secondScale 二级轮的刻度数量
    /// @param precisionMs 时间轮的精度，也是允许定时器的最小定时间隔
    TimerManager(uint32_t workScale,uint32_t secondScale,uint64_t precisionMs);
    
    virtual ~TimerManager();
    
    //创建定时器
    Node<Timer>* addTimer(uint64_t ms,std::function<void(void*)> cb,void* arg,bool recurring = false);
    //将定时器添加进管理器的时间轮中,提供写锁
    void addTimer(Node<Timer>* timer);
    //得到第一级即工作轮的最近一个定时器的执行时间，如果第一级没有，则会返回第二级时间轮的槽时间
    uint64_t getNextTimer();
    
    //获得需要当前需要执行的定时器列表(轮询)
    void listExpiredCb(Trluper::LinkedList& tlist);
    
    //是否有定时器在时间轮上，不推荐使用，该接口会造成写锁阻塞
    bool hasTimer();
    
private:
    //二级时间轮
    std::vector<Trluper::LinkedList> m_workWheel;
    std::vector<std::vector<Trluper::LinkedList>> m_secondWheel;
    
    //时间轮的刻度
    uint32_t m_workScale;
    uint32_t m_secondScale;
    
    //时间轮此时指针所在刻度
    uint32_t m_workPtr = 0;
    uint32_t m_secondPtr = 0;
    
    //上一次执行的时间
    uint64_t m_previouseTime = 0;
    
    //时间轮精度(时间轮的粒度)
    uint64_t m_precisonMs;
    
    //读写锁
    RWMutex m_mutex;
};

template<typename T>
struct Node
{
    T data;
    Node<T>* next,*prio;
    Node():next(nullptr),prio(nullptr) {}
    Node(Timer value):data(value),next(nullptr),prio(nullptr) {}
};


struct LinkedList
{
public:
    LinkedList():m_len(0) {
        m_pHead = new Node<Timer>();
        m_pTail = m_pHead;
    }
    ~LinkedList() {
        while(m_pHead != nullptr) {
            Node<Timer>* temp = m_pHead;
            m_pHead = m_pHead->next;
            if(m_pHead != nullptr) m_pHead->prio = nullptr;
            temp->next = nullptr;
            //!segmentation fault
            delete temp;
            --m_len;
        }
    }
    
    /// @brief 尾部插入新节点
    /// @param _node 新节点
    void addNode(Node<Timer>* _node) {
        if(_node == nullptr)  {
            return;
        }
        m_pTail->next = _node;
        _node->prio = m_pTail;
        m_pTail = m_pTail->next;
        ++m_len;
    }
    Node<Timer>* getHead() { return m_pHead->next; }
    void setNullptr() {
        if(m_pHead->next != nullptr) m_pHead->next->prio = nullptr;
        m_pHead->next = nullptr;
        m_pTail = m_pHead;
        m_len = 0;
    }
    bool empty() { return m_len==0; }
    size_t size() { return m_len; }

private:
    Node<Timer>* m_pHead;
    Node<Timer>* m_pTail;
    size_t m_len;
};

}

#endif
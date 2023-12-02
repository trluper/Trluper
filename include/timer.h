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


class Timer:public std::enable_shared_from_this<Timer>{//C++11新特性，异步调用保活(shared_ptr的使用注意)
private:
    //声明定时器管理类为友元类
    friend class TimerManager;
public:
    //定时器的智能指针类型
    typedef std::shared_ptr<Timer>  ptr;

    /// @brief 设置为触发状态,当为false相当于定时器的删除，只不过延迟到触发时才统一释放
    void SetTriggerState(bool trigger){this->m_trigger = trigger;}
    
    /// @brief 获得定时器的触发状态，判断是否触发
    /// @return 若触发，返回true
    bool getTriggerState(){return m_trigger;}
    
    /// @brief 判断当前定时器是否为循环定时器
    /// @return true为循环定时器应重上时间轮，false为非循环
    bool isRecur(){return m_recurring;}
    
    /// @brief 重新设置定时器在时间轮的哪一级,哪一可刻度上,如果需要重置定时器触发reset则置为true,并设置新的ms
    /// @param ms 定时器执行的间隔时间
    /// @param from_now 是否从当前时间开始计算
    bool resetTimer(bool reset,uint64_t ms,bool from_now = true);
    
    /// @brief 回调函数调用接口
    void callBack(){m_cb(m_arg);}
private:
    /// @brief 重新设置定时器在时间轮的哪一级,哪一可刻度上
    void refresh();
private:
    /**
    * @brief 私有化构造函数
    * @param ms 定时器的执行间隔
    * @param cb void()类型的回调函数
    * @param recurring 是否循环
    * @param manager 定时器管理类
    */
    Timer(uint64_t ms,std::function<void(void*)>cb,void* arg,bool recurring,TimerManager* manager);
    
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
    Timer::ptr addTimer(uint64_t ms,std::function<void(void*)> cb,void* arg,bool recurring = false);
    
    //得到第一级即工作轮的最近一个定时器的执行时间，如果第一级没有，则会返回第二级时间轮的槽时间
    uint64_t getNextTimer();
    
    //获得需要当前需要执行的定时器列表
    void listExpiredCb(std::list<Timer::ptr>& tlist);
    
    //是否有定时器在时间轮上，不推荐使用，该接口会造成写锁阻塞
    bool hasTimer();
    
protected:
    //将定时器添加进管理器的时间轮中,提供写锁
    void addTimer(Timer::ptr val,WriteLockguard<RWMutex>& wlock);

private:
    //二级时间轮
    std::vector<std::list<Timer::ptr>> m_workWheel;
    std::vector<std::vector<std::list<Timer::ptr>>> m_secondWheel;
    
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



}

#endif
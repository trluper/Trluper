#include "timer.h"
namespace Trluper{


bool Timer::resetTimer(bool reset, uint64_t ms, bool from_now)
{
    if(reset) this->m_ms=ms;
    return refresh();
}

bool Timer::refresh()
{
    int slots = this->m_ms/m_manager->m_precisonMs;
    if(slots<m_manager->m_workScale){
        slots = (slots+m_manager->m_workPtr)%m_manager->m_workScale;
        m_manager->m_workWheel[slots].push_back(shared_from_this());
    }
    else{
        int secondSlot = (slots/m_manager->m_workScale+m_manager->m_secondPtr-1)
                        %m_manager->m_secondScale;
        slots = slots%m_manager->m_workScale;
        m_manager->m_secondWheel[secondSlot][slots].push_back(shared_from_this());
    }
}


Timer::Timer(uint64_t ms, std::function<void(void *)> cb, void* arg,bool recurring, TimerManager *manager):
    m_ms(ms),m_cb(cb),m_arg(arg),m_recurring(recurring),m_manager(manager){
     
}

TimerManager::TimerManager(int workScale,int secondScale,int precisionMs):
    m_workScale(workScale),m_secondScale(secondScale),m_precisonMs(precisionMs){
    m_workWheel.resize(m_workScale);
    m_secondWheel.resize(m_secondScale);
    for(int i = 0;i<m_secondScale;++i){
        m_secondWheel[i].resize(m_workScale);
    }
    m_previouseTime = Trluper::GetCurrentMs();
}

TimerManager::~TimerManager()
{
}
Timer::ptr TimerManager::addTimer(uint64_t ms, std::function<void(void *)> cb,void* arg, bool recurring)
{
    try
    {
        if(ms<m_precisonMs){
            throw std::exception();
        }
        else{
            Timer::ptr timer(new Timer(ms,cb,arg,recurring,this));
            WriteLockguard<RWMutex> writelock(this->m_mutex);
            addTimer(timer,writelock);
            return timer;
        }
    }
    catch(const std::exception& e)
    {
        std::cout<<"创建定时器失败，定时间隔小于最小定时精度："<<m_precisonMs<<"ms"<<std::endl;
    }
}

uint64_t TimerManager::getNextTimer()
{
    ReadLockguard<RWMutex> readlock(m_mutex);
    uint64_t ms = m_precisonMs*m_workScale+5;
    int count = 0;
    for(int i = 0;i<m_workScale;++i){
        int index = (i+m_workPtr)%m_workScale;
        if(m_workWheel[index].empty()){
            ++count;
            continue;
        }
        else{
            ms = count*m_precisonMs+5;
            break;
        }
    }
    return ms;    
}

void TimerManager::listExpiredCb(std::list<Timer::ptr> &tlist)
{
    WriteLockguard<RWMutex> writelock(m_mutex);
    uint64_t Now = Trluper::GetCurrentMs();
    uint64_t ms = Now-m_previouseTime;
    if(ms>=m_precisonMs){
        m_previouseTime = Now;
        int slots = ms/m_precisonMs;
        for(int i = 1;i <= slots;++ i){
            if(!m_workWheel[(m_workPtr+i)%m_workScale].empty()){
                std::list<Timer::ptr> temp(std::move(m_workWheel[(m_workPtr+i)%m_workScale]));
                //*该方法虽然为O(1)时间复杂度，但在大量数据情况下仍有会出现耗时，后续可以自己实现一个list来优化
                tlist.splice(tlist.end(),temp,temp.begin(),temp.end());
            }
        }
        int addSlot = m_workPtr+slots;
        m_workPtr = (addSlot)%m_workScale;
        if(addSlot>=m_workScale){
            for(int i = 0;i<m_workScale;++i){
                std::list<Timer::ptr> temp(std::move(m_secondWheel[m_secondPtr][i]));
                m_workWheel[i].splice(m_workWheel[i].end(),temp,temp.begin(),temp.end());
            }
            m_secondPtr = (m_secondPtr+1)%m_secondScale;
        }
    }
    
}

bool TimerManager::hasTimer()
{

    for(int i = 0;i < m_workScale;++i){
        if(!m_workWheel[i].empty()){
             return true;
        }
    }
    for(int i = 0;i<m_secondScale;++i){
        for(int j = 0;i<m_workScale;++j){
            if(!m_secondWheel[i][j].empty()) return true;
        }
    }
    return false;
}



void TimerManager::addTimer(Timer::ptr val, WriteLockguard<RWMutex> &wlock)
{
    val->refresh();
}



}
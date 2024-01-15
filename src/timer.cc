#include "timer.h"
namespace Trluper{


bool Timer::resetTimer(Node<Timer>* timer, bool reset, uint64_t ms, bool from_now)
{
    if(reset) this->m_ms=ms;
    refresh(timer);
    return true;
}

void Timer::refresh(Node<Timer>* timer)
{
    
    uint32_t slots = this->m_ms/m_manager->m_precisonMs;
    /*
    * 两种情况：
    * 1）slots<m_workScale：该定时器放在工作轮上即可，即(m_workPtr+slots)%m_workScale
    * 2)slots>=m_workScale：该定时器不在工作轮，二是在二级中，我们需要定位其所在的二级轮的槽位置
    *       (1)二级轮位置index = (m_secondPtr+⌈(slots+m_workPtr+1-m_workScale)/m_workScale⌉)%m_secondScale,⌈⌉表示向上取整
    *       (2)所在的槽slot = (slots+m_workPtr)%m_workScale
    * 举例：假如m_workScale = 5,m_secondScale=6,经过一段时间后，m_workPtr = 2,m_secondPtr = 3,
    * Ⅰ、假如此时插入一个slots=26的定时器，位置在哪? 使用上面公式二级轮的index = (3+⌈(26+2+1-5)/5⌉)%6 = (3+5)%6=2; slot=(26+2)%5=3
    * Ⅱ、假如此时插入一个slots=7的定时器，位置又在哪？使用上面公式二级轮的index = (3+⌈(7+2+1-5)/5⌉)%6 = (3+1)%6=4; slot=(7+2)%5=4
    * Ⅲ、假如此时插入一个slots=4的定时器，位置又在哪？在工作轮的(2+4)%5=1
    */
    if(slots < m_manager->m_workScale){
        slots = (slots+m_manager->m_workPtr)%m_manager->m_workScale;
        m_manager->m_workWheel[slots].addNode(timer);
    }
    else{
        int index = (m_manager->m_secondPtr+(slots+m_manager->m_workPtr)/m_manager->m_workScale)%m_manager->m_secondScale;  //向上取整⌈a/b⌉==(a+b-1)/b
        slots = (slots+m_manager->m_workPtr)%m_manager->m_workScale;
        m_manager->m_secondWheel[index][slots].addNode(timer);
    }
}


Timer::Timer(uint64_t ms, std::function<void(void *)> cb, void* arg,bool recurring, TimerManager *manager):
    m_recurring(recurring),m_ms(ms),m_cb(cb),m_arg(arg),m_manager(manager){
     
}

TimerManager::TimerManager(uint32_t workScale,uint32_t secondScale,uint64_t precisionMs):
    m_workScale(workScale),m_secondScale(secondScale),m_precisonMs(precisionMs){
    m_workWheel.resize(m_workScale);
    m_secondWheel.resize(m_secondScale);
    for(uint32_t i = 0;i<m_secondScale;++i){
        m_secondWheel[i].resize(m_workScale);
    }
    m_previouseTime = Trluper::GetCurrentMs();
}

TimerManager::~TimerManager()
{
    
}
Node<Timer>* TimerManager::addTimer(uint64_t ms, std::function<void(void *)> cb,void* arg, bool recurring)
{
    Node<Timer>* timer = new Node<Timer>(Timer(ms,cb,arg,recurring,this));
    //Timer::ptr timer(new Timer(ms,cb,arg,recurring,this));
    
    addTimer(timer);
    return timer;
}

uint64_t TimerManager::getNextTimer()
{
    ReadLockguard<RWMutex> readlock(m_mutex);
    uint64_t ms = m_precisonMs*m_workScale+5;
    uint32_t count = 0;
    for(uint32_t i = 0;i<m_workScale;++i){
        uint32_t index = (i+m_workPtr)%m_workScale;
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

void TimerManager::listExpiredCb(Trluper::LinkedList& tlist)
{
    WriteLockguard<RWMutex> writelock(m_mutex);
    uint64_t Now = Trluper::GetCurrentMs();
    uint64_t ms = Now-m_previouseTime;
    if(ms >= m_precisonMs){
        m_previouseTime = Now;
        uint32_t slots = ms/m_precisonMs; 
        bool nextCycle = true;
        for(uint32_t i = 0; i < slots; ++i) {
            uint32_t slot = m_workPtr+i;
            Node<Timer>* node = m_workWheel[slot % m_workScale].getHead();
            m_workWheel[slot % m_workScale].setNullptr();
            tlist.addNode(node);
            //二级轮的对应位置移到工作轮上
            m_workWheel[slot % m_workScale].addNode(m_secondWheel[(m_secondPtr+1)%m_secondScale][slot % m_workScale].getHead());
            m_secondWheel[(m_secondPtr+1)%m_secondScale][slot % m_workScale].setNullptr();
        if(nextCycle && slot >= m_workScale) {
                ++m_secondPtr;
                nextCycle = false;
        }
        }
        m_workPtr = (m_workPtr+slots)%m_workScale;
    }
}

bool TimerManager::hasTimer()
{

    for(uint32_t i = 0;i < m_workScale;++i){
        if(!m_workWheel[i].empty()){
             return true;
        }
    }
    for(uint32_t i = 0;i<m_secondScale;++i){
        for(uint32_t j = 0;i<m_workScale;++j){
            if(!m_secondWheel[i][j].empty()) return true;
        }
    }
    return false;
}



void TimerManager::addTimer(Node<Timer>* timer)
{
    WriteLockguard<RWMutex> writelock(this->m_mutex);
    timer->data.refresh(timer);
}



}
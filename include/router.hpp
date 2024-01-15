#ifndef _ROUTER_H_
#define _ROUTER_H_

#include <unordered_map>
#include "fapplycations.h"


namespace Trluper{

class FApplycations;
class Request;

/**  @brief 路由模块，当框架应用于多业务场景和需要共享的业务应用时，提供给框架在数据处理层时能够依据请求类型来解决交给哪个业务对象处理（此router路由器为模板类，其内部实现的路由功能由unordered_map支持，因此`T`为 
 * unordered_map的关键字类型，简单来讲就是对unordered_map的封装，只提供适用框架的接口
    @tparam T 业务对象的定位关键字类型
**/
template<typename T,typename K>
class Router:public Trluper::AbstractHandle{
public:
    typedef std::unordered_map<T,FApplycations*> hashMap;
public:
    Router();
    virtual ~Router();
    FApplycations* getNextProcessor(T key);
    /// @brief 
    /// @param T 业务应用的关键字
    /// @param value 业务应用的对象指针
    void addProcessor(T key,FApplycations* value);
    void eraseProcessor(T key);
    size_t count(T key);
    size_t size();
    
    /// @brief 将数据处理层递交上来的UserMsg类型的消息进行路由选择，选择不同的请求应该由哪个应用层处理
    /// @param msg UserMsg类型 
    /// @return AbstractMsg
    virtual AbstractMsg* currentHandle(AbstractMsg& msg) override final;
    virtual AbstractHandle* GetNextHanlder(AbstractMsg& msg) override final;
private: 
    hashMap m_routingTable;
};


#endif

template <typename T,typename K>
inline Router<T,K>::Router()
{
    
}

template <typename T,typename K>
inline Router<T,K>::~Router()
{
    for(auto& it: m_routingTable){
        if(it.second != nullptr)
        delete it.second;
        it.second = nullptr;
    }
}

template <typename T,typename K>
inline void Router<T,K>::addProcessor(T key, FApplycations* value)
{
    m_routingTable.insert(std::pair<T,FApplycations*>(key,value));
}

template <typename T,typename K>
inline void Router<T,K>::eraseProcessor(T key)
{
    m_routingTable.erase(key);
}

template <typename T,typename K>
inline FApplycations* Router<T,K>:: getNextProcessor(T key)
{
    if(count(key) != 0) {
        return m_routingTable[key];
    }
    return nullptr;
}



template <typename T,typename K>
inline size_t Router<T,K>::size()
{
    return m_routingTable.size();
}

template <typename T,typename K>
inline AbstractMsg *Router<T,K>::currentHandle(AbstractMsg &msg)
{
    DYNAMIC_GETREF(UserMsg, _msg, msg);
    std::cout<<"this is router"<<std::endl;
    Request* req = _msg->getRequest();
    while(req != nullptr){
        auto currentNode = req;
        req = req->next;
        K _req = dynamic_cast<K>(currentNode);
        T key =  _req->getRequestType();

        std::cout<<_req->ret<<std::endl;

        FApplycations* _app = m_routingTable[key];
        std::cout<<_app<<std::endl;
        UserMsg* arg=new UserMsg(IO_Direction::IN);
        arg->SetRequest(_req);
        _app->Handle(*arg);
        _req = nullptr;
    }
    delete _msg;
    return nullptr;
}

template <typename T,typename K>
inline AbstractHandle *Router<T,K>::GetNextHanlder(AbstractMsg &msg)
{
    return nullptr;
}

template <typename T,typename K>
inline size_t Router<T,K>::count(T key)
{
    return m_routingTable.count(key);
}

}
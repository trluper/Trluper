#pragma once

#include "dataProcess.h"
#include "httpConnection.h"
#include "router.hpp"
#include "httpRequest.hpp"

class HttpDataprocess:Trluper::DataProcess{
public:
    HttpDataprocess();
    HttpDataprocess(Trluper::Connections* conn);
    virtual ~HttpDataprocess();
    void setConnectionsObj(Trluper::Connections* _conn = nullptr);
    void addFApplycationObj(HTTP_REQUEST_TYPE _type,Trluper::FApplycations* _app = nullptr);
    //!开发需要重写该函数获得绑定的conn对象，返回一个Connections对象指针，
    virtual Trluper::Connections* GetConnectionsObj(Trluper::AbstractMsg& msg);
    //!该函数获得绑定的app对象,内部调用RouterOfApp函数获得FApplycations对象指针(添加路由模块后弃用)
    virtual Trluper::FApplycations* GetApplycationsObj(Trluper::AbstractMsg& msg);
    //*将连接层递交上来的message处理为Request类型，这里就要开发者自己依据自定义额数据格式来重写该函数返回一个Request对象指针
    virtual Trluper::Request* MsgToRequest(Trluper::Message& msg);
    //*将应用层递交下来的Request类型的数据转为Message类型，这里就要开发者自己依据自定义额数据格式来重写该函数
    virtual Trluper::Message* RequestToMsg(Trluper::Request& request);
     

private:
    Trluper::Router<HTTP_REQUEST_TYPE> m_router;
    Trluper::Connections* m_conn;

};

HttpDataprocess::HttpDataprocess(){

}

inline HttpDataprocess::HttpDataprocess(Trluper::Connections *conn)
{
    m_conn = conn;
}

inline HttpDataprocess::~HttpDataprocess()
{
    
}

inline void HttpDataprocess::setConnectionsObj(Trluper::Connections *_conn)
{
    this->m_conn = _conn;
}

inline void HttpDataprocess::addFApplycationObj(HTTP_REQUEST_TYPE _type, Trluper::FApplycations *_app)
{
    m_router.addProcessor(_type,_app);
}

inline Connections *HttpDataprocess::GetConnectionsObj(AbstractMsg &msg)
{
    return m_conn;
}

//msg：UserMsg
inline FApplycations *HttpDataprocess::GetApplycationsObj(AbstractMsg &msg)
{
    DYNAMIC_GETREF(Trluper::UserMsg,_msg,msg);
    
}

inline Request *HttpDataprocess::MsgToRequest(Message &msg)
{
    return nullptr;
}

inline Message *HttpDataprocess::RequestToMsg(Request &request)
{
    return nullptr;
}

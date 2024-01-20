#pragma once

#include "dataProcess.h"
#include "httpConnection.hpp"
#include "router.hpp"
#include "httpRequest.hpp"

class  HttpRequestWrapper;
class  HttpResponseWrapper;
class HttpDataprocess:public Trluper::DataProcess{
public:
    HttpDataprocess();
    HttpDataprocess(Trluper::Connections* conn);
    virtual ~HttpDataprocess();
    void setConnectionsObj(Trluper::Connections* _conn = nullptr);
    void setRouterObj(Trluper::Router<HTTP_REQUEST_TYPE,HttpRequestWrapper*>* router);
    void addFApplycationObj(HTTP_REQUEST_TYPE _type,Trluper::FApplycations* _app = nullptr);
    //!开发需要重写该函数获得绑定的conn对象，返回一个Connections对象指针，
    virtual Trluper::Connections* GetConnectionsObj(Trluper::AbstractMsg& msg);
    //!该函数获得绑定的app对象,内部调用RouterOfApp函数获得FApplycations对象指针(添加路由模块后弃用)
    virtual Trluper::AbstractHandle* GetApplycationsObj(Trluper::AbstractMsg& msg);
    //*将连接层递交上来的message处理为Request类型，这里就要开发者自己依据自定义额数据格式来重写该函数返回一个Request对象指针
    virtual Trluper::Request* MsgToRequest(Trluper::Message& msg);
    //*将应用层递交下来的Request类型的数据转为Message类型，这里就要开发者自己依据自定义额数据格式来重写该函数
    virtual Trluper::Message* RequestToMsg(Trluper::Request& request);
    HTTP_REQUEST_TYPE getType(std::string s);

private:
    Trluper::Router<HTTP_REQUEST_TYPE,HttpRequestWrapper*>* m_router;
    Trluper::HttpRequestParser::ptr reqeustParser;
public:
    Trluper::Connections* m_conn = nullptr;
};

HttpDataprocess::HttpDataprocess(){
    reqeustParser = Trluper::HttpRequestParser::getSingle();
}

inline HttpDataprocess::HttpDataprocess(Trluper::Connections *conn):m_conn(conn)
{
    reqeustParser = Trluper::HttpRequestParser::getSingle();
}

inline HttpDataprocess::~HttpDataprocess()
{
    
}

inline void HttpDataprocess::setConnectionsObj(Trluper::Connections *_conn)
{
    this->m_conn = _conn;
}

inline void HttpDataprocess::setRouterObj(Trluper::Router<HTTP_REQUEST_TYPE, HttpRequestWrapper *> *router)
{
    this->m_router = router;
}

inline void HttpDataprocess::addFApplycationObj(HTTP_REQUEST_TYPE _type, Trluper::FApplycations *_app)
{
    this->m_router->addProcessor(_type,_app);
}

inline Trluper::Connections *HttpDataprocess::GetConnectionsObj(Trluper::AbstractMsg &msg)
{
    return m_conn;
}

//msg：UserMsg
inline Trluper::AbstractHandle* HttpDataprocess::GetApplycationsObj(Trluper::AbstractMsg &msg)
{
    return m_router;
    
}

inline Trluper::Request *HttpDataprocess::MsgToRequest(Trluper::Message &msg)
{
    std::string& str=msg.message;
    HttpRequestWrapper* _ret =nullptr,*cursor=nullptr;
    while(1){
        HttpRequestWrapper* node = new HttpRequestWrapper(this);
        Trluper::HttpRequest::ptr req(new Trluper::HttpRequest());
        if(this->reqeustParser->FSMParserHttpRequest(str,*req)){
            std::string s= req->m_path;
            node->m_type = getType(s);
            node->m_request = req;
        }
        else{
            delete node;
            break;
        }
        if(_ret == nullptr){
             _ret = node;
             cursor = node;
        }else{
            cursor->next = node;
            cursor = dynamic_cast<HttpRequestWrapper*>(cursor->next);
        }

    }
    return _ret;
}

inline Trluper::Message *HttpDataprocess::RequestToMsg(Trluper::Request &request)
{
    Trluper::Message* _outMsg = new Trluper::Message(Trluper::IO_Direction::OUT);
    DYNAMIC_GETREF(HttpResponseWrapper,response,request);
    std::string res = response->m_response->httpStringResponse();
    _outMsg->message.append(res);
    delete response;
    response = nullptr;
    return _outMsg;
}

inline HTTP_REQUEST_TYPE HttpDataprocess::getType(std::string s)
{
    if(s=="/"){
        return HTTP_REQUEST_TYPE::TEXT;
    }else{
        size_t pos=s.find('.');
        std::string type(s.substr(pos+1,s.size()));
        if(Trluper::caseInsensitiveCompare("html",type)){
            return HTTP_REQUEST_TYPE::TEXT;
        }
        else if(Trluper::caseInsensitiveCompare("png",type)){
            return HTTP_REQUEST_TYPE::IMAGE;
        }
        else if(Trluper::caseInsensitiveCompare("mp4",type)){
            return HTTP_REQUEST_TYPE::VIDEO;
        }
        else if(Trluper::caseInsensitiveCompare("pdf",type)){
            return HTTP_REQUEST_TYPE::APPLICATION;
        }
        else{
            return HTTP_REQUEST_TYPE::TEXT;
        }
    }
}

#pragma once
#include "dataProcess.h"
#include "myRequest.h"
#include "myConnections.h"
#include "myFApplycations.h"

class MyConnections;
class MyFApplycations;

class MyDataProcess:public Trluper::DataProcess
{
public:
    MyDataProcess();
    virtual ~MyDataProcess();
    void SetConnectionsObj(Trluper::Connections* _conn = nullptr);
    void SetFApplycationsObj(Trluper::FApplycations* _app = nullptr);
    //*重写该函数，返回一个Connections对象指针
    virtual Trluper::Connections* GetConnectionsObj(Trluper::AbstractMsg& msg)override;
    virtual  Trluper::FApplycations* GetApplycationsObj(Trluper::AbstractMsg& msg)override;
    //*重写该函数，这里用TVL形式的数据作为测试
    virtual Trluper::Request* MsgToRequest(Trluper::Message& str)override;
    virtual Trluper::Message* RequestToMsg(Trluper::Request& request)override;
private:
    Trluper::Connections* conn = nullptr;
    Trluper::FApplycations* app = nullptr;
    ListRequest* lreq = nullptr;
};

//5/12
MyDataProcess::MyDataProcess():lreq(new ListRequest())
{

}

MyDataProcess::~MyDataProcess()
{
    if(nullptr != app) delete app;
    if(nullptr != lreq) delete lreq;
}

inline void MyDataProcess::SetConnectionsObj(Trluper::Connections *_conn)
{
    this->conn = _conn;
}

inline void MyDataProcess::SetFApplycationsObj(Trluper::FApplycations *_app)
{
    this->app = _app;
}

inline Trluper::Connections *MyDataProcess::GetConnectionsObj(Trluper::AbstractMsg &msg)
{
    return conn;
}

inline Trluper::FApplycations *MyDataProcess::GetApplycationsObj(Trluper::AbstractMsg &msg)
{
    return app;
}


Trluper::Request *MyDataProcess::MsgToRequest(Trluper::Message& msg)
{
    ListRequest* ret = nullptr;
    std::string& str = msg.message;
    while (1)
    {
        //8字节无法满足
        if(str.size()<8)  break;
        int id = 0;
        id |= str[0]<<0;
        id |= str[1]<<8;
        id |= str[2]<<16;
        id |= str[3]<<24;
        size_t bytelength = 0;
        bytelength |= static_cast<unsigned char>(str[4])<<0;
        bytelength |= static_cast<unsigned char>(str[5])<<8;
        bytelength |= static_cast<unsigned char>(str[6])<<16;
        bytelength |= static_cast<unsigned char>(str[7])<<24;
        //请求不完整
        if(str.size()-8<bytelength){
             break;
        }
        MyRequest* _request =new MyRequest((REQUEST_TYPE)id,str.substr(8,bytelength));
        lreq->lRequest.push_back(_request);
        str.erase(0,8+bytelength);
    }
    if(lreq->lRequest.empty()==false){
        ret = lreq;
    }
   
    return ret;
}

Trluper::Message *MyDataProcess::RequestToMsg(Trluper::Request &request)
{
    Trluper::Message* _CMsg =new Trluper::Message(Trluper::IO_Direction::OUT);
    DYNAMIC_GETREF(MyRequest,_outRequest,request);
    std::string str=_outRequest->ret;
    //int id = _outRequest->requestType;
    //int bytelength = str.size();
    /*
    _CMsg->message.push_back((id>>0) & 0xff);
    _CMsg->message.push_back((id>>8) & 0xff);
    _CMsg->message.push_back((id>>16) & 0xff);
    _CMsg->message.push_back((id>>24) & 0xff);
    _CMsg->message.push_back((bytelength>>0) & 0xff);
    _CMsg->message.push_back((bytelength>>8) & 0xff);
    _CMsg->message.push_back((bytelength>>16) & 0xff);
    _CMsg->message.push_back((bytelength>>24) & 0xff);
    */
    _CMsg->message.append(str);
    delete _outRequest;
    _outRequest = nullptr;
    return _CMsg;
}

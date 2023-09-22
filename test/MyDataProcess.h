#pragma once
#include "DataProcess.h"
#include "MyRequest.h"
#include "MyConnections.h"
#include "MyFApplycations.h"

class MyConnections;
class MyFApplycations;

class MyDataProcess:public DataProcess
{
public:
    MyDataProcess();
    virtual ~MyDataProcess();
    void SetConnectionsObj(Connections* _conn = nullptr);
    void SetFApplycationsObj(FApplycations* _app = nullptr);
    //*重写该函数，返回一个Connections对象指针
    virtual Connections* GetConnectionsObj(AbstractMsg& msg)override;
    virtual  FApplycations* GetApplycationsObj(AbstractMsg& msg)override;
    //*重写该函数，这里用TVL形式的数据作为测试
    virtual Request* MsgToRequest(Message& str)override;
    virtual Message* RequestToMsg(Request& request)override;
private:
    Connections* conn = nullptr;
    FApplycations* app = nullptr;
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

inline void MyDataProcess::SetConnectionsObj(Connections *_conn)
{
    this->conn = _conn;
}

inline void MyDataProcess::SetFApplycationsObj(FApplycations *_app)
{
    this->app = _app;
}

inline Connections *MyDataProcess::GetConnectionsObj(AbstractMsg &msg)
{
    return conn;
}

inline FApplycations *MyDataProcess::GetApplycationsObj(AbstractMsg &msg)
{
    return app;
}


Request *MyDataProcess::MsgToRequest(Message& msg)
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
        int bytelength = 0;
        bytelength |= str[4]<<0;
        bytelength |= str[5]<<8;
        bytelength |= str[6]<<16;
        bytelength |= str[7]<<24;
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

Message *MyDataProcess::RequestToMsg(Request &request)
{
    Message* _CMsg =new Message(IO_Direction::OUT);
    DYNAMIC_GETREF(MyRequest,_outRequest,request);
    std::string str=_outRequest->ret;
    int id = _outRequest->requestType;
    int bytelength = str.size();
    _CMsg->message.push_back((id>>0) & 0xff);
    _CMsg->message.push_back((id>>8) & 0xff);
    _CMsg->message.push_back((id>>16) & 0xff);
    _CMsg->message.push_back((id>>24) & 0xff);
    _CMsg->message.push_back((bytelength>>0) & 0xff);
    _CMsg->message.push_back((bytelength>>8) & 0xff);
    _CMsg->message.push_back((bytelength>>16) & 0xff);
    _CMsg->message.push_back((bytelength>>24) & 0xff);
    _CMsg->message.append(str);
    delete _outRequest;
    _outRequest = nullptr;
    return _CMsg;
}

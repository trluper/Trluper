#pragma once
#include "FApplycations.h"
#include "MyDataProcess.h"
#include "MyRequest.h"
#include "Server.h"

class MyDataProcess;
class Server;
class MyRequest;
class ListRequest;

class MyFApplycations:public FApplycations
{
public:
    MyFApplycations();
    virtual ~MyFApplycations();
    void SetMyDataProcessObj(DataProcess* _process);
    DataProcess* GetMyDataProcessObj(AbstractMsg& msg);
protected:
    //处理消息类型： REQUEST_TYPE_ECHO = 1的函数入口，如果需要下一个处理者处理，那么就会返回处理好的Request*
    Request* echoFunc(std::string str);
    Request* echoAndLenFunc(std::string str);
    //处理消息类型： REQUEST_TYPE_ECHO = 1的函数入口，如果需要下一个处理者处理，那么就会返回处理好的Request*
    virtual Request* ProcRequest(Request& _request)override;
    //下一个业务处理者处理
    virtual FApplycations* GetNextApplycationsObj(AbstractMsg& msg)override;
private:
    DataProcess* process = nullptr;
};

MyFApplycations::MyFApplycations()
{
}

MyFApplycations::~MyFApplycations()
{
    
}

inline void MyFApplycations::SetMyDataProcessObj(DataProcess *_process)
{
    this->process = _process;
}

inline DataProcess *MyFApplycations::GetMyDataProcessObj(AbstractMsg &msg)
{
    return process;
}

inline Request *MyFApplycations::echoFunc(std::string str)
{
    std::cout<<"recvfrom client: "<<str<<std::endl;
    //数据层需要释放
    MyRequest* resRequest = new MyRequest( REQUEST_TYPE::REQUEST_TYPE_ECHO,str); //7/12(解决)
    Server::ServerUseHandleOfDataProcess(*process,resRequest);
    return nullptr;
}

inline Request *MyFApplycations::echoAndLenFunc(std::string str)
{
    std::cout<<"recvfrom client: "<<str<<". lenSize: "<<str.size()<<std::endl;
    MyRequest* resRequest = new MyRequest( REQUEST_TYPE::REQUEST_TYPE_ECHO_LEN,str);
    Server::ServerUseHandleOfDataProcess(*process,resRequest);
    return nullptr;
}

inline Request *MyFApplycations::ProcRequest(Request& _request)
{
    DYNAMIC_GETREF(ListRequest,_lRequestObj,_request);
   while(false == _lRequestObj->lRequest.empty()){
        auto request = _lRequestObj->lRequest.front();
        _lRequestObj->lRequest.pop_front();
        std::cout<<"Type is: "<<request->requestType<<std::endl;
        switch (request->requestType)
        {
        case REQUEST_TYPE::REQUEST_TYPE_ECHO:
            echoFunc(request->ret);
            break;
        case REQUEST_TYPE::REQUEST_TYPE_ECHO_LEN:
            echoAndLenFunc(request->ret);
            break;
        }
        //释放数据层的分配的堆
        delete request;
    }
    return nullptr;
}

inline FApplycations *MyFApplycations::GetNextApplycationsObj(AbstractMsg& msg)
{
    return nullptr;
}

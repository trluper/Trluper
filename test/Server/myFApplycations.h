#pragma once
#include "fapplycations.h"
#include "myDataProcess.h"
#include "myRequest.h"
#include "server.h"

class MyDataProcess;
class MyRequest;


class MyFApplycations:public Trluper::FApplycations
{
public:
    MyFApplycations();
    virtual ~MyFApplycations();
    void SetMyDataProcessObj(Trluper::DataProcess* _process);
    Trluper::DataProcess* GetMyDataProcessObj(Trluper::AbstractMsg& msg);
protected:
    //处理消息类型： REQUEST_TYPE_ECHO = 1的函数入口，如果需要下一个处理者处理，那么就会返回处理好的Request*
    Trluper::Request* echoFunc(std::string str);
    Trluper::Request* echoAndLenFunc(std::string str);
    //处理消息类型： REQUEST_TYPE_ECHO = 1的函数入口，如果需要下一个处理者处理，那么就会返回处理好的Request*
    virtual Trluper::Request* ProcRequest(Trluper::Request& _request)override;
    //下一个业务处理者处理
    virtual Trluper::FApplycations* GetNextApplycationsObj(Trluper::AbstractMsg& msg)override;
private:
    Trluper::DataProcess* process = nullptr;
};

MyFApplycations::MyFApplycations()
{
}

MyFApplycations::~MyFApplycations()
{
    
}

inline void MyFApplycations::SetMyDataProcessObj(Trluper::DataProcess *_process)
{
    this->process = _process;
}

inline Trluper::DataProcess *MyFApplycations::GetMyDataProcessObj(Trluper::AbstractMsg &msg)
{
    return process;
}

inline Trluper::Request *MyFApplycations::echoFunc(std::string str)
{
    std::cout<<"recvfrom client: "<<str<<std::endl;
    //数据层需要释放
    MyRequest* resRequest = new MyRequest(REQUEST_TYPE::REQUEST_TYPE_ECHO,str); //7/12(解决)
    Trluper::Server::ServerUseHandleOfDataProcess(*process,resRequest);
    return nullptr;
}

inline Trluper::Request *MyFApplycations::echoAndLenFunc(std::string str)
{
    std::cout<<"recvfrom client: "<<str<<". lenSize: "<<str.size()<<std::endl;
    MyRequest* resRequest = new MyRequest( REQUEST_TYPE::REQUEST_TYPE_ECHO_LEN,str);
    Trluper::Server::ServerUseHandleOfDataProcess(*process,resRequest);
    return nullptr;
}

inline Trluper::Request *MyFApplycations::ProcRequest(Trluper::Request& _request)
{
    DYNAMIC_GETREF(MyRequest,node,_request);
    while(node != nullptr){
        auto current = node;
        node = dynamic_cast<MyRequest*>(node->next);
        std::cout<<"Type is: "<<current->getRequestType()<<std::endl;
        switch (current->getRequestType())
        {
        case REQUEST_TYPE::REQUEST_TYPE_ECHO:
            echoFunc(current->ret);
            break;
        case REQUEST_TYPE::REQUEST_TYPE_ECHO_LEN:
            echoAndLenFunc(current->ret);
            break;
        }
        //释放数据层的分配的堆
        delete current;
    }
    return nullptr;
}

inline Trluper::FApplycations *MyFApplycations::GetNextApplycationsObj(Trluper::AbstractMsg& msg)
{
    return nullptr;
}

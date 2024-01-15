#include <string>
#include <iostream>
#include "router.hpp"
#include "Server/myRequest.h"
#include "fapplycations.h"

class TestFApplycations:public Trluper::FApplycations
{
public:
    TestFApplycations();
    virtual ~TestFApplycations();
protected:
    //处理消息类型： REQUEST_TYPE_ECHO = 1的函数入口，如果需要下一个处理者处理，那么就会返回处理好的Request*
    Trluper::Request* echoFunc(std::string str);
    Trluper::Request* echoAndLenFunc(std::string str);
    //处理消息类型： REQUEST_TYPE_ECHO = 1的函数入口，如果需要下一个处理者处理，那么就会返回处理好的Request*
    virtual Trluper::Request* ProcRequest(Trluper::Request& _request)override;
    //下一个业务处理者处理
    virtual Trluper::FApplycations* GetNextApplycationsObj(Trluper::AbstractMsg& msg)override;
};

TestFApplycations::TestFApplycations()
{
}

TestFApplycations::~TestFApplycations()
{
    
}

inline Trluper::Request *TestFApplycations::echoFunc(std::string str)
{
    std::cout<<"recvfrom client: "<<str<<std::endl;
    return nullptr;
}

inline Trluper::Request *TestFApplycations::echoAndLenFunc(std::string str)
{
    std::cout<<"recvfrom client: "<<str<<". lenSize: "<<str.size()<<std::endl;
    return nullptr;
}

inline Trluper::Request *TestFApplycations::ProcRequest(Trluper::Request& _request)
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

inline Trluper::FApplycations *TestFApplycations::GetNextApplycationsObj(Trluper::AbstractMsg& msg)
{
    return nullptr;
}

int main(int argc,char* argv[]){
    Trluper::Router<REQUEST_TYPE,MyRequest*> m;
    TestFApplycations* app1=new TestFApplycations();
    TestFApplycations* app2=new TestFApplycations();
    std::cout<<"app1:"<<app1<<" "<<"app2:"<<app2<<std::endl;
    m.addProcessor(REQUEST_TYPE::REQUEST_TYPE_ECHO,app1);
    m.addProcessor(REQUEST_TYPE::REQUEST_TYPE_ECHO_LEN,app2);

    //下语句实际开发不用写，已在框架内部实现，这里只是做测试
    Trluper::UserMsg* msg = new Trluper::UserMsg(Trluper::IO_Direction::IN);
    MyRequest* req=new MyRequest(REQUEST_TYPE::REQUEST_TYPE_ECHO,"ROUTER TEST!!");
    msg->SetRequest(req);
    m.Handle(*msg);  //
   
  
}
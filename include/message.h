#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>
#include <memory>

namespace Trluper{
//!IO方向，IN指示方向为连接层-->数据层-->业务层；OUT为业务层-->数据层-->连接层
enum IO_Direction{
        IN  = 1,
        OUT = 2
}; 

class Request;   

//!所有消息类的父类
class AbstractMsg{
public:
    AbstractMsg() {}
    virtual ~AbstractMsg() {}
};

//!IO状态类，指示该消息是IN<--->read或OUT<--->write状态，不需要继承
class IOState:public AbstractMsg{
public:
    IO_Direction IO_DIC;
    IOState(IO_Direction dic):IO_DIC(dic) {}
    virtual ~IOState() {}
};

//!在连接层会构造的传输的消息封装类,不需要继承重写
class Message:public IOState{
public:
    Message(IO_Direction dic):IOState(dic) {}
    virtual ~Message() {}
    std::string message;
};

//!用于协议层解析请求后，递交给应用层的消息封装类,不需要继承重写
class UserMsg:public IOState
{
public:
    UserMsg(IO_Direction dic):IOState(dic) {}
    virtual ~UserMsg() {}
    Request* getRequest() {return request;}
    void SetRequest(Request* _request) { request = _request;}
private:
    Request* request = nullptr;
};

//!用户请求结构体，由用户来添加相关的业务字段，比如业务的请求类型定义(开发者需继承实现添加业务字段)

struct Request:public AbstractMsg{
public:
    Request():next(nullptr){}
    virtual ~Request() {}
    template<typename T>  T getRequestType(){T e;return e;}
public:
    Request* next;
};


}
#endif
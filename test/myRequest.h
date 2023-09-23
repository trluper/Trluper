#pragma once
#include <google/protobuf/message.h>
#include <list>
#include "message.h"

/*
*继承Request，实现自定义的请求类型业务层，业务层依据请求类型来决定如何处理
!此测试用例用protobuf来模拟两个请求类型的处理，一个是回显，一个是回显+消息长度
*/
enum REQUEST_TYPE{
        REQUEST_TYPE_ECHO = 1,
        REQUEST_TYPE_ECHO_LEN = 2
};

class MyRequest:public Trluper::Request{
public:
    
    MyRequest(REQUEST_TYPE _type,std::string str);
    virtual ~MyRequest();
    

public:
    //用户的protobuf消息指针
    std::string ret;
    //定义了两种类型请求，一个是回显，一个是回显+消息长度
    REQUEST_TYPE requestType;
};

class ListRequest: public Trluper::Request{
public:
    std::list<MyRequest*> lRequest;
};

MyRequest::MyRequest(REQUEST_TYPE _type, std::string str):requestType(_type),ret(str)
{
    
}

MyRequest::~MyRequest()
{
}

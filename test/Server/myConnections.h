#pragma once

#include "connections.h"
#include "myDataProcess.h"
class MyDataProcess;

class MyConnections:public Trluper::Connections
{
public:
    MyConnections(int _dataFd);
    ~MyConnections();
    virtual void SetDataProcessObj(MyDataProcess* obj);
    //重写该函数
    virtual MyDataProcess* GetDataProcessObj(Trluper::Message& msg) override;

private:
    MyDataProcess * process = nullptr;
};



MyConnections::MyConnections(int _dataFd) : Trluper::Connections(_dataFd)
{
    
}

MyConnections::~MyConnections()
{
    if(nullptr != process) delete process;
}

inline void MyConnections::SetDataProcessObj(MyDataProcess *obj)
{
    this->process = obj;
}

inline MyDataProcess *MyConnections::GetDataProcessObj(Trluper::Message &msg)
{
    return process;
}
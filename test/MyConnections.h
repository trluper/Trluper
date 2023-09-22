#pragma once
class MyDataProcess;
#include "Connections.h"
#include "MyDataProcess.h"


class MyConnections:public Connections
{
public:
    MyConnections(int _dataFd);
    ~MyConnections();
    virtual void SetDataProcessObj(MyDataProcess* obj);
    //重写该函数
    virtual MyDataProcess* GetDataProcessObj(Message& msg) override;

private:
    MyDataProcess * process = nullptr;
};



MyConnections::MyConnections(int _dataFd) : Connections(_dataFd)
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

inline MyDataProcess *MyConnections::GetDataProcessObj(Message &msg)
{
    return process;
}
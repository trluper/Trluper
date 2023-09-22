#pragma once

#include "Server.h"
#include "MyConnections.h"
#include "MyDataProcess.h"
#include "MyFApplycations.h"

class Factory:public AbstractFactory
{
private:
    Factory();
    ~Factory(); 
    Factory(const Factory& obj){}
    Factory& operator=(const Factory& obj){}
public:
    static AbstractFactory* GetSingle();
    virtual Connections* CreateAllObjWhenAccept(int _dataFd) override;
private:
    static AbstractFactory* FactorySingle;
};

AbstractFactory* Factory::FactorySingle = new Factory();
Factory::Factory()
{
}

Factory::~Factory()
{
}

AbstractFactory *Factory::GetSingle()
{
    return FactorySingle;
}

Connections *Factory::CreateAllObjWhenAccept(int _dataFd)
{
    MyConnections* conn = new MyConnections(_dataFd);//8/12
    MyDataProcess* process = new MyDataProcess();//4/12
    MyFApplycations* app = new MyFApplycations();//2/12
    conn->SetDataProcessObj(process);
    process->SetConnectionsObj(conn);
    process->SetFApplycationsObj(app);
    app->SetMyDataProcessObj(process);
    return conn;
}

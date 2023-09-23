#pragma once

#include "server.h"
#include "myConnections.h"
#include "myDataProcess.h"
#include "myFApplycations.h"

class Factory:public Trluper::AbstractFactory
{
private:
    Factory();
    ~Factory(); 
    Factory(const Factory& obj){}
    Factory& operator=(const Factory& obj){}
public:
    static Trluper::AbstractFactory* GetSingle();
    virtual Trluper::Connections* CreateAllObjWhenAccept(int _dataFd) override;
private:
    static AbstractFactory* FactorySingle;
};

Trluper::AbstractFactory* Factory::FactorySingle = new Factory();
Factory::Factory()
{
}

Factory::~Factory()
{
}

Trluper::AbstractFactory *Factory::GetSingle()
{
    return FactorySingle;
}

Trluper::Connections *Factory::CreateAllObjWhenAccept(int _dataFd)
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

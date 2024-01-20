#pragma once


#include "httpConnection.hpp"
#include "httpDataProcess.hpp"


class HttpFactory:public Trluper::AbstractFactory
{
private:
    HttpFactory(Trluper::Router<HTTP_REQUEST_TYPE,HttpRequestWrapper*>* router);
    ~HttpFactory(); 
    HttpFactory(const HttpFactory& obj){}
    HttpFactory& operator=(const HttpFactory& obj){return *this;}
public:
    static Trluper::AbstractFactory* GetSingle();
    virtual Trluper::Connections* CreateAllObjWhenAccept(int _dataFd) override;
    void setFApplycationObj(HTTP_REQUEST_TYPE key,Trluper::FApplycations* app);
private:
    static Trluper::AbstractFactory* FactorySingle;
    Trluper::Router<HTTP_REQUEST_TYPE,HttpRequestWrapper*>* m_router = nullptr;
};

Trluper::AbstractFactory* HttpFactory::FactorySingle = new HttpFactory(new Trluper::Router<HTTP_REQUEST_TYPE,HttpRequestWrapper*>());
HttpFactory::HttpFactory(Trluper::Router<HTTP_REQUEST_TYPE,HttpRequestWrapper*>* router):m_router(router)
{
}

HttpFactory::~HttpFactory()
{
    if(m_router != nullptr){
        delete m_router;
    }
}

Trluper::AbstractFactory *HttpFactory::GetSingle()
{
    return FactorySingle;
}

Trluper::Connections *HttpFactory::CreateAllObjWhenAccept(int _dataFd)
{
    HttpConnection* conn = new HttpConnection(_dataFd);//8/12
    HttpDataprocess* process = new HttpDataprocess();//4/12
    process->setRouterObj(m_router);
    process->setConnectionsObj(conn);
    conn->setDataProcessObj(process);
    return conn;
}

inline void HttpFactory::setFApplycationObj(HTTP_REQUEST_TYPE key, Trluper::FApplycations *app)
{
    m_router->addProcessor(key,app);
}

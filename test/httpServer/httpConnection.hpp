#pragma once

#include <exception>
#include "connections.h"
#include "httpDataProcess.h"
#include "log.h"

class HttpConnection:public Trluper::Connections
{
public:
    HttpConnection();
    HttpConnection(Trluper::DataProcess* process);
    ~HttpConnection();
    void setDataProcessObj(Trluper::DataProcess* process);
    virtual Trluper::DataProcess* GetDataProcessObj(Trluper::Message& msg)override;
    
private:
    Trluper::DataProcess* m_process = nullptr;
};

HttpConnection::HttpConnection()
{
}

inline HttpConnection::HttpConnection(Trluper::DataProcess *process)
{
    m_process = process;
}

HttpConnection::~HttpConnection()
{
    if(m_process != nullptr){
        delete m_process;
    }
}

inline Trluper::DataProcess *HttpConnection::GetDataProcessObj(Trluper::Message &msg)
{
    return m_process;
}

inline void HttpConnection::setDataProcessObj(Trluper::DataProcess *process)
{
    m_process = process;
}

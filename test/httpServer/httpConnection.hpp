#pragma once

#include <exception>
#include "connections.h"
#include "httpDataProcess.hpp"
#include "log.h"

class HttpConnection:public Trluper::Connections
{
public:
    HttpConnection(int _dataFd);
    HttpConnection(int _dataFd,Trluper::DataProcess* process);
    ~HttpConnection();
    void setDataProcessObj(Trluper::DataProcess* process);
    virtual Trluper::DataProcess* GetDataProcessObj(Trluper::Message& msg)override;
    
private:
    Trluper::DataProcess* m_process = nullptr;
};

HttpConnection::HttpConnection(int _dataFd):Connections(_dataFd)
{
}

inline HttpConnection::HttpConnection(int _dataFd, Trluper::DataProcess *process)
    :Connections(_dataFd), m_process(process)
{
    
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

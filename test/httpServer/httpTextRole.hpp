#pragma once

#include <fstream>
#include <sstream>
#include <ctime>
#include "httpConnection.hpp"
#include "fapplycations.h"
#include "httpRequest.hpp"
#include "httpDataProcess.hpp"
#include "server.h"
#include "http.h"


class TextRole:public Trluper::FApplycations
{
public:
    TextRole();
    virtual ~TextRole();
protected:
    //文本类型业务处理
    Trluper::Request* textProc(Trluper::Request& _request);
    //处理消息类型： REQUEST_TYPE_ECHO = 1的函数入口，如果需要下一个处理者处理，那么就会返回处理好的Request*
    virtual Trluper::Request* ProcRequest(Trluper::Request& _request)override;
    //下一个业务处理者处理
    virtual Trluper::FApplycations* GetNextApplycationsObj(Trluper::AbstractMsg& msg)override;
};

TextRole::TextRole(){

}

TextRole::~TextRole(){

}

Trluper::Request* TextRole::textProc(Trluper::Request& _request){
    DYNAMIC_GETREF(HttpRequestWrapper,node,_request);
    HttpResponseWrapper* response = new HttpResponseWrapper();
    Trluper::HttpResponse::ptr _response(new Trluper::HttpResponse());
    std::string& path = node->m_request->m_path;
    std::string TEXT_FILE_PATH;
    std::ifstream textFile;
    std::stringstream buffer;
    if(path == "/"){
        TEXT_FILE_PATH = "/home/project/Trluper/source/welcome.html";
    }
    else if(path == "/selection.html"){
        TEXT_FILE_PATH = "/home/project/Trluper/source/selection.html";
    }
    else if(path == "/image.html"){
        TEXT_FILE_PATH = "/home/project/Trluper/source/image.html";
    }
    else if(path == "/video.html"){
        TEXT_FILE_PATH = "/home/project/Trluper/source/video.html";
    }
    else if(path == "/download.html"){
        TEXT_FILE_PATH = "/home/project/Trluper/source/download.html";
    }
    textFile.open(TEXT_FILE_PATH);
    buffer << textFile.rdbuf();
    _response->m_body = buffer.str();
    _response->m_version = node->m_request->m_version;
    _response->m_keepalive = false;
    _response->m_headers["Content-Type"] = "text/html";
    _response->m_status = Trluper::HttpStatus::OK;
    _response->m_headers["Date"] = Trluper::Time2Str(std::time(nullptr),"%a, %d %b %Y %H:%M:%S")+" GMT";
    response->m_response = _response;
    node->m_process->m_conn->SetConnectionClose();
    Trluper::Server::ServerUseHandleOfDataProcess(*(node->m_process),response);
    delete node;
    return nullptr;
}

Trluper::Request* TextRole::ProcRequest(Trluper::Request& _request){
    textProc(_request);
    return nullptr;
}

Trluper::FApplycations* TextRole::GetNextApplycationsObj(Trluper::AbstractMsg& msg){
    return nullptr;
}
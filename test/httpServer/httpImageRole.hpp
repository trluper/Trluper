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


class ImageRole:public Trluper::FApplycations
{
public:
    ImageRole();
    virtual ~ImageRole();
protected:
    //文本类型业务处理
    Trluper::Request* imageProc(Trluper::Request& _request);
    //处理消息类型： REQUEST_TYPE_ECHO = 1的函数入口，如果需要下一个处理者处理，那么就会返回处理好的Request*
    virtual Trluper::Request* ProcRequest(Trluper::Request& _request)override;
    //下一个业务处理者处理
    virtual Trluper::FApplycations* GetNextApplycationsObj(Trluper::AbstractMsg& msg)override;
};

ImageRole::ImageRole(){

}

ImageRole::~ImageRole(){

}

Trluper::Request* ImageRole::imageProc(Trluper::Request& _request){
    DYNAMIC_GETREF(HttpRequestWrapper,node,_request);
    HttpResponseWrapper* response = new HttpResponseWrapper();
    Trluper::HttpResponse::ptr _response(new Trluper::HttpResponse());
    response->m_response = _response;
    std::string& path = node->m_request->m_path;
    std::string TEXT_FILE_PATH;
    std::ifstream textFile;
    std::stringstream buffer;
    if(path == "/1.png"){
        TEXT_FILE_PATH = "/home/project/Trluper/source/1.png";
    }
    else if(path == "/2.png"){
        TEXT_FILE_PATH = "/home/project/Trluper/source/2.png";
    }
    else if(path == "/3.png"){
        TEXT_FILE_PATH = "/home/project/Trluper/source/3.png";
    }
    else if(path == "/4.png"){
        TEXT_FILE_PATH = "/home/project/Trluper/source/4.png";
    }
    else if(path == "/5.png"){
        TEXT_FILE_PATH = "/home/project/Trluper/source/5.png";
    }
    textFile.open(TEXT_FILE_PATH,std::ios::binary);
    if(!textFile.is_open()){
        LOG_SS_ERROR(LOG_GET_MANAGER->getMainLogger())<<"Error: Unable to open the file.";
        _response->m_status = Trluper::HttpStatus::NOT_FOUND;
    }else{
        buffer << textFile.rdbuf();
        _response->m_body = buffer.str();
        _response->m_headers["Content-Type"] = "image/png";
        _response->m_status = Trluper::HttpStatus::OK;
        _response->m_headers["Date"] = Trluper::Time2Str(std::time(nullptr),"%a, %d %b %Y %H:%M:%S")+"GMT";
    }
    node->m_process->m_conn->SetConnectionClose();
    Trluper::Server::ServerUseHandleOfDataProcess(*(node->m_process),response);
    delete node;
    return nullptr;
}

Trluper::Request* ImageRole::ProcRequest(Trluper::Request& _request){
    imageProc(_request);
    return nullptr;
}

Trluper::FApplycations* ImageRole::GetNextApplycationsObj(Trluper::AbstractMsg& msg){
    return nullptr;
} 
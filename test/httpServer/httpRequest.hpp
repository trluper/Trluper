#pragma once

#include <list>
#include "http.h"
#include "httpDataProcess.hpp"
#include "httpRequestParser.h"
#include "message.h"

class HttpDataprocess;

enum HTTP_REQUEST_TYPE{
    TEXT        = 1,        //文本文件类型：html\css\javascript\xml\plain
    IMAGE       = 2,        //图片文件类型：png/jpeg/jpg/gif/svg
    AUDIO       = 3,        //音频文件类型：MP3/ogg/wav
    VIDEO       = 4,        //视频文件类型：MP4/OGG/WEBM/quickTime
    APPLICATION = 5         //压缩文件类型：zip/tar/gzip
};

class HttpRequestWrapper:public Trluper::Request{
public:    
    HttpRequestWrapper(HttpDataprocess* process);
    virtual ~HttpRequestWrapper();
    template<typename T=HTTP_REQUEST_TYPE> T getRequestType(){return m_type;}
public:
    //识别请求类型
    HTTP_REQUEST_TYPE m_type;
    //http请求封装
    Trluper::HttpRequest::ptr m_request;
    //协议层对象
    HttpDataprocess* m_process=nullptr;
};


class HttpResponseWrapper:public Trluper::Request{
public:
    HttpResponseWrapper();
    virtual ~HttpResponseWrapper();
public:
    Trluper::HttpResponse::ptr m_response;
};


HttpRequestWrapper::HttpRequestWrapper(HttpDataprocess* process):m_process(process){

}

HttpRequestWrapper::~HttpRequestWrapper(){

}

HttpResponseWrapper::HttpResponseWrapper(){

}
HttpResponseWrapper::~HttpResponseWrapper(){
    
}
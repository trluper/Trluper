#pragma once

#include <list>
#include "http.h"
#include "httpRequestParser.h"
#include "message.h"

enum HTTP_REQUEST_TYPE{
    TEXT        = 1,        //文本文件类型：html\css\javascript\xml\plain
    IMAGE       = 2,        //图片文件类型：png/jpeg/jpg/gif/svg
    AUDIO       = 3,        //音频文件类型：MP3/ogg/wav
    VIDEO       = 4,        //视频文件类型：MP4/OGG/WEBM/quickTime
    APPLICATION = 5         //压缩文件类型：zip/tar/gzip
};

class HttpRequestWrapper:public Trluper::Request{
public:    
    HttpRequestWrapper(HTTP_REQUEST_TYPE _type, Trluper::HttpRequest::ptr _request);
    virtual ~HttpRequestWrapper();
private:
    //识别请求类型
    HTTP_REQUEST_TYPE m_type;
    //http请求封装
    Trluper::HttpRequest::ptr m_request;
};

class ListHttpRequest: public Trluper::Request{
public:
    std::list<Request*> m_lreqeust;
};

HttpRequestWrapper::HttpRequestWrapper(HTTP_REQUEST_TYPE _type, 
    Trluper::HttpRequest::ptr _request):m_type(_type),m_request(_request){

}

HttpRequestWrapper::~HttpRequestWrapper(){

}
#include "httpRequestParser.h"

namespace Trluper{

static Logger::ptr logger = nullptr;

bool Trluper::HttpRequestParser::RegexParserHttpRequest(const std::string &request, HttpRequest &httpRequest)
{
    logger = LOG_GET_MANAGER->getMainLogger();
    //请求行的正则表达式匹配串
    //w+匹配一个或多个字母、数字或下划线，()捕获匹配内容，后续一样
    std::regex requestLineParser(R"((\w+) (\S+) HTTP/(\d+\.\d+))");
    std::smatch match;
    if(std::regex_search(request,match,requestLineParser)) {
        std::string method = match[1];
        httpRequest.m_method = StringToHttpMethod(method);
        httpRequest.m_path = match[2];
        httpRequest.m_version = match[3];
        
    }else{
        LOG_SS_ERROR(logger) << "Invalid HTTP request line";
        return false;
    }

    //请求头匹配
    //[\w-]+匹配一个或多个字母、数字、下划线或者-，()捕获匹配内容
    std::regex headerRegex(R"(([\w-]+): ([^\r\n]+))");
    auto headerBegin = std::sregex_iterator(request.begin(),request.end(),headerRegex);
    auto headerEnd = std::sregex_iterator();
    for(std::sregex_iterator it=headerBegin; it != headerEnd; ++it){
        const std::string key = (*it)[1];
        const std::string value = (*it)[2];
        //是否是Cookie
        if(caseInsensitiveCompare("Cookie",key)){
            std::regex cookieRegex(R"(([^=\s]+)=([^;]+)(?:(;|( ;)|(; )|$)))");
            auto cookieBegin = std::sregex_iterator(value.begin(),value.end(),cookieRegex);
            auto cookieEnd = std::sregex_iterator();
            for(std::sregex_iterator cookieIt = cookieBegin; cookieIt != cookieEnd; ++cookieIt) {
                 httpRequest.m_cookies[(*cookieIt)[1]] = (*cookieIt)[2];
            }
        }
        else if(caseInsensitiveCompare("Connection",key)){
            if(caseInsensitiveCompare("Close",value)) httpRequest.m_keepalive = false;
            else httpRequest.m_keepalive = true;
        }
        else{
            httpRequest.m_headers[key] = value;
        }
    }
    // 查找并提取查询参数
    std::regex queryParamRegex(R"((\w+)=(\S+?)(?:(&|#|$)))");
    auto queryParamBegin = std::sregex_iterator(httpRequest.m_path.begin(), httpRequest.m_path.end(), queryParamRegex);
    auto queryParamEnd = std::sregex_iterator();
    for (std::sregex_iterator it = queryParamBegin; it != queryParamEnd; ++it) {
        httpRequest.m_queries[(*it)[1]] = (*it)[2];
    }

    // 查找并提取锚片段
    std::smatch fragmentMatch;
    std::regex_search(request, fragmentMatch, std::regex("#([^\\s]*)"));
    if (fragmentMatch.size() > 1) {
        httpRequest.m_fragment = fragmentMatch[1];
    }

    // 查找并提取请求体
    std::smatch bodyMatch;
    std::regex_search(request, bodyMatch, std::regex("\r\n\r\n(.*)"));
    if (bodyMatch.size() > 1) {
        httpRequest.m_body = bodyMatch[1];
    }
    size_t loc = httpRequest.m_path.find_first_of('?');
    httpRequest.m_path.erase(loc);
    return true;
}

bool HttpRequestParser::FSMParserHttpRequest(const std::string &request, HttpRequest &_httpRequest)
{
    logger = LOG_GET_MANAGER->getMainLogger();
    const char* _request = request.c_str();
    return FSMParserHttpRequest(_request, _httpRequest, request.size());
}

bool HttpRequestParser::FSMParserHttpRequest(const char *request, HttpRequest &_httpRequest, size_t len)
{
    logger = LOG_GET_MANAGER->getMainLogger();
    char* _request_begin = const_cast<char*>(request);
    HttpRequestStatus FSMStatus = HttpRequestStatus::START;
    size_t bodyLen  = 0,index=0;
    while(index < len && FSMStatus != HttpRequestStatus::INVALID 
            && FSMStatus != HttpRequestStatus::COMPLETE){
        char c = *_request_begin;
        char* currentPos=_request_begin++;
        size_t pos = index++;
        switch (FSMStatus)
        {
            //!START状态
            case HttpRequestStatus::START: {
                if(isblank(c) || c == _CR || c == _LF) {
                    continue;
                }
                else if(isupper(c)) {
                    m_method.begin = currentPos;
                    FSMStatus = HttpRequestStatus::METHOD;
                }
                else{
                    FSMStatus = HttpRequestStatus::INVALID;
                }
                break;
            }
            //!解析方法
            case HttpRequestStatus::METHOD: {
                if(isupper(c)) {
                    continue;
                }
                else if(isblank(c)) {
                    //方法解析完成
                    m_method.end = currentPos;
                    _httpRequest.m_method = StringToHttpMethod(m_method);
                    FSMStatus = HttpRequestStatus::BEFORE_PATH;
                }
                else {
                    FSMStatus = HttpRequestStatus::INVALID;
                }
                break;
            }
            //!解析路径（去除前导空格）
            case HttpRequestStatus::BEFORE_PATH: {
                if(isblank(c)) {
                    continue;
                }
                else if('/' == c) {
                    m_path.begin = currentPos;
                    FSMStatus = HttpRequestStatus::PATH;
                }
                else{
                    FSMStatus = HttpRequestStatus::INVALID;
                }
                break;
            }
            //!解析路径
            case HttpRequestStatus::PATH: {
                if('?' == c) {
                    m_path.end = currentPos;
                    _httpRequest.m_path = m_path;
                    FSMStatus = HttpRequestStatus::BEFORE_QUERY;
                }
                else if(isblank(c)) {
                    m_path.end = currentPos;
                    _httpRequest.m_path = m_path;
                    FSMStatus = HttpRequestStatus::BEFORE_PROTOCOL;
                }
                else if(_CR == c || _LF == c) {
                    FSMStatus = HttpRequestStatus::INVALID;
                }
                break;
            }
            //!查询参数解析(判断非法情况)
            case HttpRequestStatus::BEFORE_QUERY: {
                if(isblank(c) || _CR == c || _LF == c) {
                    FSMStatus = HttpRequestStatus::INVALID;
                }
                else {
                    m_query_key.begin = currentPos;
                    FSMStatus = HttpRequestStatus::QUERY_KEY;
                }
                break;
            }
            //!查询参数key的解析
            case HttpRequestStatus::QUERY_KEY: {
                if(isblank(c) || _CR == c || _LF == c) {
                    FSMStatus = HttpRequestStatus::INVALID;
                }
                else if('=' == c) {
                    m_query_key.end = currentPos;
                    FSMStatus = HttpRequestStatus::BEFORE_QUERY_VALUE;
                }
                break;
            }
            //!查询参数的value解析
            case HttpRequestStatus::BEFORE_QUERY_VALUE: {
                if(isblank(c) || _CR == c || _LF == c) {
                    FSMStatus = HttpRequestStatus::INVALID;
                }
                else {
                    m_query_value.begin = currentPos;
                    FSMStatus = HttpRequestStatus::QUERY_VALUE;
                }
                break;
            }
            //!查询参数的value解析
            case HttpRequestStatus::QUERY_VALUE: {
                if(_CR == c || _LF == c) {
                    FSMStatus = HttpRequestStatus::INVALID;
                }
                else if(isblank(c)) {
                    m_query_value.end = currentPos;
                    _httpRequest.m_queries.insert({m_query_key,m_query_value});
                    FSMStatus = HttpRequestStatus::BEFORE_PROTOCOL;
                }
                else if('&' == c) {
                    m_query_value.end = currentPos;
                    _httpRequest.m_queries.insert({m_query_key,m_query_value});
                    FSMStatus = HttpRequestStatus::BEFORE_QUERY;
                }
                break;
            }
            //!协议解析
            case HttpRequestStatus::BEFORE_PROTOCOL: {
                if(isblank(c)) {
                    continue;
                }
                else if(_CR == c || _LF == c) {
                    FSMStatus = HttpRequestStatus::INVALID;
                }
                else {
                    FSMStatus = HttpRequestStatus::PROTOCOL;
                }
                break;
            }
            //!协议解析
            case HttpRequestStatus::PROTOCOL: {
                if(_CR == c || _LF == c) {
                    FSMStatus = HttpRequestStatus::INVALID;
                }
                else if('/' == c) {
                    FSMStatus = HttpRequestStatus::BEFORE_VERSION;
                }
                break;
            }
            //!协议版本解析（
            case HttpRequestStatus::BEFORE_VERSION: {
                if('0' <= c && '9' >= c) {
                    m_version.begin = currentPos;
                    FSMStatus = HttpRequestStatus::VERSION;
                }
                else{
                    FSMStatus = HttpRequestStatus::INVALID;
                }
                break;
            }
            //!协议版本解析（默认正确）
            case HttpRequestStatus::VERSION: {
                if(_CR == c) {
                    m_version.end = currentPos;
                    _httpRequest.m_version = m_version;
                    FSMStatus = HttpRequestStatus::CR;
                }
                break;
            }

            case HttpRequestStatus::CR: {
                if(_LF == c) {
                    FSMStatus = HttpRequestStatus::CRLF;
                }
                else {
                    FSMStatus = HttpRequestStatus::INVALID;
                }
                break;
            }
            case HttpRequestStatus::CRLF: {
                if(_CR == c) {
                    FSMStatus = HttpRequestStatus::CRLF_CR;
                }
                else if(isblank(c)) {
                    FSMStatus = HttpRequestStatus::INVALID;
                }
                else{
                    m_header_key.begin = currentPos;
                    FSMStatus = HttpRequestStatus::HEADER_KEY;
                }
                break;
            }
            case HttpRequestStatus::HEADER_KEY: {
                if(isblank(c)) {
                    m_header_key.end = currentPos;
                    FSMStatus = HttpRequestStatus::HEADER_BEFOR_COLON;
                }
                else if(':' == c) {
                    m_header_key.end = currentPos;
                    FSMStatus = HttpRequestStatus::HEADER_AFTER_COLON;
                }
                break;
            }
            case HttpRequestStatus::HEADER_BEFOR_COLON: {
                if(isblank(c)) {
                    continue;
                }
                else if(':' == c) {
                    FSMStatus = HttpRequestStatus::HEADER_AFTER_COLON;
                }
                else{
                    FSMStatus = HttpRequestStatus::INVALID;
                }
                break;
            }
            case HttpRequestStatus::HEADER_AFTER_COLON: {
                if(isblank(c)) {
                    continue;
                }
                else{
                    m_header_value.begin = currentPos;
                    FSMStatus = HttpRequestStatus::HEADER_VALUE;
                }
                break;
            }
            case HttpRequestStatus::HEADER_VALUE:{
                if(_CR == c) {
                    m_header_value.end = currentPos;
                    FSMStatus = HttpRequestStatus::CR;
                    if(caseInsensitiveCompare("Cookie",m_header_key)) {
                       //解析Cookie
                       if(!cookieParser(m_header_value, _httpRequest)) {
                            FSMStatus = HttpRequestStatus::INVALID;
                       }
                    }
                    else {
                        _httpRequest.m_headers.insert({m_header_key,m_header_value});
                    }
                }
                break;
            }
            case HttpRequestStatus::CRLF_CR: {
                if(_LF == c) {
                    if(_httpRequest.m_headers.count("Content-Length") > 0) {
                        bodyLen = std::stoi(_httpRequest.m_headers["Content-Length"]);
                        FSMStatus = (bodyLen>0) ? HttpRequestStatus::BODY : HttpRequestStatus::COMPLETE;
                    }
                    else {
                        if(pos < len) {
                            bodyLen = len - pos;
                            FSMStatus = HttpRequestStatus::BODY;
                        }else{
                            FSMStatus = HttpRequestStatus::COMPLETE;
                        }
                    }
                }
                else {
                    FSMStatus = HttpRequestStatus::INVALID;
                }
            }
            case HttpRequestStatus::BODY: {
                _httpRequest.m_body.assign(currentPos,bodyLen);
                FSMStatus = HttpRequestStatus::COMPLETE;
                if(_httpRequest.m_headers.count("Connection") > 0) {
                    _httpRequest.m_keepalive = caseInsensitiveCompare("keep-alive",_httpRequest.m_headers["Connection"]);
                }
                break;
            }
            case HttpRequestStatus::COMPLETE:
                break;
            case HttpRequestStatus::INVALID:
                break;
        }
    }
    return FSMStatus==HttpRequestStatus::COMPLETE ? true:false;
}

bool HttpRequestParser::cookieParser(const std::string &request, HttpRequest &_httpRequest)
{
    CookieStatus cookieStatus = CookieStatus::COOKIE_STATR;
    size_t len = request.size(),index = 0;
    StringBuffer key,value;
    char* cookie_begin = const_cast<char*>(request.c_str());
    while(index < len && cookieStatus != CookieStatus::COOKIE_COMPLETE) {
        char c = *cookie_begin;
        char* currentPos = cookie_begin++;
        ++index;
        switch (cookieStatus)
        {
            case CookieStatus::COOKIE_STATR: {
                if(isblank(c)) {
                    continue;
                }
                else {
                    key.begin = currentPos;
                    cookieStatus = CookieStatus::COOKIE_KEY;
                }
                break;
            }
            case CookieStatus::COOKIE_KEY: {
                if('=' == c || index >= len) {
                    key.end = currentPos;
                    cookieStatus = CookieStatus::COOKIE_BEFOR_VALUE;
                }
                break;
            }
            case CookieStatus::COOKIE_BEFOR_VALUE: {
                if(isblank(c)) {
                    return false;
                }
                else {
                    value.begin = currentPos;
                    cookieStatus = CookieStatus::COOKIE_VALUE;
                }
                break;
            }
            case CookieStatus::COOKIE_VALUE: {
                if(';' == c){
                    value.end = currentPos;
                    _httpRequest.m_cookies.insert({key,value});
                    cookieStatus = CookieStatus::COOKIE_STATR;
                    break;
                }
                else if(index == len) {
                    cookieStatus = CookieStatus::COOKIE_COMPLETE;
                }
                else {
                    break;
                }
            }
            case CookieStatus::COOKIE_COMPLETE: {
                value.end = cookie_begin;
                _httpRequest.m_cookies.insert({key,value});
                break;
            }
        }
    }
    return true;
}

std::shared_ptr<HttpRequestParser> HttpRequestParser::getSingle()
{
    std::call_once(singleFlag,[&](){
        parserSingle = std::shared_ptr<HttpRequestParser>(new HttpRequestParser());
    });
    return parserSingle;
}

}
 
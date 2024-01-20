#ifndef _HTTPREQUESTPARSER_H_
#define _HTTPREQUESTPARSER_H_

#include <algorithm>
#include <regex>
#include <string>
#include <iostream>
#include <vector>
#include <mutex>
#include "http.h"
#include "log.h"

#define _CR '\r'
#define _LF '\n'

namespace Trluper{
/*
* http有限状态机的解析
*   method path?query/r/n
*   HEADER_KEY: HEADER_VALUE/r/n
*   /r/n
*   body
*/
enum class HttpRequestStatus{
    START               = 1,
    METHOD              = 2,
    BEFORE_PATH         = 3,
    PATH                = 4,
    BEFORE_QUERY        = 5,
    QUERY_KEY           = 6,
    BEFORE_QUERY_VALUE  = 7,
    QUERY_VALUE         = 8,
    BEFORE_PROTOCOL     = 9,
    PROTOCOL            = 10,
    BEFORE_VERSION      = 11,
    VERSION             = 12,
    CR                  = 13,
    CRLF                = 14,
    HEADER_KEY          = 15,
    HEADER_BEFOR_COLON  = 16,
    HEADER_AFTER_COLON  = 17,
    HEADER_VALUE        = 18,
    CRLF_CR             = 19,
    BODY                = 20,
    COMPLETE            = 21,
    INVALID             = 22
};

enum class CookieStatus {
    COOKIE_STATR        = 1,
    COOKIE_KEY          = 2,
    COOKIE_BEFOR_VALUE  = 3,
    COOKIE_VALUE        = 4,
    COOKIE_COMPLETE     = 5
};

struct StringBuffer {
    char *begin = NULL;//字符串开始位置
    char *end = NULL;//字符串结束位置

    operator std::string() const {
        return std::string(begin, end);
    }
};


class HttpRequestParser{
public:
    typedef std::shared_ptr<HttpRequestParser> ptr;
public:
    
    /// @brief 正则表达解析http请求
    /// @param request http请求报文
    /// @param httpRequest 存储解析后的结果
    /// @return 解析是否成功
    bool RegexParserHttpRequest(const std::string& request, HttpRequest& httpRequest);  //3704505us/10000条
    
    /// @brief 有限状态机解析http请求
    /// @param request http请求报文
    /// @param HttpRequest 存储解析后的结果
    /// @return 解析是否成功
    bool FSMParserHttpRequest(std::string& request, HttpRequest& _httpRequest);   //46651us/10000条
    size_t FSMParserHttpRequest(const char* request, HttpRequest& _httpRequest, size_t len);
    static std::shared_ptr<HttpRequestParser> getSingle();
    ~HttpRequestParser() = default;
private:
    HttpRequestParser() = default;
    HttpRequestParser(const HttpRequestParser& obj) = default;
    HttpRequestParser& operator=(const HttpRequestParser& obj) = default;
    HttpRequestParser& operator=(HttpRequestParser&& obj) = default;
    bool cookieParser(const std::string& request, HttpRequest& _httpRequest);
};
//懒汉模式实现
static std::shared_ptr<HttpRequestParser> parserSingle;
static std::once_flag  singleFlag;
}
#endif
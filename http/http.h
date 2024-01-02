/*
http.h:对http的请求、响应包进行封装
*/
#ifndef _HTTP_H_
#define _HTTP_H_

#include <algorithm>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <cctype>
#include <string.h>
#include "util.h"  
namespace Trluper{

//HTTP状态宏
/*
1XX:这一类型的状态码，代表请求已被接受，需要继续处理。这类响应是临时响应，只包含状态行和某些可选的响应头信息，并以空行结束
2XX:这一类型的状态码，代表请求已成功被服务器接收、理解、并接受
3XX:这类状态码代表需要客户端采取进一步的操作才能完成请求。通常，这些状态码用来重定向，后续的请求地址（重定向目标）在本次响应的Location域中指明
4XX:这类的状态码代表了客户端看起来可能发生了错误，妨碍了服务器的处理。除非响应的是一个HEAD请求，否则服务器就应该返回一个解释当前错误状况的实体，以及这是临时的还是永久性的状况
5XX:表示服务器无法完成明显有效的请求。[56]这类状态码代表了服务器在处理请求的过程中有错误或者异常状态发生，也有可能是服务器意识到以当前的软硬件资源无法完成对请求的处理
*/
#define HTTP_STATUS_MAP(XX)                                                 \
  XX(100, CONTINUE,                        Continue)                        \
  XX(101, SWITCHING_PROTOCOLS,             Switching Protocols)             \
  XX(102, PROCESSING,                      Processing)                      \
  XX(200, OK,                              OK)                              \
  XX(201, CREATED,                         Created)                         \
  XX(202, ACCEPTED,                        Accepted)                        \
  XX(203, NON_AUTHORITATIVE_INFORMATION,   Non-Authoritative Information)   \
  XX(204, NO_CONTENT,                      No Content)                      \
  XX(205, RESET_CONTENT,                   Reset Content)                   \
  XX(206, PARTIAL_CONTENT,                 Partial Content)                 \
  XX(207, MULTI_STATUS,                    Multi-Status)                    \
  XX(208, ALREADY_REPORTED,                Already Reported)                \
  XX(226, IM_USED,                         IM Used)                         \
  XX(300, MULTIPLE_CHOICES,                Multiple Choices)                \
  XX(301, MOVED_PERMANENTLY,               Moved Permanently)               \
  XX(302, FOUND,                           Found)                           \
  XX(303, SEE_OTHER,                       See Other)                       \
  XX(304, NOT_MODIFIED,                    Not Modified)                    \
  XX(305, USE_PROXY,                       Use Proxy)                       \
  XX(307, TEMPORARY_REDIRECT,              Temporary Redirect)              \
  XX(308, PERMANENT_REDIRECT,              Permanent Redirect)              \
  XX(400, BAD_REQUEST,                     Bad Request)                     \
  XX(401, UNAUTHORIZED,                    Unauthorized)                    \
  XX(402, PAYMENT_REQUIRED,                Payment Required)                \
  XX(403, FORBIDDEN,                       Forbidden)                       \
  XX(404, NOT_FOUND,                       Not Found)                       \
  XX(405, METHOD_NOT_ALLOWED,              Method Not Allowed)              \
  XX(406, NOT_ACCEPTABLE,                  Not Acceptable)                  \
  XX(407, PROXY_AUTHENTICATION_REQUIRED,   Proxy Authentication Required)   \
  XX(408, REQUEST_TIMEOUT,                 Request Timeout)                 \
  XX(409, CONFLICT,                        Conflict)                        \
  XX(410, GONE,                            Gone)                            \
  XX(411, LENGTH_REQUIRED,                 Length Required)                 \
  XX(412, PRECONDITION_FAILED,             Precondition Failed)             \
  XX(413, PAYLOAD_TOO_LARGE,               Payload Too Large)               \
  XX(414, URI_TOO_LONG,                    URI Too Long)                    \
  XX(415, UNSUPPORTED_MEDIA_TYPE,          Unsupported Media Type)          \
  XX(416, RANGE_NOT_SATISFIABLE,           Range Not Satisfiable)           \
  XX(417, EXPECTATION_FAILED,              Expectation Failed)              \
  XX(421, MISDIRECTED_REQUEST,             Misdirected Request)             \
  XX(422, UNPROCESSABLE_ENTITY,            Unprocessable Entity)            \
  XX(423, LOCKED,                          Locked)                          \
  XX(424, FAILED_DEPENDENCY,               Failed Dependency)               \
  XX(426, UPGRADE_REQUIRED,                Upgrade Required)                \
  XX(428, PRECONDITION_REQUIRED,           Precondition Required)           \
  XX(429, TOO_MANY_REQUESTS,               Too Many Requests)               \
  XX(431, REQUEST_HEADER_FIELDS_TOO_LARGE, Request Header Fields Too Large) \
  XX(451, UNAVAILABLE_FOR_LEGAL_REASONS,   Unavailable For Legal Reasons)   \
  XX(500, INTERNAL_SERVER_ERROR,           Internal Server Error)           \
  XX(501, NOT_IMPLEMENTED,                 Not Implemented)                 \
  XX(502, BAD_GATEWAY,                     Bad Gateway)                     \
  XX(503, SERVICE_UNAVAILABLE,             Service Unavailable)             \
  XX(504, GATEWAY_TIMEOUT,                 Gateway Timeout)                 \
  XX(505, HTTP_VERSION_NOT_SUPPORTED,      HTTP Version Not Supported)      \
  XX(506, VARIANT_ALSO_NEGOTIATES,         Variant Also Negotiates)         \
  XX(507, INSUFFICIENT_STORAGE,            Insufficient Storage)            \
  XX(508, LOOP_DETECTED,                    Loop Detected)                   \
  XX(510, NOT_EXTENDED,                    Not Extended)                    \
  XX(511, NETWORK_AUTHENTICATION_REQUIRED, Network Authentication Required) \

//HTTP的请求方法
#define HTTP_METHOD_MAP(XX)         \
  XX(0,  DELETE,      DELETE)       \
  XX(1,  GET,         GET)          \
  XX(2,  HEAD,        HEAD)         \
  XX(3,  POST,        POST)         \
  XX(4,  PUT,         PUT)          \
  /* pathological */                \
  XX(5,  CONNECT,     CONNECT)      \
  XX(6,  OPTIONS,     OPTIONS)      \
  XX(7,  TRACE,       TRACE)        \
  /* WebDAV */                      \
  XX(8,  COPY,        COPY)         \
  XX(9,  LOCK,        LOCK)         \
  XX(10, MKCOL,       MKCOL)        \
  XX(11, MOVE,        MOVE)         \
  XX(12, PROPFIND,    PROPFIND)     \
  XX(13, PROPPATCH,   PROPPATCH)    \
  XX(14, SEARCH,      SEARCH)       \
  XX(15, UNLOCK,      UNLOCK)       \
  XX(16, BIND,        BIND)         \
  XX(17, REBIND,      REBIND)       \
  XX(18, UNBIND,      UNBIND)       \
  XX(19, ACL,         ACL)          \
  /* subversion */                  \
  XX(20, REPORT,      REPORT)       \
  XX(21, MKACTIVITY,  MKACTIVITY)   \
  XX(22, CHECKOUT,    CHECKOUT)     \
  XX(23, MERGE,       MERGE)        \
  /* upnp */                        \
  XX(24, MSEARCH,     M-SEARCH)     \
  XX(25, NOTIFY,      NOTIFY)       \
  XX(26, SUBSCRIBE,   SUBSCRIBE)    \
  XX(27, UNSUBSCRIBE, UNSUBSCRIBE)  \
  /* RFC-5789 */                    \
  XX(28, PATCH,       PATCH)        \
  XX(29, PURGE,       PURGE)        \
  /* CalDAV */                      \
  XX(30, MKCALENDAR,  MKCALENDAR)   \
  XX(31, LINK,        LINK)         \
  XX(32, UNLINK,      UNLINK)       \
  XX(33, SOURCE,      SOURCE)       \


//http状态枚举类
enum class HttpStatus{
#define XX(Status,Name,String) Name=Status,
    HTTP_STATUS_MAP(XX)
#undef XX
};

//http方法枚举类
enum class HttpMethod{
#define XX(Code,Name,String) Name=Code,
    HTTP_METHOD_MAP(XX)
#undef XX
    INVALID_METHOD
};

/// @brief 获得Http方法名称的对应枚举量
/// @param m Http方法名称(sting)\const char*
/// @return HttpMethod
HttpMethod StringToHttpMethod(const std::string& name);
HttpMethod StringToHttpMethod(const char* name);

/// @brief 获得对应http方法的字符串名称
/// @param method 方法枚举量
/// @return const char*
const char* HttpMethodToString(const HttpMethod& method);
const char* HttpStatusToString(const HttpStatus& status);

/// @brief 不区分大小写
struct CaseInsensitiveCompare {
    bool operator()(const std::string& str1, const std::string& str2) const {
        return std::lexicographical_compare(
            str1.begin(), str1.end(), str2.begin(), str2.end(),
            [](char c1, char c2) {
                return std::tolower(c1) < std::tolower(c2);
            }
        );
    }
};

inline bool caseInsensitiveStringCompare(char c1, char c2) {
    return std::toupper(c1) == std::toupper(c2);
}

inline bool caseInsensitiveCompare(const std::string&& str1, const std::string& str2) {
    return str1.size() == str2.size() &&
           std::equal(str1.begin(), str1.end(), str2.begin(), caseInsensitiveStringCompare);
}

struct HttpResponse;
/// @brief http的请求结构体（封装请求信息）
struct HttpRequest{
public:
    typedef std::shared_ptr<HttpRequest> ptr;
    typedef std::map<std::string,std::string,CaseInsensitiveCompare> Map;
public:
    /// @brief 构造函数
    /// @param version http的版本号，默认http/1.1
    /// @param keepalive 是否保持连接
    HttpRequest(std::string version = "1.1", bool keepalive = true);
    std::ostream& httpStreamRequest(std::ostream& os)const;
    std::string httpStringRequest()const;    
public:
    /*
    uri格式：protocol://[username:password@]hostname[:post][/path][[?query][#fragment]
    */
    HttpMethod m_method;        //http请求方法
    bool m_keepalive;           //是否保持连接
    std::string m_version;      //http的版本号
    std::string m_path;         //所需文件的路径
    std::string m_fragment;     //锚片段
    std::string m_body;         //请求的消息体
    Map m_queries;              //请求行的查询参数(请求资源时提高额外信息)
    Map m_headers;              //请求头map映射
    Map m_cookies;              //cookies的map映射
};

/// @brief http的响应结构体（封装响应信息）
struct HttpResponse{
public:
    typedef std::shared_ptr<HttpResponse> ptr;
    typedef std::map<std::string,std::string,CaseInsensitiveCompare> Map;
public:
    HttpResponse(std::string version = "1.1", bool keepalive = true);
    void setCookies(const std::string &name, const std::string &val, 
                time_t expired, const std::string &path, const std::string &domain, 
                bool secure);
    std::string httpStringResponse()const;
    /// @brief 流式得到响应报文
    /// @param os 
    /// @return ostream&
    std::ostream& httpStreamResponse(std::ostream& os)const;
public:
    HttpStatus m_status;    //http的响应状态
    bool m_keepalive;       //是否保持连接
    std::string m_version;      //http版本，前4位存储高、后四位存储低
    std::string m_body;     //响应的消息体
    std::string m_reason;   //响应原因
    Map m_headers;          //响应头
    std::vector<std::string> m_cookies; //Set-Cookie:Set-Cookie: name=val; expires=***; path=*; domain=*; secure
};

}
#endif
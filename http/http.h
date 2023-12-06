#ifndef _HTTP_H_
#define _HTTP_H_

#include <memory>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>
namespace Trluper{

//HTTP状态
/*
1XX:这一类型的状态码，代表请求已被接受，需要继续处理。这类响应是临时响应，只包含状态行和某些可选的响应头信息，并以空行结束
2XX:这一类型的状态码，代表请求已成功被服务器接收、理解、并接受
3XX:这类状态码代表需要客户端采取进一步的操作才能完成请求。通常，这些状态码用来重定向，后续的请求地址（重定向目标）在本次响应的Location域中指明
4XX:这类的状态码代表了客户端看起来可能发生了错误，妨碍了服务器的处理。除非响应的是一个HEAD请求，否则服务器就应该返回一个解释当前错误状况的实体，以及这是临时的还是永久性的状况
5XX:表示服务器无法完成明显有效的请求。[56]这类状态码代表了服务器在处理请求的过程中有错误或者异常状态发生，也有可能是服务器意识到以当前的软硬件资源无法完成对请求的处理
*/
enum class HttpStatus{
    CONTINUE = 100,
    //切换传输协议         
    SWITCHING_PROTOCOLS = 101,
    //该代码表示​​服务器已经收到并正在处理请求，但无响应可用
    PROCESSING = 102,
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NOW_AUTHORITATIVE_INFORMATION = 203,
    NO_CONTENT = 204,
    RESET_CONTENT = 205,
    PARTIAL_CONTENT = 206,
    MULTI_STATUS = 207,
    ALREADY_REPORTED = 208,
    IM_USED = 226,
    MULTIPLE_CHOICES = 300,
    MOVED_PERMANENTLY = 301,
    FOUND = 302,
    SEE_OTHER = 303,
    NOT_MIDIFIED = 304,
    USE_PROXY = 305,
    SWITCH_PROXY = 306,
    TEMPORARY_REDIRECT = 307,
    PERMANENT_REDIRECT = 308,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    PAYMENT_REQUIRED = 402,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOR_NOT_ALLOWED = 405,
    NOT_ACCEPTABLE = 406,
    PROXY_AUTHENTICATION_REQUIRED = 407,
    REQUEST_TIMEOUT = 408,
    CONFLICT = 409,
    GONE = 410,
    LENGTH_REQUIRED = 411,
    PRECONDITION_FAILED = 412,
    REQUEST_ENTITY_TOO_LARGE = 413,
    REQUEST_URI_TOO_LARGE = 414,
    UNSUPPORTED_MEDIA_TYPE = 415,
    REQUEST_RANGE_NOT_SATISFIABLE = 416,
    EXPECTATION_FAILED = 417,
    MISDIRECTED_REQUEST = 421,
    UNPROCESSABLE_ENTITY = 422,
    LOCKED = 423,
    FAILED_DEPENDENCY = 424,
    UNORDERED_COLLECTION = 425,
    UPGRADE_REQUIRED = 426,
    PRECONDITION_REQUIRED = 428,
    TOO_MANY_REQUEST = 429,
    REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
    NO_RESPONSE = 444,
    UNAVAILABLE_FOR_LEGAL_REASONS = 451,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505,
    VARIANT_ALSO_NEGOTIATES = 506,
    INSUFFICIENT_STORAGE = 507,
    LOOP_DETECTED = 508,
    NOT_EXTENDED = 510
};


//HTTP的请求方法
enum class HttpMethod{
    DELETE = 0,
    GET = 1,
    HEAD = 2,
    POST = 3,
    PUT = 4,
    CONNECT = 5,
    OPTIONS = 6,
    TRACE = 7,
    COPY = 8,
    LOCK = 9,
    MKCOL = 10,
    MOVE = 11,
    PROPFIND = 12,
    PROPPATCH = 13,
    SEARCH = 14,
    UNLOCK = 15,
    BIND = 16,
    REBIND = 17,
    UNBIND = 18,
    ACL = 19,
    REPORT = 20,
    MKACTIVITY = 21,
    CHECKOUT = 22,
    MERGE = 23,
    MSEARCH = 24,
    NOTIFY = 25,
    SUBSCRIBE = 26,
    UNSUBSCRIBE = 27,
    PATCH = 28,
    PURGE = 29,
    MKCALENDAR = 30,
    LINK = 31,
    UNLINK = 32,
    SOURCE = 33
};

}
#endif
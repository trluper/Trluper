#include "http.h"

namespace Trluper{
HttpMethod StringToHttpMethod(const std::string &name)
{
#define XX(Code, Name, String) \
    if(strcmp(#Name,name.c_str()) == 0){ \
        return HttpMethod::Name; \
    }
    HTTP_METHOD_MAP(XX);
#undef XX
    return HttpMethod::INVALID_METHOD;
}

HttpMethod StringToHttpMethod(const char *name)
{
#define XX(Code, Name, String) \
    if(strcmp(#Name,name) == 0){ \
        return HttpMethod::Name; \
    }
    HTTP_METHOD_MAP(XX);
#undef XX
    return HttpMethod::INVALID_METHOD;
}

const char *HttpMethodToString(const HttpMethod &method)
{
    switch (method)
    {
#define XX(Code, Name, String) \
        case HttpMethod::Name:  \
            return #Name;
        HTTP_METHOD_MAP(XX);
#undef XX
        default:
            return "<unknown>";
            break;
    }
}

const char *HttpStatusToString(const HttpStatus &status)
{
    switch (status)
    {
#define XX(Status, Name, String) \
        case HttpStatus::Name: \
            return #Name;
        HTTP_STATUS_MAP(XX);
#undef XX
    default:
        return "<unknown>";
    }
}

//!HttpResponse
HttpResponse::HttpResponse(std::string version, bool keepalive):
        m_status(HttpStatus::OK),m_keepalive(keepalive),m_version(version)
{

}
std::string HttpResponse::httpStringResponse() const
{
    std::stringstream ss;
    httpStreamResponse(ss);
    return ss.str();
}
std::ostream &HttpResponse::httpStreamResponse(std::ostream &os) const
{
   os << "HTTP/" << m_version
      <<" "
      <<(uint32_t)m_status
      <<" "
      <<(m_reason.empty()? HttpStatusToString(m_status) : m_reason)
      << "\r\n";
    
    for(auto& header : m_headers){
        if(strcasecmp(header.first.c_str(),"Connection") == 0){
            continue;
        }
        os << header.first << ": " << header.second<< "\r\n";
    }
    
    for(auto& cookie : m_cookies){
        os << "Set-Cookie: " << cookie <<"\r\n";
    }
    os << "Connection: " << (m_keepalive? "keep-alive" : "close") << "\r\n";
    if(!m_body.empty()){
        os << "Content-Length: " << m_body.size() << "\r\n\r\n" << m_body;
    }else{
        os << "\r\n";
    }
    return os;
}

void HttpResponse::setCookies(const std::string &name, const std::string &val, 
    time_t expired, const std::string &path, const std::string &domain, 
    bool secure)
{
    std::stringstream ss;
    ss << name << "=" << val;
    if(expired > 0){
        ss << ";expires=" << Trluper::Time2Str(expired,"%a, %d %b %Y %H:%M:%S") << " GMT";
    }
    if(!domain.empty()){
        ss << ";domain=" << domain;
    }
    if(!path.empty()){
        ss << ";path=" << path;
    }
    if(secure){
        ss << ";secure";
    }
    m_cookies.push_back(ss.str());
}

//!HttpRequest
HttpRequest::HttpRequest(std::string version, bool keepalive):
    m_method(HttpMethod::GET),m_keepalive(keepalive),m_version(version),
    m_path("/")
{

}


std::ostream &HttpRequest::httpStreamRequest(std::ostream &os) const
{
    os << HttpMethodToString(m_method) << " "<< m_path;
    if(!m_queries.empty()){
        os << "?";
        bool first=true;
        for(auto&query:m_queries){
            os << (first ? "" : "&")
                << query.first << "=" << query.second;
            first=false;
        }
    }
    os<< (m_fragment.empty() ? "" : "#")
       << m_fragment
       << " HTTP/"
       << m_version
       << "\r\n";
    os << "Connection: " << (m_keepalive ? "keep-alive" : "close") << "\r\n";
    for(auto& header : m_headers) {
        if( strcasecmp(header.first.c_str(), "Connection") == 0) {
            continue;
        }
        os << header.first << ": " << header.second << "\r\n";
    }
    if(!m_cookies.empty()){
        os << "Cookie: ";
        bool first = true;
        for(auto& cookie : m_cookies) {
            if(!first) os << "; ";
            os << cookie.first << "="<<cookie.second;
            first = false;
        }
        os << "\r\n";
    }
    if(!m_body.empty()) {
        os << "Content-Length: " << m_body.size() << "\r\n\r\n"
           << m_body;
    } else {
        os << "\r\n";
    }
    return os;
}

std::string HttpRequest::httpStringRequest() const
{
    std::stringstream ss;
    httpStreamRequest(ss);
    return ss.str();
}



}
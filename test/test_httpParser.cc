#include <chrono>
#include "httpRequestParser.h"

int main(int argc, char* argv[]){
    
    std::string request("POST /audiolibrary/music?ar=1595301089068&n=1p1 HTTP/1.1\r\n"
    "Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, application/x-silverlight, application/x-shockwave-flash\r\n"
    "Referer: http://www.google.cn\r\n"
    "Accept-Language: zh-cn\r\n"
    "Accept-Encoding: gzip, deflate\r\n"
    "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; TheWorld)\r\n"
    "content-length:27\r\n"
    "Host: www.google.cn\r\n"
    "Connection: Close\r\n"
    "Cookie: PREF=ID=80a06da87be9ae3c:U=f7167333e2c3b714:NW=1:TM=1261551909:LM=1261551917:S=ybYcq2wpfefs4V9g; NID=31=ojj8d-IygaEtSxLgaJmqSjVhCspkviJrB6omjamNrSm8lZhKy_yMfO2M4QMRKcH1g0iQv9u\r\n"
    "\r\n"
    "hl=zh-CN&source=hp&q=domety");
    Trluper::HttpRequestParser::ptr p=Trluper::HttpRequestParser::getSingle();
    Trluper::HttpRequest req;
    // 获取当前时间点（开始时间）
    auto start_time = std::chrono::high_resolution_clock::now();
    for(int i=0;i<10000;++i){
        //p->RegexParserHttpRequest(request,req);
        /*if(p->RegexParserHttpRequest(request,req)){
            std::cout<<req.httpStringRequest()<<std::endl;
        }*/
        p->FSMParserHttpRequest(request,req);
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;

}
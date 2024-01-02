#include "http.h"

int main(){
    Trluper::HttpRequest::ptr req(new Trluper::HttpRequest());
    req->m_headers["Request URL"] = "http://baidu.com";
    req->m_cookies["BDUSS_BFESS"] = "UNjOE4zOEdYNnRGMH5mLThYQ3JkR2ExdmZaTzExaUFFNVotNHZYZk9PcEhCeEJsSVFBQUFBJCQAAAAAAAAAAAEAAADpY99tddK7yMgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEd66GRHeuhkO";
    req->m_body="Hello Http!";
    std::string ret = req->httpStringRequest();
    std::cout<<ret<<std::endl;
}
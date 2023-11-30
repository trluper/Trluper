#ifndef _STRINGEXCEPTION_H_
#define _STRINGEXCEPTION_H_

#include <exception>
#include <string>
#include <iostream>

namespace Trluper{


class LogException:public std::exception{
public:
    LogException(std::string msg,const char* filename,uint32_t line):m_msg(msg),m_fileName(filename),m_line(line){}
    const std::string getMsg()const {return m_msg;}
    friend std::ostream& operator<<(std::ostream& os, LogException& obj);
private:
    std::string m_msg;
    const char* m_fileName;
    uint32_t m_line;
};

//定义为内联函数，否则会出现多重定义
inline std::ostream& operator<<(std::ostream& os, LogException& obj){
    os<<obj.m_msg<<" located in: "<< obj.m_fileName<< " line:" << obj.m_line;
    return os;
}

}

#endif
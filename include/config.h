#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <json/json.h>
#include <fstream>
#include <iostream>
#include <string>
#include <exception>
//配置文件config读取
namespace Trluper{
    inline const Json::Value getConfig(std::string& path){
        std::ifstream configFile(path);
        if(!configFile){
            throw::std::exception();
            exit(-1);
        }
        //Json对象
        Json::Value root;
        configFile >> root;
        configFile.close();
        std::cout<<"The configuration file was read successfully."<<std::endl;
        std::string serverHost = root["ip"].asString();
        int serverPort = root["port"].asInt();
        int threadNums = root["thread_nums"].asInt();
        int maxHandle = root["max_handle"].asInt();
        std::cout<<"Configure the following server attributes:"<<std::endl;
        std::cout<<"Server Host: "<<serverHost<<std::endl;
        std::cout<<"Server port: "<<serverPort<<std::endl;
        std::cout<<"Number of child thread is: "<<threadNums<<std::endl;
        std::cout<<"Request cache maximum is: "<<maxHandle<<std::endl;
        return root;
    }
}

#endif
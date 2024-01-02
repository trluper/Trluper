#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <json/json.h>
#include <fstream>
#include <iostream>
#include <string>
#include <exception>
#include "log.h"
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
        int read_buffer_size = root["read_buffer_size"].asInt();
        int write_buffer_size = root["write_buffer_size"].asInt();
        std::string MySQLHost = root["mysql_host"].asString();
        std::string MySQLUser = root["mysql_user"].asString();
        std::string MySQLPassword = root["mysql_password"].asString();
        int MySQLPort = root["mysql_port"].asInt();
        std::string MySQLDatabase = root["mysql_database"].asString();
        //载入log的配置初始化
        Trluper::LoggerManager* manager = LOG_GET_MANAGER;
        std::cout<<"Configure the following server attributes:"<<std::endl;
        std::cout<<"--------------------Server Configuration--------------------"<<std::endl;
        std::cout<<"Server Host: "<<serverHost<<std::endl;
        std::cout<<"Server port: "<<serverPort<<std::endl;
        std::cout<<"Number of child thread is: "<<threadNums<<std::endl;
        std::cout<<"Request cache maximum is: "<<maxHandle<<std::endl;
        std::cout<<"Read Buffer size is: "<<read_buffer_size<<"Byte"<<std::endl;
        std::cout<<"Write Buffer size is: "<<write_buffer_size<<"Byte"<<std::endl;
        std::cout<<"--------------------MySQL Configuration--------------------"<<std::endl;
        std::cout<<"MySQL Host: "<<MySQLHost<<std::endl;
        std::cout<<"MySQL Port: "<<MySQLPort<<std::endl;
        std::cout<<"MySQL Username: "<<MySQLUser<<std::endl;
        std::cout<<"MySQL Password: "<<MySQLPassword<<std::endl;
        std::cout<<"MySQL Database: "<<MySQLDatabase<<std::endl;
        std::cout<<"--------------------Log Configuration----------------------"<<std::endl;
        std::cout<<manager->toYAMLString()<<std::endl;
        
        return root;
    }

    inline const Json::Value getConfig(std::string&& path){
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
        int read_buffer_size = root["read_buffer_size"].asInt();
        int write_buffer_size = root["write_buffer_size"].asInt();
        std::string MySQLHost = root["mysql_host"].asString();
        std::string MySQLUser = root["mysql_user"].asString();
        std::string MySQLPassword = root["mysql_password"].asString();
        int MySQLPort = root["mysql_port"].asInt();
        std::string MySQLDatabase = root["mysql_database"].asString();
        //载入log的配置初始化
        Trluper::LoggerManager* manager = LOG_GET_MANAGER;
        std::cout<<"Configure the following server attributes:"<<std::endl;
        std::cout<<"--------------------Server Configuration--------------------"<<std::endl;
        std::cout<<"Server Host: "<<serverHost<<std::endl;
        std::cout<<"Server port: "<<serverPort<<std::endl;
        std::cout<<"Number of child thread is: "<<threadNums<<std::endl;
        std::cout<<"Request cache maximum is: "<<maxHandle<<std::endl;
        std::cout<<"Read Buffer size is: "<<read_buffer_size<<"Btye"<<std::endl;
        std::cout<<"Write Buffer size is: "<<write_buffer_size<<"Byte"<<std::endl;
        std::cout<<"--------------------MySQL Configuration--------------------"<<std::endl;
        std::cout<<"MySQL Host: "<<MySQLHost<<std::endl;
        std::cout<<"MySQL Port: "<<MySQLPort<<std::endl;
        std::cout<<"MySQL Username: "<<MySQLUser<<std::endl;
        std::cout<<"MySQL Password: "<<MySQLPassword<<std::endl;
        std::cout<<"MySQL Database: "<<MySQLDatabase<<std::endl;
        std::cout<<"--------------------Log Configuration----------------------"<<std::endl;
        std::cout<<manager->toYAMLString()<<std::endl;
        
        return root;
    }
}

#endif
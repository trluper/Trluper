/*
 * @Author: wuwenjie wenjiewu2017@163.com
 * @Date: 2023-09-05 20:31:44
 * @LastEditors: wuwenjie wenjiewu2017@163.com
 * @LastEditTime: 2023-09-14 16:51:04
 * @FilePath: /Trluper/TEST/main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include "log.h"
#include "factory.h"
#include "timer.h"
#include "thread.h"


int main(){
    
    //C/S回显测试代码
    std::string configPath = "/home/project/Trluper/config/config.json";
    Trluper::AbstractFactory* factorySingle = Factory::GetSingle();
    Trluper::Server::ServerExceptionStop();
    Trluper::Server::ServerInit(configPath,factorySingle);
    Trluper::Server::ServerRun();
    
}

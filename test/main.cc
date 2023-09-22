/*
 * @Author: wuwenjie wenjiewu2017@163.com
 * @Date: 2023-09-05 20:31:44
 * @LastEditors: wuwenjie wenjiewu2017@163.com
 * @LastEditTime: 2023-09-14 16:51:04
 * @FilePath: /Trluper/TEST/main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "Factory.h"





int main(){
    AbstractFactory* factorySingle = Factory::GetSingle();
    Server::ServerInit("172.29.249.90",8080,factorySingle);
    Server::ServerExceptionStop();
    Server::ServerRun();
    exit(0);
}
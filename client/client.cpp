/*
 * @Author: wuwenjie wenjiewu2017@163.com
 * @Date: 2023-09-10 15:20:43
 * @LastEditors: wuwenjie wenjiewu2017@163.com
 * @LastEditTime: 2023-09-14 17:14:02
 * @FilePath: /Trluper/client/client.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE#
 */
#include "client.h"

int main(){
    std::string serverip("172.20.25.130");
    int port=8080;
    client _client(serverip,port);
    _client.run();
}



#ifndef _CONNECTIONS_H_
#define _CONNECTIONS_H_

#include "abstractHandle.h"
#include <list>
#include <memory>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <iostream>
#include "server.h"
#include "dataProcess.h"

namespace Trluper{

class Server;
class DataProcess;
class UserMsg;
/*
* @brief:当一个连接到来时就会创建Connections对象，与相应的dataFd执行绑定，后续的监听到读写事件会转到Connections对象执行。一般开发者只需要重写类内的GetDataProcessObj虚函数即可和添加一个数据处理对象指针成员
* @memberof dataFd:与dataFd绑定
* @memberof NeedClose：判断这个连接是否被客户端关闭了
* @memberof writeBuffer：当服务器要发送数据时，会将数据存储在这里，这里充当服务器的写缓存
*/
class Connections : public AbstractHandle{
public:
    Connections(int _dataFd);
    virtual ~Connections();
    //获得当前连接的文件描述符
    int GetFd(){return dataFd;}
    
    void SetIP(char* _ip);
    const char* GetIP()const{return ip;}
    //由Server调用，进行写回，内部调用writeFd
    void FlushOut();
    //当writebBuffer空时，调用HashOutPut说明已经有数据要发送，此时要告诉Server的可写缓存区变化：有不可写变为可写，来触发epoll_wait
    bool HashOutPut(){return false==writerBuffer.empty();}
    //当readFd返回值为0时，就会调用该函数，NeedClose置true，执行关闭
    void SetConnectionClose(){NeedClose=true;}
    //返回NeedClose的状态
    bool ConnectionNeedClose(){return NeedClose;}
    //!得到数据层对象，开发者需重写这个函数，返回一个DataProcess类型对象指针
    virtual DataProcess* GetDataProcessObj(Message& msg) = 0;
protected:
    //当writerBuffer有数据时，会调用writeFd将数据写回给客户端
    bool writeFd(std::string& _output);
    //从dataFd中读取数据
    bool ReadFd(std::string& _input);
    //当前连接层的处理逻辑在此实现，对于IN返回的是Message对象，对于OUT，将消息写入writerBuffer，返回nullptr
    virtual AbstractMsg* currentHandle(AbstractMsg& msg)override;
    //位于连接层，因此只处理IN的下一个处理者，对于OUT没有下一个处理者
    virtual AbstractHandle* GetNextHanlder(AbstractMsg& msg)override;
protected:
    int dataFd;                             //绑定dataFD
    char ip[16];                            //ip地址
    bool NeedClose = false;                 //指示当前dataFd的开关状态,通知Server应该关闭这个连接
    std::list<std::shared_ptr<std::string>> writerBuffer;    //当需要主动发送信息给客户端时，这里作为写缓存存储数据
    Message* _msg_;                           //维护一个堆式的读缓存,客户端断开连接后释放
}; 

}
#endif

#ifndef _SERVER_H_
#define _SERVER_H_


#include <list>
#include <iostream>
//epoll和socket头文件

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <iostream>
#include <future>
#include <signal.h>
#include <unordered_map>
#include "threadPool.h"
#include "connections.h"


/*
 * @Author: wuwenjie wenjiewu2017@163.com
 * @Date: 2023-09-14 20:11:48
 * @LastEditors: wuwenjie wenjiewu2017@163.com
 * @LastEditTime: 2023-09-14 22:07:07
 */

/*
!v0.1版本
*Server应该以单例的模式运行，因此应该只提供实例化一个服务器对象,因此提供静态方法
*/

namespace Trluper{

class Connections;
class DataProcess;
class AbstractFactory;
class UserMsg;

class Server{
public:
    /*初始化一个服务器单例对象，提供服务的开始，内部其实纠错创建绑定监听socket、并将socker上树*/
    static Server* ServerInit(std::string&& ip,int&& port, AbstractFactory* _singleFactory);
    /*信号的捕捉，有时候我们停止服务器是在发生异常或者输入ctrl+c:SIGINT、ctrl+z:SIGSTOP、ctrl+\:SIGQUIT造成服务器非正常退出
    ，无法执行析构函数，因此需要捕捉这些信号来执行析构*/
    static void ServerExceptionStop();
    /*停止服务器，在进程退出前建议调用该函数，回收相关的资源*/
    static void ServerStop(int signal,siginfo_t* info,void* context);
    /*开始运行服务器*/
    static void ServerRun();
    /*断开一个连接*/
    static void ServerDelConn(struct epoll_event& ev);
    /*设置server_exit的状态*/
    static void ServerExit();
    /*
    设置dataFd由不可写状态变为可写状态，该静态函数是在连接层writeBuffer由没数据变为有数据时触发
    *static void ServerSetConectionOut(Connections& conn);
    在写完后，重新设置dataFd由可写变为不可写状态
    *static void ServerUnSetConnectionOut(Connections& conn);
    */
    /*当业务应用层处理完后，应该调用数据层的额Handle函数进行响应，该函数在业务层处理后调用，由开发者调用*/
    static void ServerUseHandleOfDataProcess(DataProcess& process,Request* request);

private:
    //构造、拷贝构造、赋值私有化
    Server(std::string ip,int port,AbstractFactory* _singleFactory);
    Server(const Server& s){}
    const Server& operator=(const Server& s){return *this;}
    ~Server();
    bool init();
    void run();
    //listenfd调用该函数，执行accept、创建Connctions与dataFd绑定、dataFd上树
    void ctlAcceptFd();
    //当客户端关闭时会调用此函数
    void ctlCloseFd(struct epoll_event& ev);
    //epoll句柄
    int epollfd = -1;
    //listenfd的文件描述符
    int listenfd = -1;
    //serveraddr存储服务器的ip地址、端口号
    struct sockaddr_in serveraddr;
    //判断server的状态，当为true时，说明需要关闭服务器，那么执行ServerStop操作
    bool server_exit=false;
    //管理每个dataFd绑定的Connections对象
    std::unordered_map<int,Connections*> m_ConnctionsMap;
    //服务器单例对象
    static Server* singleServer;
    //工厂的单例对象，需要开发者初始化
    AbstractFactory* singleFactory;

};


//抽象工厂，开发者要继承实现这个抽象工厂
class AbstractFactory{
protected:
    AbstractFactory(){}
    AbstractFactory(const AbstractFactory& obj){}
    AbstractFactory& operator=(const AbstractFactory& obj){}
public:
    virtual ~AbstractFactory(){};
    //开发者需要重写的函数，一个是覆盖静态函数GetSingle，一个是重写虚函数CreateAllObjWhenAccept
    static AbstractFactory* GetSingle(){}
    virtual Connections* CreateAllObjWhenAccept(int _dataFd) = 0;
};

}
#endif

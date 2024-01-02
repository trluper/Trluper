//Trluper框架 
#include "server.h"
namespace Trluper{
    
Server* Server::singleServer = nullptr;
LoggerManager* Server::logger_manager = nullptr;
Logger::ptr Server::logger = nullptr;
std::size_t Server::read_buffer_size = 1024;
std::size_t Server::write_buffer_size = 2024;

/// @brief 调用构造函数Server()和Init(),创建服务器socket，并执行绑定、监听功能，调用init()进行epoll上树操作
/// @param ip ：ip地址
/// @param port ：端口号
Server *Server::ServerInit(std::string& path , AbstractFactory* _singleFactory,bool multiThread)
{
    if(nullptr==singleServer){
        singleServer->logger_manager = LOG_GET_MANAGER;
        singleServer->logger = logger_manager->getLogger("root");
        //完美转发
        singleServer = new Server(path,_singleFactory,multiThread);//9/12
        //日志器
        Trluper::LogAppender::ptr appender_error(new FileLogAppender("../Log/root_error.txt"));
        appender_error->setLogLevel(LogLevel::ERROR);
        Trluper::LogAppender::ptr appender_info(new FileLogAppender("../Log/root_info.txt"));
        appender_info->setLogLevel(LogLevel::INFO);
        logger->addAppender(appender_info);
        logger->addAppender(appender_error);
    }
    std::cout<<"--------------------Server's Log Configuration-------------"<<std::endl;
    std::cout<<LOG_GET_MANAGER->toYAMLString()<<std::endl;
    std::cout<<"--------------------Running Log info-----------------------"<<std::endl;
    return singleServer;
}

void Server::ServerExceptionStop()
{
    struct sigaction SigEvent;
    SigEvent.sa_sigaction = ServerStop;
    SigEvent.sa_flags = SA_NOCLDWAIT|SA_NOCLDSTOP;
    sigaction(SIGINT,&SigEvent,NULL);
    sigaction(SIGQUIT,&SigEvent,NULL);
    sigaction(SIGSTOP,&SigEvent,NULL);
    sigaction(SIGTSTP,&SigEvent,NULL);
}

/// @brief 停止服务器前应该调用的函数，此时需要回收资源，内部调用end函数
void Server::ServerStop(int signal,siginfo_t* info,void* context)
{
    singleServer->~Server();
    delete singleServer;
    exit(0);
}



void Server::ServerRun()
{
    if(nullptr!=singleServer){
        singleServer->run();
    }else{
        LOG_SS_ERROR(logger)<<"ServerRun failed, the value of singleServer is nullptr."<<std::endl;
        exit(1);
    }
}

void Server::ServerDelConn(epoll_event &ev)
{
    if(nullptr!=singleServer) singleServer->ctlCloseFd(ev);
}

void Server::ServerExit()
{
    if(nullptr!=singleServer) singleServer->server_exit=true;
}

void Server::ServerAccept()
{
    if(nullptr!=singleServer) singleServer->ctlAcceptFd();
}

int Server::ServerGetListenFd()
{
    return singleServer->listenfd;
}

#ifdef _OLD_CODE_

void Server::ServerSetConectionOut(Connections &conn)
{
    int dataFd = conn.GetFd();
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
    ev.data.ptr = &conn;
    //!若不注释，这里会出现神奇的bug，dataFd的赋值会导致ptr地址值产生变化
    //ev.data.fd = dataFd;
    epoll_ctl(singleServer->epollfd,EPOLL_CTL_MOD,dataFd,&ev);

}

void Server::ServerUnSetConnectionOut(Connections &conn)
{
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = &conn;
    //ev.data.fd = conn.GetFd();
    epoll_ctl(singleServer->epollfd,EPOLL_CTL_MOD,conn.GetFd(),&ev);
}
#endif

void Server::ServerUseHandleOfDataProcess(DataProcess &process, Request* request)
{
    UserMsg* _UMsg = new UserMsg(IO_Direction::OUT);
    _UMsg->SetRequest(request);
    process.Handle(*_UMsg);
}


/// @brief 创建服务器socket，并执行绑定、监听功能，调用init()进行epoll上树操作
/// @param ip ：ip地址
/// @param port ：端口号
Server::Server(std::string& path, AbstractFactory* _singleFactory,bool multiThread):singleFactory(_singleFactory)
{
    const Json::Value& root = getConfig(path);
    Server::read_buffer_size = root["read_buffer_size"].asInt();
    Server::write_buffer_size = root["write_buffer_size"].asInt();
    std::string ip = root["ip"].asString();
    int port = root["port"].asInt();
    const char* _ip=ip.c_str();
    max_handle = root["max_handle"].asInt();

    int sokcetfd=socket(AF_INET,SOCK_STREAM,0);
    if(0<=sokcetfd){
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin_port=htons(port);
        serveraddr.sin_family=AF_INET;
        inet_pton(AF_INET,_ip,&serveraddr.sin_addr.s_addr);
        //设置地址复用,确保服务器在time_wait也能重启
        int opt=1;
        setsockopt(sokcetfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
        if(0==bind(sokcetfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr))){
            if(0==listen(sokcetfd,10)){
                listenfd=sokcetfd;
                init();
            }
            else{
                LOG_SS_ERROR(logger)<<"Server start error: Listen."<<std::endl;
                exit(1);
            }
        }else{
            LOG_SS_ERROR(logger)<<"Server start error: Bind."<<std::endl;
            exit(1);
        }

    }else{
        LOG_SS_ERROR(logger)<<"Server start error: Get socket failed"<<std::endl;
        exit(1);
    }
    
    if(true == multiThread){
        threadPool = new ThreadPool(root["thread_nums"].asInt(),max_handle);
    }   
}

Server::~Server()
{
    //释放工厂对象
    delete singleFactory;
    //释放连接
    auto it=m_ConnctionsMap.begin();
    while(it!=m_ConnctionsMap.end()){
        close(it->first);
        delete it->second;
        m_ConnctionsMap.erase(it->first);
        it=m_ConnctionsMap.begin();
    }
    if(0<=epollfd){
        close(epollfd);
    }
    close(listenfd);
    if(nullptr != threadPool){
        delete threadPool;
    }
    std::cout<<"GoodBye!"<<std::endl;
}   

/// @brief 执行epoll句柄创建和listenfd的上树操作
/// @return 成功返回true，失败返回false
bool Server::init()
{
    epollfd=epoll_create(1);
    bool bRet=false;
    struct epoll_event listenev;
    //listenfd使用默认的水平触发
    listenev.events=EPOLLIN|EPOLLOUT;
    listenev.data.fd=listenfd;
    if(0==epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&listenev)){
        bRet=true;
    }else{
        LOG_SS_ERROR(logger)<<"Server start error: Epoll init."<<std::endl;
        exit(1);
    }
    return bRet;
}

/// @brief 启动服务器
void Server::run()
{
    while(false==server_exit){
        struct epoll_event hashChange[max_handle];
        //阻塞监听
        int iEpollRet = 0;
        //!这里有一个bug，每次accept后都会再一次触发epoll_wait，这是因为datafd采用边缘触发，刚建立连接，datafd由不可写变为可写就会出现这种二次触发现象(已修正)
        iEpollRet = epoll_wait(epollfd, hashChange, max_handle, -1);
        //!V0.3版本
        for(int i=0;i<iEpollRet;++i){
#ifdef _SIGLETHREAD_
            //*listenfd读事件
            if(hashChange[i].data.fd==listenfd&&hashChange[i].events&EPOLLIN){ 
                ctlAcceptFd();
            }
            //*dataFd读事件，服务器epoll_wait只获取读事件，写事件在处理完请求后，直接响应发送
            else if(hashChange[i].events&EPOLLIN){
                IOState state(IO_Direction::IN);
                Connections* conn =(Connections*)hashChange[i].data.ptr;
                conn->Handle(state);
                //判断客户端的连接状态
                if(conn->ConnectionNeedClose()==true){
                    Server::ServerDelConn(hashChange[i]);
                }
            }
#else
            while(false == threadPool->pushTask(&hashChange[i]));
#endif
#ifdef _OLD_CODE_
            //*dataFd写事件,服务器主动发送时就会触发
            else{
                IOState state(IO_Direction::OUT);
                Connections* conn =(Connections*)hashChange[i].data.ptr;
                conn->FlushOut();
                if(false == conn->HashOutPut()) ServerUnSetConnectionOut(*conn);

            }
#endif
        }
    }
}



/// @brief 对epoll_wait监听到的建立连接请求，进行上树
/// @param dataFd 建立链接后的文件描述符
void Server::ctlAcceptFd()
{ 
    struct sockaddr_in clientSocket;
    int dataFd = -1;
    socklen_t lAddrlen=sizeof(clientSocket);
    dataFd = accept(listenfd,(struct sockaddr*)&clientSocket,&lAddrlen);
    if(dataFd>=0){
        char ip[16];
        inet_ntop(AF_INET,&clientSocket.sin_addr.s_addr,ip,16);
        LOG_SS_INFO(logger)<<"新的客户端连接,ip："<<ip<<std::endl;
        std::cout<<"<------------------------------------------------------------->"<<std::endl;
        //创建相应的连接层、数据层和业务层对象
        Connections* conn = singleFactory->CreateAllObjWhenAccept(dataFd);
        if(nullptr!=conn){
            m_ConnctionsMap[dataFd] = conn; //3/12
            struct epoll_event client;
            //存储ip
            conn->SetIP(ip);
            //边缘触发+EPOLLIN,EPOLLOUT后续要send的时候添加，使epoll_wait能够触发写操作
            client.events=EPOLLIN|EPOLLET;
            //*client.data.ptr可以作为dataFd的绑定通道
            client.data.ptr = conn;
            if(-1==epoll_ctl(epollfd,EPOLL_CTL_ADD,dataFd,&client)){
                LOG_SS_ERROR(logger)<<"epoll_ctl error: client"<<std::endl;
                perror("epoll_ctl error: client");
            }
        }
        //创建conn失败，向客户端发送“Unknow error!”，直接close
        else{
            char buf[]="Unknow error!";
            send(dataFd,buf,sizeof(buf),MSG_DONTWAIT);
            close(dataFd);
        }     
    }else{
        LOG_SS_ERROR(logger)<<"accept failed."<<std::endl;
        perror("accept:");
    }
    
}

/// @brief 监听到客户的关闭信息，执行epoll下树操作和被动关闭socket
/// @param dataFd:需要关闭的套接字
void Server::ctlCloseFd(struct epoll_event& ev)
{
    Connections* conn = (Connections*)ev.data.ptr;
    LOG_SS_INFO(logger)<<"客户端ip："<<conn->GetIP()<<"已断开链接"<<std::endl;
    m_ConnctionsMap.erase(ev.data.fd);
    epoll_ctl(epollfd,EPOLL_CTL_DEL,ev.data.fd,&ev);
    close(ev.data.fd);
    //释放堆区
    delete conn;
}
}
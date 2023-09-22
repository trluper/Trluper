#include <sys/socket.h>
#include <sys/types.h>
#include <sys/bitypes.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <iostream>
#define messageLen 1024

class client{
public:
    client(std::string _ip,int port):_serverIP(_ip),_port(port){
        _clientFd=socket(AF_INET,SOCK_STREAM,0);
        addr.sin_family = AF_INET;
        addr.sin_port = htons(_port);
        const char* ip=_serverIP.c_str(); 
        inet_pton(AF_INET,ip,&addr.sin_addr.s_addr);   
    }
    void run(){
        int ero = connect(_clientFd,(struct sockaddr*)&addr,sizeof(addr));
        if(0!=ero){
            perror("connect:");
        }
        while(true){
            std::string sin("");
            int id = 0;
            std::cin>>id;
            std::cin>>sin;
            sin = serilize(id,sin);
            int len=sin.size();
            //!bug：buf不能用new、malloc分配，这样可能使得介收到的数据为空
            char buf[messageLen] = {0};
            sin.copy(buf,len);
            sin.clear();
            //flags=0为阻塞
            send(_clientFd,buf,len,0);
            memset(buf,0,len);
            int l = recv(_clientFd,buf,messageLen,0);
            sin.append(buf,l);
            std::cout<<sin<<std::endl;
        }
    }

private:
    std::string serilize(int& id,std::string str);
    std::string unserilize(std::string str);

private:
    struct sockaddr_in addr;
    std::string _serverIP;
    int _port;
    int _clientFd;
};
std::string client::serilize(int& id, std::string str)
{
    std::string s("");
    int lenght = str.size();
    s.push_back((id>>0) & 0xff);
    s.push_back((id>>8) & 0xff);
    s.push_back((id>>16) & 0xff);
    s.push_back((id>>24) & 0xff);
    s.push_back((lenght>>0) & 0xff);
    s.push_back((lenght>>8) & 0xff);
    s.push_back((lenght>>16) & 0xff);
    s.push_back((lenght>>24) & 0xff);
    s.append(str);
    return s;
}

inline std::string client::unserilize(std::string str)
{
    return "";
}

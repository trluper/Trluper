/*
 * @Author: wuwenjie wenjiewu2017@163.com
 * @Date: 2023-09-11 22:18:37
 * @LastEditors: wuwenjie wenjiewu2017@163.com
 * @LastEditTime: 2023-09-15 22:37:04
 */
#include "connections.h"

namespace Trluper{
//6/12
Connections::Connections(int _dataFd):dataFd(_dataFd),_msg_(new Message(IO_Direction::IN))
{
    
}

Connections::~Connections()
{
    delete _msg_;
}




inline void Connections::FlushOut()
{
    //!bug:Segmentation fault
    while(writerBuffer.size()>0){
        if(true == writeFd(*writerBuffer.front())) 
            writerBuffer.pop_front();
        else break;
    }
}

inline bool Connections::writeFd(std::string &_output)
{
    bool ret = false;
    char *_outbuf = (char*)calloc(1UL,_output.size());
    _output.copy(_outbuf,_output.size());
    if(dataFd>=0 && (_output.size() == send(dataFd,_outbuf,_output.size(),0))){
        std::cout<<"send to client:"<<dataFd<<", packetSize: "<<_output.size()<<std::endl;
        ret = true;
    }
    free(_outbuf);
    return ret;
}

inline bool Connections::ReadFd(std::string& _input)
{
    bool ret = false;
    char buf[1024] = {0};
    while(true){
        ssize_t rcvLen = recv(dataFd,buf,sizeof(buf),MSG_DONTWAIT);
        if(0 < rcvLen){
            _input.append(buf,rcvLen);
            ret = true;
        }
        //客户端关闭，将NeedClose置为true，服务器执行被动关闭前，需要将writerBffer的数据发送出去
        else if(0 == rcvLen){ 
            FlushOut();
            SetConnectionClose();
            break;
        }
        else{
            //!EAGAIN：套接字currentfd设置是非阻塞的,但recv是阻塞的或者接受超时是出现
            //!EWOULDBLOCK：用于非阻塞模式，不需要重新读或者写
            //!EINTR：指操作被中断唤醒，需要重新读/写
            if(errno == EAGAIN || errno == EWOULDBLOCK) break;
            else if(errno == EINTR) continue;
            else{
                perror("Recv:");
                break;
            }
        }  
    }
    return ret;
} 

inline AbstractMsg *Connections::currentHandle(AbstractMsg &msg)
{
    AbstractMsg* _msg = nullptr;
    DYNAMIC_GETREF(IOState,state,msg);
    if(state->IO_DIC==IO_Direction::IN){
        if(true == ReadFd(_msg_->message)){
            _msg = _msg_;
        }
    }
    /*若是业务层有数据要发送,会调用Handle函数，数据是Message形式的额，同时不会马上发送，而是存储在writeBuffer里面；同时我们还需要让服务器能够察觉到有数据要发送触发epoll_wait的写事件
    因此服务器必须提供一个静态函数来设置相应dataFd的event，是不可写状态变为可写*/
    else if(state->IO_DIC == IO_Direction::OUT){
        DYNAMIC_GETREF(Message,_outMsg,msg);
#ifdef _OLD_CODE_
        if(false == HashOutPut()){
            Server::ServerSetConectionOut(*this);
        }
#endif
        std::shared_ptr<std::string> str = std::make_shared<std::string>(_outMsg->message);
        writerBuffer.push_back(str);
        //对来自数据层的Message释放
        delete _outMsg;
        _outMsg = nullptr;
        FlushOut();
    }
    return _msg;
}

inline AbstractHandle *Connections::GetNextHanlder(AbstractMsg &msg)
{
    AbstractHandle* nextHanlder = nullptr;
    DYNAMIC_GETREF(Message,_msg,msg);
    if(_msg->IO_DIC==IO_Direction::IN){
        nextHanlder = GetDataProcessObj(*_msg);
    }
    return nextHanlder;
}

}

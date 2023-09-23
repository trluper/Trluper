#include "dataProcess.h"
namespace Trluper{

DataProcess::DataProcess()
{
}

DataProcess::~DataProcess()
{
   
}


//!msg的真实类型是Message或者UserMsg
inline AbstractMsg *DataProcess::currentHandle(AbstractMsg &msg)
{
    AbstractMsg* _UMmsg = nullptr;
    DYNAMIC_GETREF(IOState,state,msg);
    if(IO_Direction::IN==state->IO_DIC){//IN
        DYNAMIC_GETREF(Message,_msg,msg);
        Request *request = MsgToRequest(*_msg);
        if(nullptr != request){
            //需要在业务层释放
            UserMsg* _msg_ = new UserMsg(IO_Direction::IN);
            _msg_->SetRequest(request);
            _UMmsg = _msg_;
        }
    }
    else if(IO_Direction::OUT==state->IO_DIC){
        DYNAMIC_GETREF(UserMsg,_userMsg,msg);
        Message* response = RequestToMsg(*(_userMsg->getRequest()));
        if(nullptr != response){
            //需要在连接层释放
            _UMmsg = response;
        }
        //释放业务层下来的msg
        delete _userMsg;
        _userMsg = nullptr;
    }
    return _UMmsg;
}

inline AbstractHandle *DataProcess::GetNextHanlder(AbstractMsg &msg)
{
    AbstractHandle* nextHanlder = nullptr;
    DYNAMIC_GETREF(IOState,state,msg);
    if(IO_Direction::IN == state->IO_DIC){
        //msg为UserMsg
        nextHanlder = GetApplycationsObj(msg);
    }
    else{
        //msg为Message
        nextHanlder = GetConnectionsObj(msg);
    }
    return nextHanlder;
}

}
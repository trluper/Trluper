#include "fapplycations.h"

namespace Trluper{

FApplycations::FApplycations()
{
}

FApplycations::~FApplycations()
{
}

AbstractMsg *FApplycations::currentHandle(AbstractMsg &msg)
{
    AbstractMsg* _UMsg = nullptr;
    DYNAMIC_GETREF(IOState,state,msg);
    if(IO_Direction::IN==state->IO_DIC){
        DYNAMIC_GETREF(UserMsg,_umsg_,msg);
        Request* _request = ProcRequest(*(_umsg_->getRequest()));
        if(nullptr != _request){
            UserMsg* _UMsg_ = new UserMsg(IO_Direction::IN);
            _UMsg_->SetRequest(_request);
            _UMsg = _UMsg_;
        }
        //释放Dataprocess阶段的的请求
        delete _umsg_;
        _umsg_ = nullptr;
    }
    return _UMsg;
}

AbstractHandle *FApplycations::GetNextHanlder(AbstractMsg &msg)
{
    return GetNextApplycationsObj(msg);
}

}
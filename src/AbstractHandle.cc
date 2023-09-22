#include "AbstractHandle.h"

void AbstractHandle::Handle(AbstractMsg &msg)
{
    AbstractMsg* _msg = nullptr;
    AbstractHandle* nextHanlder = nullptr;
    _msg = currentHandle(msg);
    if(nullptr != _msg){
        nextHanlder = GetNextHanlder(*_msg);
        if(nullptr != nextHanlder){
            nextHanlder->Handle(*_msg);
        }
    }
}

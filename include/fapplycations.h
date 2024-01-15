#ifndef _FAPPLYCATIONS_H_
#define _FAPPLYCATIONS_H_

#include "abstractHandle.h"
#include <list>

namespace Trluper{
class UserMsg;

class FApplycations:public AbstractHandle{
public:
    typedef std::shared_ptr<FApplycations> ptr;
public:
    FApplycations();
    virtual ~FApplycations();
    //该业务对象处理请求的函数，內部实现业务功能，若还需要进一步处理，则需要返回相关的请求信息（堆对象）
    virtual Request* ProcRequest(Request& _request) = 0;
    //下一个业务处理者处理
    virtual FApplycations* GetNextApplycationsObj(AbstractMsg& msg) = 0;
    
protected:
    //本阶段真正的处理函数,
    virtual AbstractMsg* currentHandle(AbstractMsg& msg)override final;
    virtual AbstractHandle* GetNextHanlder(AbstractMsg& msg)override final;
};

}

#endif

#ifndef _ABSTRACTHANDLE_H_
#define _ABSTRACTHANDLE_H_

#include<string>
#include "Message.h"


/*定义一个支持动态类型转换的宏，判断orig数据能否转为type类型的指针，若转换失败则执行返回NULL，成功则返回相应的指针*/
#define DYNAMIC_GETREF(type, ref, orig)  type * ref = dynamic_cast<type *>(&orig); if (nullptr == ref) {return nullptr;} 

/// @brief 所有带处理操作的基类,后续的Connections、
class AbstractHandle{
public:
    AbstractHandle(){}
    virtual ~AbstractHandle(){}
    //所有继承AbstractHandle共同接口提供给外部调用的接口，内部会调用currentHandle和GetNxetHanlder
    virtual void Handle(AbstractMsg& msg);
protected:
    //本阶段真正的处理函数,
    virtual AbstractMsg* currentHandle(AbstractMsg& msg) = 0;
    virtual AbstractHandle* GetNextHanlder(AbstractMsg& msg) =0;
};

#endif
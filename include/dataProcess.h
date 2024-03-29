  #ifndef _DATAPROCESS_H_
#define _DATAPROCESS_H_

#include <unordered_map>
#include "abstractHandle.h"
#include "connections.h"
#include "fapplycations.h"

namespace Trluper{

class Connections;
class FApplycations;
class UserMsg;


class DataProcess:public AbstractHandle
{
public:
    typedef std::shared_ptr<DataProcess> ptr;
public:
    DataProcess();
    virtual ~DataProcess();
    //!开发需要重写该函数获得绑定的conn对象，返回一个Connections对象指针，
    virtual Connections* GetConnectionsObj(AbstractMsg& msg) = 0;
    //!该函数获得绑定的app对象,内部调用RouterOfApp函数获得FApplycations对象指针(添加路由模块后弃用)
    virtual AbstractHandle* GetApplycationsObj(AbstractMsg& msg) = 0;
    //*将连接层递交上来的message处理为Request类型，这里就要开发者自己依据自定义额数据格式来重写该函数返回一个Request对象指针
    virtual Request* MsgToRequest(Message& msg) = 0;
    //*将应用层递交下来的Request类型的数据转为Message类型，这里就要开发者自己依据自定义额数据格式来重写该函数
    virtual Message* RequestToMsg(Request& request) = 0;
protected:
    virtual AbstractMsg* currentHandle(AbstractMsg& msg)override final;
    virtual AbstractHandle* GetNextHanlder(AbstractMsg& msg)override final;
};

}

#endif
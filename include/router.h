#ifndef _ROUTER_H_
#define _ROUTER_H_

#include "fapplycations.h"
#include "message.h"

/* 
!@brief 路由模块，当框架应用于多业务常见时，提供给框架在数据处理层时能够依据请求类型来解决交给哪个业务对象处理
*此头文件只提供额路由模块的基类，开发者若要实现路由功能，则需要在自己实现该类的两个基本纯虚函数，同时在开发者自己实现的数据处理层中添加路由选择函数
*/
class FApplycations;
class Request;

class Router{
public:
    Router(){};
    virtual ~Router(){};
    /*依据数据处理层转化成的请求，获得业务对象*/
    virtual FApplycations* GetFApplycationsObj(Request* request) = 0;
    /*r如果路由表中改业务对象为nullptr,则需要创建*/
    virtual void CreatApplycationsObj(Request* request) = 0;
};


#endif
#ifndef _MYSQLPOOL_H_
#define _MYSQLPOOL_H_

#include <mysql/mysql.h>
#include <string>
#include <memory>
#include <list>
#include "noncopyable.h"
#include "mutex.h"
#include "log.h"

namespace Trluper
{
class MySQLPool:public std::enable_shared_from_this<MySQLPool>,public Noncopyable{
public:
    typedef std::shared_ptr<MySQLPool> ptr;
public:
    /// @brief 数据连接池的构造函数
    /// @param maxConn 预创建的最大连接数量
    /// @param ip 数据库的主机ip
    /// @param user 数据库用户名
    /// @param password 数据库的密码
    /// @param database 使用哪个数据库
    /// @param port 端口号
    MySQLPool(uint32_t maxConn,std::string ip,std::string user,std::string password,std::string database,unsigned int port);
    ~MySQLPool();
    /// @brief 初始化数据库连接池的初始配置
    void init();
    /// @brief 获取一个空闲的连接
    /// @return MYSQL连接句柄
    MYSQL* getFreeConn();
   
    /// @brief 归还一个连接给连接池
    /// @param conn 需要归还的连接
    /// @return bool
    bool freeConn(MYSQL* conn);
    /// @brief 释放（删除）所有连接
    void deleteAllConn();
    /// @brief 获取当前连接池中的空闲连接数
    /// @return 返回一个非负整数，表示当前的空闲连接数
    uint32_t getFreeConnNum()const{return m_freeConn;}
    /// @brief 获取当前连接池中的最大连接数
    /// @return 返回一个非负整数，表示当前的最大连接数
    uint32_t getMaxConnNum()const{return m_maxConn;}
    /// @brief 增加一个连接数
    /// @return bool
    bool increaseConn();
    /// @brief 减小一个连接数
    /// @return bool
    bool decreaseConn();
    /// @brief 尝试将连接池的连接数调整为newMaxConn（该函数不一定满足给定的newMaxConn)
    /// @param newMaxConn 新的最大连接数
    /// @return 已经取得的连接数
    uint32_t adjustMaxConn(uint32_t newMaxConn);
    
private:
    Trluper::Mutex m_mutex;         //互斥锁
    Trluper:: Semaphore m_sem;      //信号量
    std::list<MYSQL *> m_connPool;  //数据库连接池
    uint32_t m_maxConn;             //连接数数量
    uint32_t m_freeConn;            //当前可使用的连接数
private:
    std::string m_ip;               //主机地址
    std::string m_user;             //数据库的用户名
    std::string m_password;         //登录数据库的密码  
    std::string m_DatabaseName;     //使用的数据库名称
    unsigned int m_port;            //数据库端口

};

} //
#endif
#include "mysqlpool.h"

namespace Trluper{
static Logger::ptr logger = nullptr;

MySQLPool::MySQLPool(uint32_t maxConn, std::string ip, std::string user,
 std::string password, std::string database, unsigned int port)
 :m_maxConn(maxConn),m_ip(ip),m_user(user),m_password(password),
  m_DatabaseName(database),m_port(port)
{
    logger = LOG_GET_MANAGER->getMainLogger();
    LOG_SS_INFO(logger)<<"Initialize the MySQL database connections pool."<<std::endl;
    init(); //初始化MySQL连接池
}

MySQLPool::~MySQLPool()
{
    deleteAllConn();
}
void MySQLPool::init()
{
    for(uint32_t i = 0; i < m_maxConn; ++i){
        //初始化MySQL句柄
        MYSQL* conn = nullptr;
        conn = mysql_init(conn);
        if(nullptr == conn){
            LOG_SS_ERROR(logger)<<"MySQL init error"<<std::endl;
            exit(1);
        }
        //连接
        //出错请执行 SELECT user, host FROM mysql.user WHERE user = 'm_user';查看该数据库用户是否允许远程连接
        conn = mysql_real_connect(conn, m_ip.c_str(), m_user.c_str(), m_password.c_str(), m_DatabaseName.c_str(), m_port, NULL,0);
        if(nullptr == conn){
            LOG_SS_ERROR(logger)<<"MySQL connection failed."<<std::endl;
            exit(1);
        }
        m_connPool.push_back(conn);
        ++m_freeConn;
        m_sem.notify();
    }
}
MYSQL *MySQLPool::getFreeConn()
{
    MYSQL* conn = nullptr;
    if(0 == m_connPool.size()){
        return conn;
    }
    m_sem.wait();
    m_mutex.lock();
    conn = m_connPool.front();
    m_connPool.pop_front();
    --m_freeConn;
    m_mutex.unlock();
    return conn;
}

bool MySQLPool::freeConn(MYSQL *conn)
{
    if(nullptr == conn) {
        return false;
    }
    m_mutex.lock();
    m_connPool.push_back(conn);
    ++m_freeConn;
    m_mutex.unlock();
    m_sem.notify();
    return true;
}

void MySQLPool::deleteAllConn()
{
    m_mutex.lock();
    for(auto it = m_connPool.begin(); it != m_connPool.end(); ++it){
        MYSQL* conn = *it;
        mysql_close(conn);
    }
    m_maxConn = 0;
    m_freeConn = 0;
    m_connPool.clear();
    m_mutex.unlock();
}

bool MySQLPool::increaseConn()
{
    MYSQL* conn = nullptr;
    conn = mysql_init(conn);
    if(nullptr == conn){
        return false;
    }
    conn = mysql_real_connect(conn, m_ip.c_str(), m_user.c_str(), m_password.c_str(), m_DatabaseName.c_str(), m_port, NULL, 0);
    if(nullptr == conn){
        LOG_SS_ERROR(logger)<<"MySQL connection failed."<<std::endl;
        return false;
    }
    m_mutex.lock();
    m_connPool.push_back(conn);
    ++m_freeConn;
    m_mutex.unlock();
    ++m_maxConn;
    m_sem.notify();
    return true;
}

bool MySQLPool::decreaseConn()
{
    if(m_connPool.empty()){
        return false;
    }
    m_sem.wait();
    m_mutex.lock();
    m_connPool.pop_front();
    --m_freeConn;
    --m_maxConn;
    m_mutex.unlock();
    return true;
}

uint32_t MySQLPool::adjustMaxConn(uint32_t newMaxConn)
{
    if(newMaxConn > m_maxConn){
        while(m_maxConn != newMaxConn){
            if(false == increaseConn()){
                break;
            }
        }
    }else if(newMaxConn < m_maxConn){
        while(m_maxConn != newMaxConn){
            if(false == decreaseConn()){
                break;
            }
        }
    }
    return m_maxConn;
}

}
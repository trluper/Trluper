/**
* @file log.h
* @brief 实现日志系统
* @author Trluper
* @date 2023/10/19
*/

#ifndef _LOG_H_
#define _LOG_H_

#include <string>
#include <exception>
#include <iostream>
#include <stdint.h>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdarg.h>
#include <map>
#include <functional>
#include <yaml-cpp/yaml.h>
#include "util.h"
#include "thread.h"
#include "mutex.h"
#include "stringException.h"

/*
* 定义相应宏供使用
*/

//! 流式方式将日志级别level的日志写入logger
#define LOG_SS(logger, level) \
    if(logger->getLogLevel() <= level) \
        Trluper::LogEventWrap(Trluper::LogEvent::ptr(new Trluper::LogEvent(logger ,level,\
        __FILE__, __func__, __LINE__, 0, Trluper::GetThreadID(), Trluper::GetCoroutineID(), \
        time(0), Trluper::Thread::GetName()))).getSS()

#define LOG_SS_DEBUG(logger) LOG_SS(logger, Trluper::LogLevel::DEBUG)
#define LOG_SS_INFO(logger) LOG_SS(logger,Trluper::LogLevel::INFO)
#define LOG_SS_WARN(logger) LOG_SS(logger,Trluper::LogLevel::WARN)
#define LOG_SS_ERROR(logger) LOG_SS(logger,Trluper::LogLevel::ERROR)
#define LOG_SS_FATAL(logger) LOG_SS(logger,Trluper::LogLevel::FATAL)

//!使用格式化方式将日志写入logger
#define LOG_FMT(logger, level, fmt, ...)\
    if(logger->getLogLevel() <= level)\
        Trluper::LogEventWrap(Trluper::LogEvent::ptr(new Trluper::LogEvent(logger, level, \
        __FILE__, __func__, __LINE__, 0 , Trluper::GetThreadID(), Trluper::GetCoroutineID(), \
        time(0), Trluper::Thread::GetName()))).getLogEvent()->format(fmt,__VA_ARGS__)
#define LOG_FMT_DEBUG(logger,fmt,...) LOG_FMT(logger, Trluper::LogLevel::DEBUG, fmt, __VA_ARGS__)
#define LOG_FMT_INFO(logger,fmt,...) LOG_FMT(logger, Trluper::LogLevel::INFO, fmt, __VA_ARGS__)
#define LOG_FMT_WARN(logger,fmt,...) LOG_FMT(logger, Trluper::LogLevel::WARN, fmt, __VA_ARGS__)
#define LOG_FMT_ERROR(logger,fmt,...) LOG_FMT(logger, Trluper::LogLevel::ERROR, fmt, __VA_ARGS__)
#define LOG_FMT_FATAL(logger,fmt,...) LOG_FMT(logger, Trluper::LogLevel::FATAL, fmt, __VA_ARGS__)

//!获得日志器管理类的单例对象
#define LOG_GET_MANAGER Trluper::LoggerManager::getSingle()

namespace Trluper{

//日志器
class Logger;
//日志器管理类
class LoggerManager;

/// @brief 日志级别
class LogLevel{
public:
    
    enum Level{
        //未知级别
        UNKNOW = 0,
        //DEBUG级别
        DEBUG = 1,
        //INFO级别
        INFO = 2,
        //WARN级别
        WARN = 3,
        //ERROR级别
        ERROR = 4,
        //FATAL级别,灾难性错误， 在能够输出日志的所有等级中最高
        FATAL = 5
    };

    /// @brief 将日志级别转换程文本流输出
    /// @param level 日志级别
    /// @return const char*
    static const char* ToString(LogLevel::Level level);

    /// @brief 将文本转换成日志级别
    /// @param str string字符串
    /// @return LogLevel::Level
    static LogLevel::Level FromString(const std::string& str);

};


/// @brief 日志事件，主要会有触发日志事件的内容、时间、级别、文件名、行号、线程ID、线程名字等
class LogEvent{
public:
    typedef std::shared_ptr<LogEvent> ptr;
    /// @brief 日志时间构造函数
    /// @param logger 日志器智能指针对像
    /// @param level 日志级别
    /// @param file 文件名
    /// @param line 行号
    /// @param elapse 程序启动开始到现在的毫秒数
    /// @param threadId 线程ID
    /// @param fiberId 协程ID
    /// @param time 时间戳
    /// @param threadName 线程名字
    LogEvent(std::shared_ptr<Logger> logger,LogLevel::Level level
            ,const char* file, const char* func, uint32_t line,uint32_t elapse
            ,uint32_t threadId,uint32_t coroutineId,uint64_t time
            ,const std::string& threadName);
    /*
    * !以下定义各属性的get方法
    */

    /// @brief 得到File
    /// @return const char*
    const char* getFile() const {return m_file;}

    /// @brief 获取触发事件的函数
    /// @return const char*
    const char* getFuncName() const{return m_funcName;}

    /// @brief 获取行号
    /// @return 返回行号m_line
    int32_t getline()const {return m_line;}

    /// @brief 获取当前日志事件触发毫秒数
    /// @return 返回m_elapse
    uint32_t getElapse() const {return m_elapse;}

    /// @brief 获取触发日志事件的线程ID
    /// @return 返回m_threadID
    uint32_t getThreadID() const {return m_threadID;}

    /// @brief 获取触发日志事件的协程ID
    /// @return 返回m_fiberID
    uint32_t getCoroutineID() const {return m_coroutineID;}

    /// @brief 获取触发日志事件的事件戳
    /// @return 返回m_time
    uint64_t getTime() const {return m_time;}

    /// @brief 获取触发日志事件的线程名称
    /// @return m_threadName
    std::string getThreadName() const {return m_threadName;}

    /// @brief 流形式获得内容m_ss(外部调用)
    /// @return 返回m_Ss(stringstream)
    std::stringstream& getSs()  {return m_ss;}
    /// @brief 返回日志内容
    /// @return string
    std::string getContent() const {return m_ss.str();}

    /// @brief 获取日志器智能指针
    /// @return 返回m_logger
    std::shared_ptr<Logger> getLogger() const { return m_logger;}

    /// @brief 获取日志事件的等级
    /// @return 返回m_level
    LogLevel::Level getLogLevel() const { return m_level;}
                                                                                                                                                                                                                                                                                                                                                                          
    /// @brief 格式形式得到日志内容即得到m_ss,内部调用format(const char* fmt, va_list al);
    /// @param fmt：m_ss流的格式化形式
    /// @param  ...：一些需要的变参
    void format(const char* fmt, ...);

    /// @brief 格式化写入日志内容流m_ss
    /// @param fmt 
    /// @param al 
    void format(const char* fmt, va_list al);
private:
    //文件名
    const char* m_file = nullptr;
    //函数名
    const char* m_funcName = nullptr;
    //行号
    uint32_t m_line = 0;
    //程序启动开始到现在的毫秒数
    uint32_t m_elapse = 0;
    //线程ID
    uint32_t m_threadID = 0; 
    //协程ID
    uint32_t m_coroutineID = 0;
    //时间戳
    uint64_t m_time = 0;
    //线程名称
    std::string m_threadName;
    //日志内容流
    std::stringstream m_ss;
    //日志器
    std::shared_ptr<Logger> m_logger;
    //日志等级
    LogLevel::Level m_level;
};


/// @brief 日志事件包装器
class LogEventWrap{
public:
    /// @brief 构造函数
    /// @param e 日志事件（智能指针）
    LogEventWrap(LogEvent::ptr e);
    ~LogEventWrap();
    LogEvent::ptr getLogEvent()const{return m_event;}

    /// @brief 获得日志的内容流
    /// @return stringstream
    std::stringstream& getSS();
private:
    LogEvent::ptr m_event;
};



/// @brief 日志的格式化器
class LogFormatter{
public:
    typedef std::shared_ptr<LogFormatter> ptr;
public:
    /**
    * @brief 实现类似log4j形式的日志输出格式模板
    *     * @param[in] pattern 格式模板
     * @details 
     *  %m 消息
     *  %p 日志级别
     *  %r 累计毫秒数
     *  %c 日志名称
     *  %t 线程id
     *  %N 线程名称
     *  %C 协程ID
     *  %n 换行
     *  %d 时间
     *  %f 文件名
     *  %F 函数名称 
     *  %l 行号
     *  %T 制表符
     *
     *  默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
    */
    LogFormatter(const std::string& pattern);


    /// @brief 返回格式化后的日志文本string,即将日志事件event按照格式化模板格式为字符串,内部调用FormatItem的format
    /// @param Logger 日志器
    /// @param level 日志等级
    /// @param event 日志事件
    /// @return 
    std::string format(std::shared_ptr<Logger> Logger,LogLevel::Level level,LogEvent::ptr event);
    
    /// @brief 将日志事件event按照格式化模板格式为字符串给输出流对象os(可为标准输出流cout、文件输出流ofstream)
    /// @param os 输出流对象
    /// @param logger 日志器
    /// @param level 日志等级
    /// @param event 日志事件
    /// @return 
    std::ostream& format(std::ostream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event);

    /// @brief 初始化，解析日志模板，创建各字段的对象存入m_items
    void init();
    /// @brief 是否有错
    /// @return bool
    bool isError()const{return m_error;}
    /// @brief 返回日志模板
    /// @return 
    const std::string getPattern()const {return m_pattern;}
public:
    /// @brief 格式转换器（内部类）
    class FormatItem{
    public:
        typedef std::shared_ptr<FormatItem> ptr;    
        virtual ~FormatItem(){}
        virtual void format(std::ostream& os, std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) = 0;
    };

protected:
    //日志的格式模板
    std::string m_pattern;
    //日志格式解析后的格式，内部节点是一个个格式转换器
    std::vector<FormatItem::ptr> m_items;
    //是否有错误
    bool m_error = false;
};




/// @brief Appender父类，决定日志的输出去向，需要继承实现，本日志系统实现了终端和文本输出
class LogAppender{
friend class Logger;
public:
    typedef std::shared_ptr<LogAppender> ptr;
public:
    //析构函数
    virtual ~LogAppender(){}
    /// @brief 写入日志函数
    /// @param logger 日志器
    /// @param level 日志级别
    /// @param event 日志事件
    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
    
    /// @brief 将日志器的配置转成YAML String，得到相应的字符串
    /// @return string
    virtual std::string toYAMLString() = 0;

    /// @brief 更改日志的格式器
    /// @param formatter 新的日志格式器
    void setFormatter(LogFormatter::ptr formatter);

    /// @brief 获取日志格式化器
    /// @return 返回LogFormatter
    LogFormatter::ptr getFormatter();
    /// @brief 获取日志级别
    /// @return 日志级别LogLevel::Level
    LogLevel::Level getLogLevel()const {return m_level;}
    /// @brief 设置日志级别,日志事件只有在不低于m_Level情况下才会写入log
    /// @param level 新的日志级别
    void setLogLevel(LogLevel::Level level){m_level = level;}
protected:
    //日志级别
    LogLevel::Level m_level = LogLevel::DEBUG;
    //是否初始化了日志格式器
    bool m_hasFormatter = false;
    //自旋锁
    Trluper::Spinlock m_spinlock;
    //日志格式器
    LogFormatter::ptr m_formatter;
};


/// @brief 输出到控制台的Appender
class StdoutLogAppender: public LogAppender{
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
    std::string toYAMLString() override;
};

/// @brief 日志输出到日志文件中
class FileLogAppender : public LogAppender{
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string& filename);
    void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
    std::string toYAMLString() override;

    /// @brief 重新打开日志文件
    /// @return 成功则返回true
    bool reopen();
private:
    //日志文件路径
    std::string m_filename;
    //文件输出流
    std::ofstream m_filestream;
    //上次重新打开的事件
    uint64_t m_lastOpenTime = 0;
};







/// @brief 日志器，
class Logger:public std::enable_shared_from_this<Logger>{
friend class LoggerManager;
public:
    typedef std::shared_ptr<Logger> ptr;
public:
    Logger(const std::string& name = "root");
    /// @brief 写日志操作
    /// @param level 日志级别
    /// @param event 日志事件
    void log(LogLevel::Level level, LogEvent::ptr event);

    /// @brief debug级别的日志
    /// @param event 日志事件
    void debug(LogEvent::ptr event);
    /// @brief info级别日志
    /// @param event 日志事件
    void info(LogEvent::ptr event);
    /// @brief warnning级别日志
    /// @param event 日志事件
    void warn(LogEvent::ptr event);
    /// @brief error级别日志
    /// @param event 日志事件
    void error(LogEvent::ptr event);
    /// @brief fatal级别日志
    /// @param event 日志事件
    void fatal(LogEvent::ptr event);

    /// @brief 添加日志输出去向对象（shared_ptr)
    /// @param appender 
    void addAppender(LogAppender::ptr appender);
    /// @brief 删除日志输出去向对象
    /// @param appender 
    void delAppender(LogAppender::ptr appender);
    /// @brief 情况日志输出去向对象
    void clearAppenders();
    
    /// @brief 获取日志级别
    /// @return 日志级别
    LogLevel::Level getLogLevel()const{return m_level;}
    /// @brief 设置日志器的日志级别（表示该日志器只对日志事件中不低于level等级的事件进行log)
    /// @param level 日志级别
    void setLogLevel(LogLevel::Level level){m_level = level;}
    /// @brief 获取日志名称
    /// @return 日志名称
    const std::string& getName()const {return m_name;}

    /// @brief 设置日志的格式化器，同时会将其所有的appender的格式化器也设置为formatter
    /// @param formatter 新的格式化器对象
    void setFormatter(LogFormatter::ptr formatter);
    /// @brief 设置日志的格式化器，格式化模板为str
    /// @param str 格式化模板字符串
    void setFormatter(const std::string& str);

    /// @brief 获取日志格式化器对象
    /// @return 格式化器对象
    LogFormatter::ptr getFormatter();
    /// @brief 将日志器的配置转换为YAML String
    /// @return string
    std::string toYAMLString();

private:
    //日志名称
    std::string m_name;
    //日志级别,当触发的日志事件级别大于该m_level时，才会执行appender日志输出去向，默认为DEBUG
    LogLevel::Level m_level;
    //自旋锁
    Spinlock m_spinlock;
    //日志appender集合，该日志器会调用这些日志器appender于何处写入log
    std::list<LogAppender::ptr> m_appenders;
    //日志格式器，日志器会依照该日志输出格式写入log
    LogFormatter::ptr m_formatter;
    //主日志器
    Logger::ptr m_mainLogger;

};




/// @brief 日志器管理类，单例模式，默认会初始化一个主日志器m_mainLogger(appender为StdOutLogAppender)
class LoggerManager{
public:
    ~LoggerManager();
    /// @brief 获取指定名称的日志器，如果还没有创建，则创建该日志器
    /// @param name ：日志器名称
    /// @return  Logger::ptr
    Logger::ptr getLogger(const std::string& name);
    /// @brief 初始化工作
    void init();
    /// @brief 获取主日志器
    /// @return 主日志器对象
    Logger::ptr getMainLogger()const {return m_mainLogger;}
    /// @brief 将所有日志器配置成YAML String（入口toYAMLString）：调用路径：LoggerManager->Logger->Appender
    /// @return string
    std::string toYAMLString();
    /// @brief 获得日志器管理类的单例对象
    /// @return 单例对象
    static LoggerManager* getSingle();
    /// @brief 释放单例对象
    static void deleteManager();
protected:
    //私有化构造函数和拷贝构造函数
    LoggerManager();
    LoggerManager(const LoggerManager& manager);
    const  LoggerManager& operator=(const  LoggerManager& single);
private:
    //自旋锁
    Spinlock m_spinlock;
    //日志器容器
    std::unordered_map<std::string,Logger::ptr> m_mapOfLoggers;
    //主日志器
    Logger::ptr m_mainLogger;
    //日志器管理类单例对象
    static LoggerManager* singleLoggerManager;
};



}
#endif
#include "log.h"

namespace Trluper{

//!日志器管理类单例对象
LoggerManager* LoggerManager::singleLoggerManager = new(std::nothrow) LoggerManager();


//!各种FormatItem类的实现--->执行日志格式化时调用（其对象存储至Formatter的m_items中
//* %m 日志事件内容
class ContentFormatItem:public LogFormatter::FormatItem{
public:
    ContentFormatItem(const std::string& str=""){}
    void format(std::ostream& os, Logger::ptr loger, LogLevel::Level level, LogEvent::ptr event) override{
        os<<event->getContent();
    }
};

//* %p 日志事件等级
class LogLevelFormatItem:public LogFormatter::FormatItem{
public:
    LogLevelFormatItem(const std::string& str=""){}
    void format(std::ostream& os, Logger::ptr loger, LogLevel::Level level, LogEvent::ptr event) override{
        os<<LogLevel::ToString(level);
    }
};

//* %r 程序启动到到现在的毫秒数
class ElapseFormatItem:public LogFormatter::FormatItem{
public:
    ElapseFormatItem(const std::string& str=""){}
    void format(std::ostream& os, Logger::ptr loger, LogLevel::Level level, LogEvent::ptr event) override{
        os<<event->getElapse();
    }
};


//* %c 日志器名称
class LoggerNameFormatItem:public LogFormatter::FormatItem{
public:
    LoggerNameFormatItem(const std::string& str=""){}
    void format(std::ostream& os, Logger::ptr loger, LogLevel::Level level, LogEvent::ptr event) override{
        os<<event->getLogger()->getName();
    }
};

//* %t 触发日志事件的线程id
class ThreadIdFormatItem:public LogFormatter::FormatItem{
public:
    ThreadIdFormatItem(const std::string& str=""){}
    void format(std::ostream& os, Logger::ptr loger, LogLevel::Level level, LogEvent::ptr event) override{
        os<<event->getThreadID();
    }
};

//* %N 触发日志事件的线程名称
class ThreadNameFormatItem:public LogFormatter::FormatItem{
public:
    ThreadNameFormatItem(const std::string& str=""){}
    void format(std::ostream& os, Logger::ptr loger, LogLevel::Level level, LogEvent::ptr event) override{
        os<<event->getThreadName();
    }
};

//* %C 触发日志事件的协程ID
class CoroutineIdFormatItem:public LogFormatter::FormatItem{
public:
    CoroutineIdFormatItem(const std::string& str=""){}
    void format(std::ostream& os, Logger::ptr loger, LogLevel::Level level, LogEvent::ptr event) override{
        os<<event->getCoroutineID();
    }
};


//* %n 换行
class NewLineFormatItem:public LogFormatter::FormatItem{
public:
    NewLineFormatItem(const std::string& str=""){}
    void format(std::ostream& os, Logger::ptr loger, LogLevel::Level level, LogEvent::ptr event) override{
        os<<std::endl;
    }
};

//* %d{%Y-%m-%d %H:%M:%S} 时间%Y-%m-%d %H:%M:%S
class DateTimeFormatItem:public LogFormatter::FormatItem{
public:
    DateTimeFormatItem(const std::string& str="%Y-%m-%d %H:%M:%S"):m_DateTime_stander(str){
        if(m_DateTime_stander.empty()){
            m_DateTime_stander = "%Y-%m-%d %H:%M:%S";
        }
    }
    void format(std::ostream& os, Logger::ptr loger, LogLevel::Level level, LogEvent::ptr event) override{
        struct tm t;
        time_t time = event->getTime();
        localtime_r(&time,&t);
        char buf[64];
        strftime(buf,sizeof(buf),m_DateTime_stander.c_str(),&t);
        os<<buf;
    }
private:
    std::string m_DateTime_stander;
};

//* %f 文件名
class FileNameFormatItem:public LogFormatter::FormatItem{
public:
    FileNameFormatItem(const std::string& str=""){}
    void format(std::ostream& os, Logger::ptr loger, LogLevel::Level level, LogEvent::ptr event) override{
        os<<event->getFile();
    }
};
//* %F 触发日志事件的函数名称
class FuncNameFormatItem:public LogFormatter::FormatItem{
public:
    FuncNameFormatItem(const std::string& str=""){}
    void format(std::ostream& os, Logger::ptr loger, LogLevel::Level level, LogEvent::ptr event) override{
        os<<event->getFuncName();
    }
};

//* %l 触发日志事件的行号
class LineFormatItem:public LogFormatter::FormatItem{
public:
    LineFormatItem(const std::string& str=""){}
    void format(std::ostream& os, Logger::ptr loger, LogLevel::Level level, LogEvent::ptr event) override{
        os<<event->getline();
    }
};

//* %T 制表符TAB
class TabFormatItem:public LogFormatter::FormatItem{
public:
    TabFormatItem(const std::string& str=""){}
    void format(std::ostream& os, Logger::ptr loger, LogLevel::Level level, LogEvent::ptr event) override{
        os<<"\t";
    }
};

//* 格式模板自带的字符串 如"[ ] :"等
class StringFormatItem:public LogFormatter::FormatItem{
public:
    StringFormatItem(const std::string& str):m_str(str){}
    void format(std::ostream& os, Logger::ptr loger, LogLevel::Level level, LogEvent::ptr event) override{
        os<<m_str;
    }
private:
    std::string m_str;
};




//!LogLevel类函数定义
const char *LogLevel::ToString(LogLevel::Level level)
{
    switch(level) {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARN:
            return "WARN";
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::FATAL:
            return "FATAL";
        default:
            return "UNKNOW";
    }
    return "UNKNOW";  
}

LogLevel::Level LogLevel::FromString(const std::string &str)
{
 #define XX(level, v) \
    if(str == #v) { \
        return LogLevel::level; \
    }
    XX(DEBUG, debug);
    XX(INFO, info);
    XX(WARN, warn);
    XX(ERROR, error);
    XX(FATAL, fatal);

    XX(DEBUG, DEBUG);
    XX(INFO, INFO);
    XX(WARN, WARN);
    XX(ERROR, ERROR);
    XX(FATAL, FATAL);
    return LogLevel::UNKNOW;
#undef XX
}

//!LogEvent类函数定义  
LogEvent::LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level
        , const char *file, const char* func,uint32_t line, uint32_t elapse, uint32_t threadId
        , uint32_t coroutineId, uint64_t time, const std::string &threadName)
    : m_file(file),m_funcName(func),m_line(line),m_elapse(elapse),
    m_threadID(threadId),m_coroutineID(coroutineId),m_time(time),m_threadName(threadName),m_logger(logger),m_level(level)
{ 

}

void LogEvent::format(const char *fmt, ...)
{
    va_list al; //char*类型
    va_start(al,fmt);   //获得第一个可变参数地址
    format(fmt,al);
    va_end(al); //清除char*,赋值为0
}

void LogEvent::format(const char *fmt, va_list al)
{
    char* buf = nullptr;
    /*函数解释：int vasprintf(char **str, const char *format, va_list ap);
    *它可以通过可变参数创建一个格式化的字符串，并将其存储在动态分配的内存中。
    *它的使用方法与 printf 类似，但它不会将结果打印到标准输出流中，而是将其存储在一个指向字符数组的指针中。
    * str：指向指针变量的指针，用于存储格式化字符串的结果。
    * format：格式化字符串的格式，与 printf 函数中的用法相同。
    * ap：指向使用 va_start 和 va_arg 函数处理的可变参数列表的指针。
    */
    int len = vasprintf(&buf,fmt,al);
    if(len != -1){
        //按格式给m_ss
        m_ss << std::string(buf,len);
        free(buf);
    }
}


//!LogEventWrap类的函数定义
LogEventWrap::LogEventWrap(LogEvent::ptr e):m_event(e)
{
}

LogEventWrap::~LogEventWrap()
{
    m_event->getLogger()->log(m_event->getLogLevel(),m_event);
}
std::stringstream &LogEventWrap::getSS()
{
    return m_event->getSs();
}



//!Logger类的函数定义
//*Logger构造函数,名称默认为main，Level默认为Debug
Logger::Logger(const std::string &name):m_name(name),m_level(LogLevel::DEBUG)
{
    //默认的格式化模板
    m_formatter.reset(new LogFormatter("%d{%Y-%m-%d %H:%M:%S} [%c][%p] %t(%N) %f (in %F: %l): %m%n"));
}

void Logger::log(LogLevel::Level level, LogEvent::ptr event)
{
    if(level >= m_level){
        auto self = shared_from_this();
        Trluper::Lockguard<Spinlock> Lock(m_spinlock);
        if(!m_appenders.empty()){
            for(auto& i: m_appenders){
                i->log(self,level,event);
            }
        }
        else if(m_mainLogger){
            m_mainLogger->log(level,event);
        }
    }
}

void Logger::debug(LogEvent::ptr event)
{
    log(LogLevel::DEBUG, event);
}
void Logger::info(LogEvent::ptr event)
{
    log(LogLevel::INFO, event);
}
void Logger::warn(LogEvent::ptr event)
{
    log(LogLevel::WARN, event);
}
void Logger::error(LogEvent::ptr event)
{
    log(LogLevel::ERROR, event);
}
void Logger::fatal(LogEvent::ptr event)
{
    log(LogLevel::FATAL, event);
}

void Logger::addAppender(LogAppender::ptr appender)
{
    Trluper::Lockguard<Spinlock> Lock(m_spinlock);
    //如果该appender还没有日志输出格式对象，则设置为该日志器的日志输出格式对象
    if(!appender->getFormatter()){  
        Trluper::Lockguard<Spinlock> aLock(appender->m_spinlock);
        appender->m_formatter = m_formatter;
    }
    m_appenders.push_back(appender);
}
void Logger::delAppender(LogAppender::ptr appender)
{
    Trluper::Lockguard<Spinlock> Lock(m_spinlock);
    for(auto it = m_appenders.begin(); it != m_appenders.end(); ++it){
        if(*it == appender){
            m_appenders.erase(it);
            break;
        }
    }
}
void Logger::clearAppenders()
{
    Trluper::Lockguard<Spinlock> Lock(m_spinlock);
    m_appenders.clear();
}

//*设置格式化模板
void Logger::setFormatter(LogFormatter::ptr formatter)
{
    Trluper::Lockguard<Spinlock> Lock(m_spinlock);
    m_formatter = formatter;
    for(auto& i: m_appenders){
        Trluper::Lockguard<Spinlock> alock(i->m_spinlock);
        i->m_formatter = m_formatter;
        i->m_hasFormatter = true;
    }
}

void Logger::setFormatter(const std::string &str)
{
    std::cout<<"设置日志格式化模板为"<<"---"<<str<<"---"<<std::endl;
    Trluper::LogFormatter::ptr newFormatter(new Trluper::LogFormatter(str));
    if(newFormatter->isError()){
        std::cout<<"Logger setFormatter name="<<m_name<<" string_pattern="<<str<<" is invalid formatter"<<std::endl;
        return;
    }
    setFormatter(newFormatter);
}

LogFormatter::ptr Logger::getFormatter() 
{
    Trluper::Lockguard<Spinlock> Lock(m_spinlock);
    return m_formatter;
}

std::string Logger::toYAMLString()
{
   Trluper::Lockguard<Spinlock> Lock(m_spinlock);
   YAML::Node node;
   node["name"] = m_name;
   if(m_level != LogLevel::UNKNOW){
        node["level"] = LogLevel::ToString(m_level);
   }
   if(m_formatter){
        node["formatter"] = m_formatter->getPattern();
   }
   for(auto& i: m_appenders){
        node["appenders"].push_back(YAML::Load(i->toYAMLString()));
   }
   std::stringstream ss;
   ss << node;
   return ss.str();
}


//! LoggerManager类函数定义
LoggerManager::LoggerManager()
{
    init();
}
LoggerManager::LoggerManager(const LoggerManager &manager)
{
}
const LoggerManager &LoggerManager::operator=(const LoggerManager &single)
{
   return *this;
}
LoggerManager::~LoggerManager()
{
    deleteManager();
}


Logger::ptr LoggerManager::getLogger(const std::string &&name)
{
    Trluper::Lockguard<Spinlock> Lock(m_spinlock);
    if(existLogger(name)){
        return m_mapOfLoggers[name];
    }
    Logger::ptr logger(new Logger(name));
    logger->m_mainLogger = m_mainLogger;
    m_mapOfLoggers[name]  = logger;
    return logger;
}

void LoggerManager::init()
{   //初始化一个root主日志器，Appender为StdoutLogAppende
    m_mainLogger.reset(new Logger());//主日志器（日志器管理类默初始化一主日志器）
    m_mainLogger->addAppender(LogAppender::ptr(new StdoutLogAppender()));//主日志器的appender
    m_mapOfLoggers[m_mainLogger->m_name] = m_mainLogger;//建立name与日志器的映射

}

//toYAMLString调用路径：LoggerManager->Logger->Appender
std::string LoggerManager::toYAMLString()
{
   Trluper::Lockguard<Spinlock> Lock(m_spinlock);
   YAML::Node node;
   for(auto& i : m_mapOfLoggers){
        node.push_back(YAML::Load(i.second->toYAMLString()));
   }
   std::stringstream ss;
   ss<<node;
   return ss.str();
}

LoggerManager *LoggerManager::getSingle()
{
    return singleLoggerManager;
}

void LoggerManager::deleteManager()
{
    if(singleLoggerManager != nullptr){
        delete singleLoggerManager;
        singleLoggerManager = nullptr;
    }
}



//!Appender抽象类函数定义
void LogAppender::setFormatter(LogFormatter::ptr formatter)
{
    Trluper::Lockguard<Spinlock> Lock(m_spinlock);
    m_formatter = formatter;
    if(m_formatter){
        m_hasFormatter = true;
    }else{
        m_hasFormatter = false;
    }
}

LogFormatter::ptr LogAppender::getFormatter()
{
    Trluper::Lockguard<Spinlock> Lock(m_spinlock);
    return m_formatter;
}
//!StdoutLogAppender实现
void StdoutLogAppender::log(std::shared_ptr<Logger>  logger, LogLevel::Level level, LogEvent::ptr event)
{
    if(level >= m_level){
        Trluper::Lockguard<Spinlock> Lock(m_spinlock);
        m_formatter->format(std::cout, logger, level, event);
    }
}

std::string StdoutLogAppender::toYAMLString()
{
   Trluper::Lockguard<Spinlock> Lock(m_spinlock);
   YAML::Node node;
   node["type"] = "StdoutLogAppender";
   if(m_level != LogLevel::UNKNOW){
        node["level"] = LogLevel::ToString(m_level);
   }
   if(m_hasFormatter && m_formatter){
        node["formatter"] = m_formatter->getPattern();
   }
   std::stringstream ss;
   ss<<node;
   return ss.str();
}

//! FileLogAppender实现
FileLogAppender::FileLogAppender(const std::string &&filename):m_filename(filename)
{
    reopen();
}
FileLogAppender::FileLogAppender(const std::string &filename):m_filename(filename)
{
    reopen();
}
void FileLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
{
    if(level >= m_level){
        uint64_t now = event->getTime();
        if(now >= (m_lastOpenTime+3)){
            reopen();
            m_lastOpenTime = now;
        }
        Trluper::Lockguard<Spinlock> Lock(m_spinlock);
        if(!m_formatter->format(m_filestream,logger,level,event)){
            std::cout<<"error"<<std::endl;
        }
    }
}

std::string FileLogAppender::toYAMLString()
{
    Trluper::Lockguard<Spinlock> Lock(m_spinlock);
    YAML::Node node;
    node["type"] = " FileLogAppender";
    node["file"] = m_filename;
    if(m_level != LogLevel::UNKNOW){
        node["level"] = LogLevel::ToString(m_level);
    }
    if(m_hasFormatter && m_formatter){
        node["formatter"] = m_formatter->getPattern();
    }
    std::stringstream ss;
    ss<<node;
    return ss.str();
}

bool FileLogAppender::reopen()
{
    Trluper::Lockguard<Spinlock> Lock(m_spinlock);
    if(m_filestream){
        m_filestream.close();
    }
    return Trluper::FileUtil::OpenForWrite(m_filestream, m_filename, std::ios::app);
}


//!LoggerFormatter基类的函数定义
LogFormatter::LogFormatter(const std::string &pattern):m_pattern(pattern)
{
    init();
}

std::string LogFormatter::format(std::shared_ptr<Logger> Logger, LogLevel::Level level, LogEvent::ptr event)
{
    std::stringstream ss;
    for(auto& i: m_items){
        i->format(ss,Logger,level,event);
    }
    return ss.str();
}

std::ostream &LogFormatter::format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
{
    for(auto& i : m_items){
        i->format(os,logger,level,event);
    }
    return os;
}

//*解析格式串，创建对应格式的对象放入m_items
//* %xxx %xxx{xxx} %% 纯非字母形式的符号
void LogFormatter::init()
{   
     try{
        int k = m_pattern.size(),index = 0;
        //记录解析后的对应关心，其中v[i].first 为格式的类型Type即m,n,t这些；vi.second为其是否带有str，无则为空，有则赋值
        std::vector<std::pair<std::string,std::string>> vec;
        std::string Type("");   //记录格式类型
        std::string str("");    //记录自带的str,如{}内的字符串以及纯非字母形式的符号
        while(index < k){
            //m_pattern[i]=='%'&&m_pattern[i+1]!='%，说明为后面为类型，否则格式串出错
            if((index+1) < k && m_pattern[index] == '%' && m_pattern[index+1] != '%'){
                ++index;
                int start_index = index;
                while(index < k && isalpha(m_pattern[index])!=0){
                    ++index;
                }
                //记录格式类型
                Type =  m_pattern.substr(start_index,index-start_index);
                //后续是否有{}内容
                if(index < k && m_pattern[index] == '{'){
                    ++index;
                    start_index = index;
                    while(index < k&&m_pattern[index]!='}'){
                        ++index;
                    }
                    str = m_pattern.substr(start_index,index-start_index);
                    ++index;
                }
                if(Type.empty()){
                     throw Trluper::LogException("Error: The The log pattern string is invalid.", __FILE__, __LINE__);
                }
                //std::cout<<"type: "<<Type<<" str: "<<str<<std::endl;
                vec.push_back(std::pair<std::string,std::string>{Type,str});
                Type.clear();
                str.clear();
            }
            //%%或者其他字符串形式
            else{
                while(index < k&&(m_pattern[index] != '%'||((index+1) < k && m_pattern[index+1] == '%'))){
                    if(m_pattern[index]=='%'){
                        Type+=m_pattern[index];
                        index+=2;
                    }else{
                        Type+=m_pattern[index];
                        ++index;
                    }
                }
                if(Type.empty()){
                     throw Trluper::LogException("Error: The The log pattern string is invalid.", __FILE__, __LINE__);
                }
                //std::cout<<"type: "<<Type<<" str: "<<str<<std::endl;
                vec.push_back(std::pair<std::string,std::string>{Type,Type});
                Type.clear();
            } 
        }
        //预创建相应的formatItem对象
        static std::unordered_map<std::string, std::function<FormatItem::ptr(const std::string& str)> > s_format_items = {
#define XX(str, C) \
        {#str, [](const std::string& fmt) { return FormatItem::ptr(new C(fmt));}}

        XX(m, ContentFormatItem),           //m:消息
        XX(p, LogLevelFormatItem),          //p:日志级别
        XX(r, ElapseFormatItem),            //r:累计毫秒数
        XX(c, LoggerNameFormatItem),        //c:日志名称
        XX(t, ThreadIdFormatItem),          //t:线程id
        XX(N, ThreadNameFormatItem),        //N:线程名称 
        XX(C, CoroutineIdFormatItem),       //C:协程id
        XX(n, NewLineFormatItem),           //n:换行
        XX(d, DateTimeFormatItem),          //d:时间
        XX(f, FileNameFormatItem),          //f:文件名
        XX(F, FuncNameFormatItem),          //F:函数名称
        XX(l, LineFormatItem),              //l:行号
        XX(T, TabFormatItem),               //T:Tab
#undef XX
        };
        for(auto& v: vec){
            if(s_format_items.count(v.first)){
                m_items.emplace_back(s_format_items[v.first](v.second));
            }else{
                m_items.emplace_back(FormatItem::ptr(new StringFormatItem(v.second)));
            }
        }
        //std::cout<< m_items.size()<<std::endl;
        std::cout<<"Log pattern string initial successfully."<<std::endl;
    }catch(Trluper::LogException& e){
        std::cout<<e<<std::endl;
        exit(1);
    }
}


}
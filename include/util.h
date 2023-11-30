#ifndef _UTIL_H_
#define _UTIL_H_

#include <cxxabi.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>
#include <json/json.h>
#include <pthread.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <iomanip>

namespace Trluper{

/// @brief(ms)得到当前时间戳
uint64_t GetCurrentMs();

/// @brief 得到当前调用线程 
pid_t GetThreadID();

/// @brief 获取当前调用协程ID
/// @return 协程ID
uint32_t GetCoroutineID();


/// @brief 字符串的大小写转换
std::string ToUpper(const std::string& str);
/// @brief 字符串的大小写转换
std::string ToLower(const std::string& str);


/// @brief 将时间转为字符串"%Y-%m-%d %H:%M:%S"形式
std::string Time2Str(time_t ts = time(0), const std::string& format = "%Y-%m-%d %H:%M:%S");

/// @brief 将时间字符串转为time_t
time_t Str2Time(const char* str, const char* format = "%Y-%m-%d %H:%M:%S");

/// @brief 提供常见的文件操作
class FileUtil{
public:
    static void ListAllFile(std::vector<std::string>& files, const std::string& path, const std::string& subfix);
    static bool Unlink(const std::string& filename, bool exist = false);
    static bool Mkdir(const std::string& dirname);
    static bool Rm(const std::string& path);
    static bool Mv(const std::string& from, const std::string& to);
    static std::string Dirname(const std::string& filename);
    static std::string Basename(const std::string& filename);
    static bool OpenForRead(std::ifstream& ifs, const std::string& filename,std::ios_base::openmode mode);
    static bool OpenForWrite(std::ofstream& ofs, const std::string& fileame,std::ios_base::openmode mode);
};

}
#endif
#include <dirent.h>
#include "util.h"
namespace Trluper{



//!时间戳
uint64_t GetCurrentMs(){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*1000ul+tv.tv_usec/1000;
}

//!线程ID
pid_t GetThreadID()
{
    return syscall(SYS_gettid);
}

//!协程ID
uint32_t GetCoroutineID()
{
    return 0;
}

//!字符转转大写
std::string ToUpper(const std::string &str)
{
    std::string ret(str);
    std::transform(ret.begin(), ret.end(),ret.begin(),::toupper);
    return ret;
}

//!字符串转小写
std::string ToLower(const std::string &str)
{
    std::string ret(str);
    std::transform(ret.begin(), ret.end(),ret.begin(),::tolower);
    return ret;
}

//!时间戳转字符串
std::string Time2Str(time_t ts, const std::string &format)
{
    struct tm t;
    localtime_r(&ts,&t);
    char buf[64];
    strftime(buf, sizeof(buf), format.c_str(), &t);
    return buf;
}

//!字符串时间转时间戳
time_t Str2Time(const char *str, const char *format)
{
    struct tm t;
    memset(&t,0,sizeof(t));
    if(!strptime(str, format, &t)){
        return 0;
    }
    return mktime(&t);
}

//!获得堆栈信息
bool BackTrace(std::vector<std::string> &bt, int size, int skip)
{
    void** array = (void**)malloc(sizeof(void*)*size);
    int len = backtrace(array, size);
    char** strings = backtrace_symbols(array, len);
    if(NULL == strings){
        free(array);
        return false;
    }
    for(int i = skip; i < len ; ++i){
        bt.push_back(strings[i]);
    }
    free(array);
    free(strings);
    return true;
}

std::string BackTraceString(int size, int skip)
{
    std::vector<std::string> bt;
    BackTrace(bt, size, skip);
    std::stringstream ss;
    for(size_t i = 0; i< bt.size(); ++i){
        ss<<bt[i]<<std::endl;
    }
    return ss.str();
}

//!文件操作
static int __lstat(const char* file, struct stat* st = nullptr) {
    struct stat lst;
    int ret = lstat(file, &lst);
    if(st) {
        *st = lst;
    }
    return ret;
}

static int __mkdir(const char* dirname) {
    if(access(dirname, F_OK) == 0) {
        return 0;
    }
    return mkdir(dirname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void FileUtil::ListAllFile(std::vector<std::string> &files, const std::string &path, const std::string &subfix)
{
    if(access(path.c_str(),0) != 0){//没有权限进入
        std::cout<<"你没有权限访问该文件"<<std::endl;
        return;
    }
    DIR* dir = opendir(path.c_str());
    if(dir == nullptr) return;
    struct dirent* dp = nullptr;
    while((dp = readdir(dir))!=nullptr){
        //是目录
        if(dp->d_type == DT_DIR){
            if(!strcmp(dp->d_name,".")||!strcmp(dp->d_name,"..")){
                continue;
            }
            ListAllFile(files,path+"/"+dp->d_name,subfix);
        }
        //文件
        else if(dp->d_type == DT_REG){
            std::string filename(dp->d_name);
            //所有文件
            if(subfix.empty()){
                files.push_back(path+"/"+filename);
            }else{//指定后缀文件
                if(filename.size() < subfix.size()){
                    continue;
                }
                if(filename.substr(filename.length()-subfix.size()) == subfix){
                    files.push_back(path+"/"+filename);
                }
            }
        }
    }
    closedir(dir);
}

bool FileUtil::Unlink(const std::string &filename, bool exist)
{
    if(!exist && __lstat(filename.c_str())){
        return true;
    }
    return ::unlink(filename.c_str()) == 0;
}



bool FileUtil::Mkdir(const std::string &dirname)
{
    if(access(dirname.c_str(), F_OK) == 0){
        return true;
    }
    char* path = strdup(dirname.c_str());
    char* ptr = strchr(path+1,'/');
    do{
        for(; ptr;*ptr = '/',ptr = strchr(ptr+1,'/')){
            *ptr = '\0';
            if(__mkdir(path) != 0){
                break;
            }
        }
        if(ptr != nullptr){
            break;
        }else if(__mkdir(path) != 0){
            break;
        }
        free(path);
        return true;
    }while(0);
    free(path);
    return false;
}
bool FileUtil::Rm(const std::string &path)
{
    struct stat st;
    if(lstat(path.c_str(),&st)){
            return true;
    }
    if(!(st.st_mode & S_IFDIR)){
            return Unlink(path);
    }
    DIR* dir = opendir(path.c_str());
    if(!dir){
            return false;
    }
    bool ret = true;
    struct dirent* dp = nullptr;
    while((dp = readdir(dir))){
        if(!strcmp(dp->d_name,".")||!strcmp(dp->d_name,"..")){
            continue;
        }
        std::string dirname = path+"/"+dp->d_name;
        ret = Rm(dirname);
    }
    closedir(dir);
    if(::rmdir(path.c_str())){
        ret = false;
    }
    return ret;
}
bool FileUtil::Mv(const std::string &from, const std::string &to)
{
    if(!Rm(to)){
        return false;
    }
    return rename(from.c_str(), to.c_str()) == 0;
}
std::string FileUtil::Dirname(const std::string &filename)
{
    if(filename.empty()){
        return ".";
    }
    auto pos = filename.rfind('/');
    if(pos == 0){
        return "/";
    }else if(pos == std::string::npos){
        return ".";
    }else{
        return filename.substr(0,pos);
    }
}

std::string FileUtil::Basename(const std::string &filename)
{
    if(filename.empty()){
        return filename;
    }
    auto pos = filename.rfind('/');
    if(pos == std::string::npos){
        return filename;
    }else{
        return filename.substr(pos+1);
    }
}

bool FileUtil::OpenForRead(std::ifstream &ifs, const std::string &filename, std::ios_base::openmode mode)
{
    ifs.open(filename, mode);
    return ifs.is_open();
}
bool FileUtil::OpenForWrite(std::ofstream &ofs, const std::string &fileame, std::ios_base::openmode mode)
{
    ofs.open(fileame, mode);
    if(!ofs.is_open()){
        std::string dir = Dirname(fileame);
        Mkdir(dir);
        ofs.open(fileame, mode);
    }
    return ofs.is_open();
}

}
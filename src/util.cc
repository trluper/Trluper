#include "util.h"
namespace Trluper{

/*
* 获得当前的系统时间戳(ms)
*/
uint64_t GetCurrentMs(){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*1000ul+tv.tv_usec/1000;
}

}
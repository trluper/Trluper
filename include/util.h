#ifndef _UTIL_H_
#define _UTIL_H_

#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>

namespace Trluper{
/*
* 获得当前的系统时间戳(ms)
*/
uint64_t GetCurrentMs();

}
#endif
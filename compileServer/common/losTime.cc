
#include "losTime.h"

#include <sys/time.h>

#include <string>

namespace LosCom
{

// 获取 系统时间戳
std::string LosTime::GetTimeStamp()
{
    struct timeval myTime;
    gettimeofday(&myTime, nullptr);
    return std::to_string(myTime.tv_sec);
}

// 获得毫秒时间戳
std::string LosTime::GetTimeMs()
{
    struct timeval myTime;
    gettimeofday(&myTime, nullptr);  // 第二个参数是 区域
    return std::to_string(myTime.tv_sec * 1000 + myTime.tv_usec / 1000);
}

};  // namespace LosCom

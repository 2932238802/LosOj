
#include "losTime.h"

#include <sys/time.h>

namespace LosCom {

// 获取 系统时间戳
std::string Time::GetTimeStamp() {
    struct timeval _time;
    gettimeofday(&_time, nullptr);
    return std::to_string(_time.tv_sec); 
}
};  // namespace LosCom
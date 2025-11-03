#pragma once
#include <iostream>
#include <ostream>
#include <string>

#include "losTime.h"

#define DEBUG_TEST

namespace LosCom
{

// 日志等级
enum class LOG_LEVEL
{
    INFO = 0,
    DEBUG,
    WARNING,
    ERR,  // 出错了 不能继续执行
    FATAL // 最严重的
};

// 日志输出
using cs = const std::string&;

// 开放式 日志
inline std::ostream& Log(cs level, cs file_name, int line)
{
    std::string message = "[";
    message += level;
    message += "]";
    message += "[";
    message += file_name;
    message += "]";
    message += "[";
    message += std::to_string(line);
    message += "]";
    message += "[";
    message += LosTime::GetTimeStamp();
    message += "]";

    // cout 本身是保存 缓存区的
    // 暂存到 cout
    // 不着急 等下打印
    std::cout << message;
    return std::cout;
}
#ifdef DEBUG_TEST
#define LOG(level) Log(#level, __FILE__, __LINE__)
#endif
} // namespace LosCom
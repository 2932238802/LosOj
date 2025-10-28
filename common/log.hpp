#pragma once

#include <iostream>
#include <ostream>
#include <string>

namespace MyLog {

// 日志等级
enum {
    INFO = 0,
    DEBUG,
    WARNING,
    ERR,   // 出错了 不能继续执行
    FATAL  // 最严重的
};

// 日志输出
using cs = const std::string&;
std::ostream& Log(cs level, cs file_name, int line) {
    std::string message = "[";
    message += level;
    message += "]";

    message += "[";
    message += file_name;
    message += "]";

    message += "[";
    message += std::to_string(line);
    message += "]";

    return std::cout;
}

}  // namespace MyLog


#include "losPath.h"

namespace LosCom
{
std::string LosPath::AddSuffix(const std::string file_name,
                               const std::string suffix)
{
    std::string file_path = TEMP_PATH;
    file_path += file_name;
    file_path += suffix;

    return file_path;
}

std::string LosPath::Src(const std::string& file_name)
{
    return AddSuffix(file_name, ".cc");
}

//
std::string LosPath::Exe(const std::string& file_name)
{
    return AddSuffix(file_name, ".exe");
}

std::string LosPath::CoE(const std::string& file_name)
{
    return AddSuffix(file_name, ".compile_error");
}

// 构建标准错误
std::string LosPath::Ser(const std::string& file_name)
{
    return AddSuffix(file_name, ".stderr");
}

// 标准输入
std::string LosPath::Sin(const std::string& file_name)
{
    return AddSuffix(file_name, ".stdin");
}

// 标准输出
std::string LosPath::Sot(const std::string& file_name)
{
    return AddSuffix(file_name, ".stdout");
}

}  // namespace LosCom
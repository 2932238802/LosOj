

#include "losFile.h"

#include <unistd.h>

#include <atomic>
#include <fstream>
#include <string>

#include "losPath.h"
#include "losTime.h"
#include <common/common.h>

namespace LosCom
{
bool LosFile::IsFileExit(const std::string& file_path)
{
    struct stat st;
    return (stat(file_path.c_str(), &st) == 0);
    // 文件存在 返回true
    // 文件不存在返回 false
}

// 生成唯一文件名
std::string LosFile::UniqFile()
{
    // 毫秒时间戳
    // 获得毫秒时间戳
    std::string msStr = LosTime::GetTimeMs();
    static std::atomic_uint id(0); // static 静态之后 之后 都会加 不会重置了
    id++;

    return "losFile_" + msStr + "_" + std::to_string(id);
};

// 形成临时 文件
bool LosFile::WriteFile(const std::string& file_name, const std::string& file_code)
{
    std::ofstream out(file_name);
    if (!out.is_open())
    {
        return false;
    }

    // 成功了
    LOG(INF) << "文件: " << file_name << "写入失败" << std::endl;
    out.write(file_code.c_str(), file_code.size());
    out.close();

    return true;
};

bool LosFile::ReadFile(const std::string& file_name, std::string* str_out, bool key = false)
{
    std::string temp;
    std::ifstream in(file_name);

    if (!in.is_open())
    {
        return false;
    }

    std::string line;
    while (getline(in, line))
    {
        temp += line;
        temp += (key ? "\n" : "");
    }

    in.close();

    *str_out = temp;
    return true;
}

void LosFile::RemoveFiles(const std::string& file_name)
{
    std::string coeFile = LosPath::CoE(file_name);
    std::string exeFile = LosPath::Exe(file_name);
    std::string srcFile = LosPath::Src(file_name);
    std::string serFile = LosPath::Ser(file_name); // stderr
    std::string sotFile = LosPath::Sot(file_name); // stdout
    std::string sinFile = LosPath::Sin(file_name); // stdin

    auto DelFile = [](const std::string& file_name)
    {
        if (IsFileExit(file_name))
        {
            unlink(file_name.c_str());
        }
    };

    DelFile(coeFile);
    DelFile(exeFile);
    DelFile(srcFile);
    DelFile(serFile);
    DelFile(sotFile);
    DelFile(sinFile);
}

} // namespace LosCom

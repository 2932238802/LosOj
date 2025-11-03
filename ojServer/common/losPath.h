
#pragma once
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>

namespace LosCom
{

const std::string TEMP_PATH = "./temp/";

class LosPath
{
   public:
    static std::string AddSuffix(const std::string file_name,
                                 const std::string suffix);

    static std::string Src(const std::string& file_name);

    //
    static std::string Exe(const std::string& file_name);

    // 编译错误
    static std::string CoE(const std::string& file_name);

    // 构建标准错误
    static std::string Ser(const std::string& file_name);

    // 标准输入
    static std::string Sin(const std::string& file_name);

    // 标准输出
    static std::string Sot(const std::string& file_name);

   private:
};

};  // namespace LosCom
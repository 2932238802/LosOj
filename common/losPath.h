
#pragma once
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>

namespace LosCom {

const std::string TEMP_PATH = "./temp/";

class Path {
   public:
    static std::string AddSuffix(const std::string file_name,
                                 const std::string suffix);

    static std::string Src(const std::string& file_name);

    //
    static std::string Exe(const std::string& file_name);

    // 构建标准错误
    static std::string Err(const std::string& file_name);

   private:
};

};  // namespace LosCom
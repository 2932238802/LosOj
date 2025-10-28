
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
                                 const std::string suffix) {
        std::string file_path = TEMP_PATH;
        file_path += file_name;
        file_path += suffix;

        return file_path;
    }

    static std::string Src(const std::string& file_name) {
        return AddSuffix(file_name, ".cpp");
    }

    //
    static std::string Exe(const std::string& file_name) {
        return AddSuffix(file_name, ".exe");
    }

    // 构建标准错误
    static std::string Err(const std::string& file_name) {
        return AddSuffix(file_name, ".stderr");
    }

   private:
};

class File {
   public:
    static bool IsFileExit(const std::string& file_path) {
        struct stat st;
        return (stat(file_path.c_str(), &st) == 0);
        // 文件存在 返回true
        // 文件不存在返回 false
    }
};

};  // namespace LosCom
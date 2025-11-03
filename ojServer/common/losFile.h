#pragma once
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>

namespace LosCom
{
class LosFile
{
   public:
    static bool IsFileExit(const std::string& file_path);
    static std::string UniqFile();
    static bool WriteFile(const std::string& file_name,
                          const std::string& file_code);
    static bool ReadFile(const std::string& file_name, std::string* str_out,
                         bool key);

    static void RemoveFiles(const std::string& file_name);
};
}  // namespace LosCom
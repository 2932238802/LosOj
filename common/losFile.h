#pragma once
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>

namespace LosCom {
class File {
   public:
    static bool IsFileExit(const std::string& file_path);
};
}  // namespace LosCom
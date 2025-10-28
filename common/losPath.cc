

#include "losPath.h"

namespace LosCom {
std::string Path::AddSuffix(const std::string file_name,
                            const std::string suffix) {
    std::string file_path = TEMP_PATH;
    file_path += file_name;
    file_path += suffix;

    return file_path;
}

std::string Path::Src(const std::string& file_name) {
    return AddSuffix(file_name, ".cc");
}

//
std::string Path::Exe(const std::string& file_name) {
    return AddSuffix(file_name, ".exe");
}

// 构建标准错误
std::string Path::Err(const std::string& file_name) {
    return AddSuffix(file_name, ".stderr");
}

}  // namespace LosCom
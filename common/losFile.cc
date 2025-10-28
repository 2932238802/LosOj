

#include "losFile.h"

namespace LosCom {
bool File::IsFileExit(const std::string& file_path) {
    struct stat st;
    return (stat(file_path.c_str(), &st) == 0);
    // 文件存在 返回true
    // 文件不存在返回 false
}

}  // namespace LosCom

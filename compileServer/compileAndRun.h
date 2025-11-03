#pragma once
#include <jsoncpp/json/json.h>

#include "compiler.h"
#include "runner.h"

namespace LosServer
{
using namespace LosCom;

enum class ErrorStateCode
{
    CODE_EMPTY = -1,
    SERVER_ERR = -2,  // 服务器内部错误
    COMPILE_ERR = -3, // 编译错误
};

class CompileAndRun
{
  public:
    // 输入参数
    static void Start(const std::string& in_json,
                      std::string* out_json); //

  private:
};

} // namespace LosServer
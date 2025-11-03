// CompileAndRun.cpp (优雅且安全的重构版)
#include "compileAndRun.h"

#include <jsoncpp/json/json.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>

#include "common/common.h"
#include "common/losFile.h"
#include "common/losPath.h"
#include "compiler.h"
#include "runner.h"

namespace LosServer
{

static void SerializeResponse(const Json::Value& value, std::string* out_json)
{
    Json::StyledWriter writer;
    *out_json = writer.write(value);
}

void CompileAndRun::Start(const std::string& in_json, std::string* out_json)
{
    Json::Value request_json;
    Json::Reader reader;
    Json::Value response_json;

    if (!reader.parse(in_json, request_json))
    {
        LOG(ERR) << "JSON 解析失败: " << in_json << std::endl;
        response_json["state"] = (int)ErrorStateCode::SERVER_ERR;
        response_json["reason"] = "无效的请求格式 (JSON Parse Error)";
        SerializeResponse(response_json, out_json);
        return;
    }

    std::string code = request_json["code"].asString();
    int cpuLimit = request_json.get("cpu_limit", 1000).asInt(); // 默认1秒
    int memLimit = request_json.get("mem_limit", 4000).asInt();

    std::string fileToUnique = LosFile::UniqFile();
    struct TempFileGuard
    {
        std::string name;
        ~TempFileGuard()
        {
            if (!name.empty())
            {
                LosFile::RemoveFiles(name);
            }
        }
    } file_guard{fileToUnique};

    if (code.empty())
    {
        response_json["state"] = (int)ErrorStateCode::CODE_EMPTY;
        response_json["reason"] = "用户提交的代码是空的";
    }
    else if (!LosFile::WriteFile(LosPath::Src(fileToUnique), code))
    {
        LOG(ERR) << "文件写入失败, 服务端问题" << std::endl;
        response_json["state"] = (int)ErrorStateCode::SERVER_ERR;
        response_json["reason"] = "文件写入失败, 服务端问题";
    }
    else if (!Compiler::Compile(fileToUnique))
    {
        std::string compile_error;
        LosFile::ReadFile(LosPath::CoE(fileToUnique), &compile_error, true);
        response_json["state"] = (int)ErrorStateCode::COMPILE_ERR;
        response_json["reason"] = compile_error;
    }
    else
    {
        LOG(INF) << "开始运行: " << fileToUnique << " [CPU Limit: " << cpuLimit
                 << "ms, Mem Limit: " << memLimit << "B]" << std::endl;
        int right_number = 0;
        int ret = Runner::Run(fileToUnique, cpuLimit, memLimit, &right_number);

        if (ret == 0)
        {
            std::string std_output, std_error;
            LosFile::ReadFile(LosPath::Sot(fileToUnique), &std_output, true);
            LosFile::ReadFile(LosPath::Ser(fileToUnique), &std_error, true);

            response_json["state"] = 0;
            response_json["reason"] = "";
            response_json["stdout"] = std_output;
            response_json["stderr"] = std_error;
            response_json["right_number"] = right_number;
        }
        else if (ret < 0)
        {
            LOG(ERR) << "Runner 返回内部错误: " << ret << std::endl;
            response_json["state"] = (int)ErrorStateCode::SERVER_ERR;
            response_json["reason"] = "服务端在运行代码时发生未知错误";
        }
        else
        {
            std::string reason = Runner::SignoToDesc(ret);
            LOG(ERR) << "运行出错, 信号: " << ret << ", 描述: " << reason << std::endl;
            response_json["state"] = ret; // 将信号量直接作为 state 返回
            response_json["reason"] = reason;

            std::string std_output, std_error;
            LosFile::ReadFile(LosPath::Sot(fileToUnique), &std_output, true);
            LosFile::ReadFile(LosPath::Ser(fileToUnique), &std_error, true);
            response_json["stdout"] = std_output;
            response_json["stderr"] = std_error;
        }
    }

    SerializeResponse(response_json, out_json);
};

} // namespace LosServer

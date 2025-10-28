#include "compiler.h"

#include "losPath.h"

namespace LosServer {

bool Compiler::Compile(const std::string &file_name) {
    pid_t res = fork();
    if (res == 0) {
        // 保存错误信息
        int stderr_handle = open(LosCom::Path::Err(file_name).c_str(),
                                 O_CREAT | O_WRONLY, 0644);

        if (stderr_handle < 0) {
            LOG(LOG_LEVEL::ERROR) << "打开错误文件失败" << "\n";

            exit(1);
        }

        // 重定向 标准错误
        // 就反着 理解就可以了 就是 后面都到前面去了 ~
        dup2(stderr_handle, 2);

        // 调用编译器
        // 程序替换的功能
        execlp("g++", "g++", "-o", LosCom::Path::Exe(file_name).c_str(),
               LosCom::Path::Src(file_name).c_str(), "-std=c++17", nullptr);

        LOG(LOG_LEVEL::ERROR) << "调用编译器失败 可能是参数错误" << "\n";

        exit(1);

    } else if (res < 0) {
        LOG(LOG_LEVEL::ERROR) << "fork 失败" << "\n";

        return false;
    } else {
        // 父进程
        int state_code = 0;
        waitpid(res, &state_code, 0);

        if (WIFEXITED(state_code) && WEXITSTATUS(state_code) == 0) {
            LOG(LOG_LEVEL::INFO)
                << LosCom::Path::Src(file_name) << "文件 :编译成功...";
            return LosCom::File::IsFileExit(LosCom::Path::Exe(file_name));
        } else {
            LOG(LOG_LEVEL::ERROR) << "编译失败或子进程异常退出\n";
            return false;
        }

        return LosCom::File::IsFileExit(LosCom::Path::Exe(file_name));
    }

    LOG(LOG_LEVEL::ERROR) << "编译失败 没有形成可执行程序" << "\n";
    return false;
}
}  // namespace LosServer

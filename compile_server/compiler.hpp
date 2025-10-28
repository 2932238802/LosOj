#pragma once
#include <fcntl.h>
#include <sched.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

#include "../common/utils.hpp"

// 只负责 代码的编译

namespace LosCompiler {
class Compiler {
   public:
    Compiler();
    ~Compiler();

    static bool Compile(const std::string& file_name) {
        pid_t res = fork();
        if (res == 0) {
            // 保存错误信息
            int stderr_handle = open(LosCom::Path::Err(file_name).c_str(),
                                     O_CREAT | O_WRONLY, 0644);

            if (stderr_handle < 0) {
                exit(1);
            }

            // 重定向 标准错误
            dup2(stderr_handle,
                 2);  // 就反着 理解就可以了 就是 后面都到前面去了 ~

            // 调用编译器
            // 程序替换的功能
            execlp("g++", "-o", LosCom::Path::Exe(file_name).c_str(),
                   LosCom::Path::Src(file_name).c_str(), "-std=c++17", nullptr);
            exit(1);
        } else if (res < 0) {
            return false;
        } else {
            // 父进程
            waitpid(res, nullptr, 0);

            // 子进程完成了 检测一下
            return LosCom::File::IsFileExit(LosCom::Path::Exe(file_name));
        }

        return false;
    }

   private:
};
};  // namespace LosCompiler
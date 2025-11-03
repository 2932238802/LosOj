#include "runner.h"

#include <cstdlib>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <csignal>
#include <string>

#include "common/common.h"
#include "common/losLog.h"

namespace LosServer
{

using namespace LosCom;

int Runner::Run(const std::string& path, int cpu_limit, int mem_limit, int* right_number)
{
    pid_t pid = fork();

    /*
    1. 程序写完 代码好的
    2. 代码跑完 结果不正确
    3.

    */
    auto OpenFile = [](const std::string& file_name, int flag)
    {
        int fd = open(file_name.c_str(), flag, 0644);
        return fd;
    };

    std::string executeFileName = LosPath::Exe(path);
    std::string stdinFileName = LosPath::Sin(path);
    std::string stdoutFileName = LosPath::Sot(path);
    std::string stderrFileName = LosPath::Ser(path);

    int stdinFd = OpenFile(stdinFileName, O_CREAT | O_RDONLY);
    int stdoutFd = OpenFile(stdoutFileName, O_CREAT | O_WRONLY);
    int stderrFd = OpenFile(stderrFileName, O_CREAT | O_WRONLY);

    auto CloseFile = [=]()
    {
        close(stdinFd);
        close(stdoutFd);
        close(stderrFd);
    };

    if (stdinFd < 0 || stdoutFd < 0 || stderrFd < 0)
    {
        LOG(LOG_LEVEL::ERR) << "运行时错误";
        return -1; // 打开文件失败
    }

    if (pid == 0)
    {
        // 重定向 再 写入文件
        dup2(stdinFd, 0);
        dup2(stdoutFd, 1);
        dup2(stderrFd, 2);

        SetProcLimit(cpu_limit, mem_limit);
        // 左边是执行谁
        // 右边是怎么执行
        // 第三个参数 是 额外的参数z配置
        execl(executeFileName.c_str(), executeFileName.c_str(), nullptr);
        exit(-1);
    }
    else if (pid < 0)
    {
        LOG(LOG_LEVEL::ERR) << "创建子进程失败";

        CloseFile();

        // 代表创建子进程失败
        return -2;
    }
    else
    {
        // 关闭完毕
        CloseFile();
        int state = 0;
        waitpid(pid, &state, 0);

        if (WIFEXITED(state))
        {
            *right_number = WEXITSTATUS(state);
        }

        LOG(LOG_LEVEL::INFO) << "运行完毕,state: " << (state & 0x7F) << "\n";
        // 7 * 16 + 15 = 127
        return state & 0x7F;
    }
}

// 设置内存限制
// 提供 设置进程占用资源
void Runner::SetProcLimit(int cpu_limit, int mem_limit)
{
    // 设置cpu时长
    struct rlimit cpuRlimit;
    cpuRlimit.rlim_max = RLIM_INFINITY;
    cpuRlimit.rlim_cur = cpu_limit;
    setrlimit(RLIMIT_CPU, &cpuRlimit);

    struct rlimit memRlimit;
    memRlimit.rlim_cur = mem_limit * 1024;
    memRlimit.rlim_max = RLIM_INFINITY; // kb
    setrlimit(RLIMIT_AS, &memRlimit);
}

// 通过信号 转变为 描述信息
std::string Runner::SignoToDesc(int signo)
{
    std::string str;
    switch (signo)
    {
    case SIGABRT: // 6
    {
        str = "内存超过了范围";
        break;
    }
    case SIGXCPU:
    {
        str = "CPU使用超时";
        break;
    }
    case SIGFPE:
    {
        str = "浮点数溢出错误";
        break;
    }
    case SIGSEGV: // 11
    {
        str = R"(
            发生段错误
            原因如下:
            1. 解引用空指针
            2. 访问数组时越界
            3. 试图修改只读的内存
            4. 栈溢出
        )";
        break;
    }
    default:
    {
        str = ("未知" + std::to_string(signo));
        break;
    }
    }

    return str;
}

} // namespace LosServer
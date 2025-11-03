#pragma once
#include <fcntl.h>
#include <sched.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

#include "common/common.h"

// 只负责 代码的编译

namespace LosServer
{

using namespace LosCom;

class Compiler
{
   public:
    Compiler();
    ~Compiler();

    static bool Compile(const std::string& file_name);

   private:
};

}  // namespace LosServer
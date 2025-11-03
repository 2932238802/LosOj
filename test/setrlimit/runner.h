#pragma once

#include <iostream>
#include <string>

namespace LosServer
{

class Runner
{
   public:
    Runner();
    ~Runner();

   public:
    static int Run(const std::string &path, int cpu_limit, int mem_limit);
    static void SetProcLimit(int cpu_limit, int mem_limit);
};

}  // namespace LosServer
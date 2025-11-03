#pragma once
#include <string>
namespace LosCom
{

class LosTime
{
   public:
    static std::string GetTimeStamp();
    static std::string GetTimeMs();
};

}  // namespace LosCom
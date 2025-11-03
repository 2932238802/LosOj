#pragma once
#include <string>
#include <vector>
namespace LosCom
{
class LosStr
{
   public:
    static void SplitString(const std::string& str,
                            std::vector<std::string>* target,
                            const std::string& sep);
};

};  // namespace LosCom
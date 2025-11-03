#include "losStr.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/constants.hpp>
#include <boost/algorithm/string/split.hpp>
namespace LosCom
{
void LosStr::SplitString(const std::string& str, std::vector<std::string>* target,
                         const std::string& sep)
{
    boost::split((*target), str, boost::is_any_of(sep), boost::algorithm::token_compress_on);
}

}; // namespace LosCom
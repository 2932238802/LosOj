#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/constants.hpp>
#include <boost/algorithm/string/split.hpp>
#include <iostream>
#include <vector>
using namespace std;
int main()
{
    std::vector<std::string> vec;

    const std::string str = "ni   shi zhuma";

    boost::split(vec, str, boost::is_any_of(" "),
                 boost::algorithm::token_compress_on);

    for (const auto& a : vec)
    {
        cout << a << ":";
    }
    cout << endl;

    return 0;
}
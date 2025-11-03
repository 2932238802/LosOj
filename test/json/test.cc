
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>

#include <iostream>
using namespace std;
int main()
{
    Json::Value root;

    root["code"] = "123";
    root["name"] = "lsj";
    root["age"] = 14;

    Json::StyledWriter writer;
    std::string s{writer.write(root)};

    cout << s << endl;

    return 0;
}
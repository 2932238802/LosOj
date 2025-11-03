#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>

#include <cstdlib>

#include "compileAndRun.h"
#include "httplib.h"

using namespace LosServer;

void Usage(const std::string& proc)
{
    std::cerr << "Usage: " << "\n\t" << proc << " port" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        return 1;
    }

    httplib::Server svr;
    svr.Get("/hello",
            [](const httplib::Request& req, httplib::Response& res)
            {
                // res 要改写 所以 要引用 没有const

                res.set_content(u8"hello ~ 你好 ~", "text/plain; charset=utf-8");
            });

    svr.Post("/compile",
             [](const httplib::Request& req, httplib::Response& res)
             {
                 //  json string
                 LOG(INF) << " /compile 收到请求 开始编译" << std::endl;
                 std::string in_json = req.body;
                 std::string out = "";

                 if (!in_json.empty())
                 {
                     CompileAndRun::Start(in_json, &out);
                     res.set_content(out, "application/json; charset=utf-8");
                 }
             });

    svr.listen("0.0.0.0", atoi(argv[1]));

    return 0;
}

// int main()
// {
//     // std::cout << "Hello, World!" << std::endl;

//     LosServer::CompileAndRun car;

//     std::string str;

//     Json::Value in_val;
//     in_val["code"] = R"(
//         #include<iostream>
//         using namespace std;
//         int main()
//         {
//             int n = 0;
//             int sum = 0;
//             while(1);
//             for(int i = 0; i < 10 ; i++)
//             {
//                 sum+=i;
//             }
//             cout << "sum的和: " << sum << endl;
//             return 0;
//         }
//     )";

//     in_val["input"] = "10";
//     in_val["cpu_limit"] = 1;
//     // 1024 * 1024 = 1 MB   1兆
//     //
//     in_val["mem_limit"] = 10240 * 3;

//     Json::FastWriter writer;
//     str = writer.write(in_val);

//     std::string str_out = "123";
//     CompileAndRun::Start(str, &str_out);

//     std::cout << str_out << std::endl;
//     return 0;
// }
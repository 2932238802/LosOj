
#include <csignal>
#include <string>

#include <common/common.h>
#include <httplib.h>
#include <ojController.h>
#include <ojModel.h>
#include <ojView.h>

inline const std::string IP = "0.0.0.0";
inline const int PORT = 8080;
inline const std::string TEXT_TYPE = "text/plain; charset=utf-8";
inline const std::string HTML_TYPE = "text/html; charset=utf-8";
inline const std::string JSON_TYPE = "application/json; charset=utf-8";

using namespace LosCom;

LosOj::OjController controller;

void Recovery(int code) { controller.RecoveryMachine(); }

int main()
{

    signal(SIGQUIT, Recovery);

    httplib::Server server;
    LosOj::OjController controller;

    // 获取 所有的题目列表
    // 按照 题目的编号 获取 题目的内容
    // 用户 提交代码 判题 功能

    server.Get("/all_questions",
               [&controller](const httplib::Request& req, httplib::Response& res)
               {
                   LOG(INFO) << "/all_questions 收到请求" << std::endl;
                   // TODO
                   std::string html;
                   controller.allQuestions(&html);
                   res.set_content(html, HTML_TYPE);
               });

    server.Get(R"(/question/(\d+))",
               [&controller](const httplib::Request& req, httplib::Response& res)
               {
                   std::string number = req.matches[1];
                   std::string html;

                   LOG(INFO) << " /question/ 收到请求" << number << std::endl;

                   controller.oneQuestion(number, &html);
                   res.set_content(html, HTML_TYPE);
               });

    server.Post(R"(/judge/(\d+))",
                [&controller](const httplib::Request& req, httplib::Response& res)
                {
                    // TODO
                    std::string number = req.matches[1];
                    std::string rst = "";
                    LOG(INFO) << number << std::endl;

                    controller.judgeQuestion(number, req.body, &rst);
                    res.set_content(rst, JSON_TYPE);
                    // res.set_content("这个是判定的一道题" + number, TEXT_TYPE);
                });

    server.set_base_dir("./wwwroot");

    // set_mount_point 设置节点
    server.set_mount_point("/static", "./static");

    server.listen(IP, PORT);

    return 0;
}


#include "ojView.h"

#include <common/common.h>
#include <fstream>
#include <inja/inja.hpp>
#include <iterator>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>

namespace LosOj
{

using namespace LosCom;

OjView::OjView()
{
    // TODO
}
OjView::~OjView()
{
    // TODO
}

/**
 * @brief 获取多个题目
 *
 * @return true
 * @return false
 */
bool OjView::getViewOfAllQuestions(const std::vector<Question>& all_questions, std::string* html)
{
    // 显示所有的题目列表
    // 还有题目的列表
    std::string srcHtmls = TEMP_PATH + "all_questions.html";

    // 形成数据
    nlohmann::json data;

    data["questions"] = all_questions;

    // data["questions"] = nlohmann::json::array();
    // for (const auto& q : all_questions)
    // {
    //     nlohmann::json json_;
    //     json_["number"] = q.id;
    //     json_["star"] = q.star;
    //     json_["title"] = q.title;

    //     data["questions"].push_back(json_);
    // }

    std::fstream in(srcHtmls);

    if (!in.is_open())
    {
        LOG(ERR) << "模板打开失败";
        return false;
    }

    std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    in.close();

    try
    {
        inja::Environment env;
        *html = env.render(content, data);
    }
    catch (const std::exception& e)
    {
        LOG(ERR) << "inja 环境打开失败: " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool OjView::getViewOfOneQuestions(Question* q, std::string* html)
{
    std::string srcHtmls = TEMP_PATH + "one_question.html";
    std::fstream in(srcHtmls);
    if (!in.is_open())
    {
        LOG(ERR) << "文件打开失败" << std::endl;
        return false;
    }

    nlohmann::json data;
    data["id"] = q->id;
    data["title"] = q->title;
    data["desc"] = q->desc;
    data["cpu_limit"] = q->cpu_limit;
    data["mem_limit"] = q->mem_limit;
    data["header"] = q->header;
    data["star"] = q->star;

    std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    LOG(INFO) << "数据已经保存到string内部 下一步进行inja渲染" << std::endl;

    try
    {
        inja::Environment env;
        *html = env.render(content, data);

        LOG(INFO) << "渲染成功,html的大小是: " << html->size() << std::endl;
    }
    catch (const std::exception& e)
    {
        LOG(ERR) << "inja 环境打开失败: " << e.what() << std::endl;
        return false;
    }

    return true;
}

}; // namespace LosOj
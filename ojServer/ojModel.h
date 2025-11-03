#pragma once

#include "mysql/mysql.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/losLog.h"
#include <nlohmann/json.hpp>
#define MYSQL_MODEL

namespace LosOj
{
struct Question
{
    std::string id;
    std::string title;
    std::string star; // 难度
    int cpu_limit;
    int test_number; // 测试用例个数
    int mem_limit;
    std::string desc;   // 题目的描述
    std::string header; // 预设代码
    std::string tail;   // 题目的测试用例 需要和header 拼接
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Question, id, title, star, cpu_limit, mem_limit, desc, header,
                                   tail, test_number);

    // 打印自身
    void Print()
    {
        std::cout << "id: " << id;
        std::cout << " title: " << title;
        std::cout << " star: " << star << std::endl;
        std::cout << " cpu_limit: " << cpu_limit << std::endl;
        std::cout << " mem_limit: " << mem_limit << std::endl;
        std::cout << " test_number: " << test_number << std::endl;
        std::cout << " desc: " << desc << std::endl;
        std::cout << " header: " << header << std::endl;
        std::cout << " tail: " << tail << std::endl;
    }
};

inline const std::string QUESTIONS_PATH = "./questions/questions.list";
inline const std::string QUESTIONS_FOLDERS_PATH = "./questions/";

#ifdef MYSQL_MODEL
const std::string tableName = "oj_questions";
const std::string host = "127.0.0.1";
const std::string user = "oj_client";
const std::string pd = "Lsj20041021";
const std::string db = "Losoj";
const int port = 3306;

#endif

class OjModel
{
  public:
    OjModel();
    ~OjModel();

#ifdef FILE_MODEL
    bool LoadQuestionList(const std::string& questions); // 加载视频列表
#endif

#ifdef MYSQL_MODEL
    bool QueryMysql(const std::string& sql, std::vector<Question>* out);
#endif

    bool GetAllQuestions(std::vector<Question>* out);
    bool GetOneQuestion(const std::string& questions_id, Question* q);

  private:
#ifdef MYSQL_MODEL
    MYSQL* mysql;
#endif
    std::unordered_map<std::string, Question> questionsLists;
};

}; // namespace LosOj
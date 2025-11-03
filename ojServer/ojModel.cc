#include "ojModel.h"

#include "common/common.h"
#include "mysql/mysql.h"
#include <cassert>
#include <fstream>
#include <string>
#include <vector>

namespace LosOj
{
using namespace LosCom;

OjModel::OjModel()
{ // TODO
#ifdef FILE_MODEL
    assert(LoadQuestionList(QUESTIONS_PATH));
#endif

#ifdef MYSQL_MODEL
    mysql = mysql_init(nullptr);
    if (mysql == nullptr)
    {
        LOG(FATAL) << "mysql_init failed!" << std::endl;
        return;
    }

    if (mysql_real_connect(mysql, host.c_str(), user.c_str(), pd.c_str(), db.c_str(), port, nullptr,
                           0) == nullptr)
    {
        LOG(FATAL) << "Failed to connect to database: " << mysql_error(mysql) << std::endl;
        mysql_close(mysql);
        mysql = nullptr;
        return;
    }
    mysql_set_character_set(mysql, "utf8");
#endif
}

OjModel::~OjModel()
{
#ifdef MYSQL_MODEL
    if (mysql != nullptr)
    {
        mysql_close(mysql);
        LOG(INFO) << "Database connection closed." << std::endl;
    }
#endif
}

#ifdef MYSQL_MODEL
bool OjModel::QueryMysql(const std::string& sql, std::vector<Question>* out)
{
    if (nullptr == mysql)
    {
        LOG(FATAL) << "连接数据库失败" << std::endl;
        return false;
    }

    if (mysql_query(mysql, sql.c_str()) != 0)
    {
        // 说明失败了
        LOG(ERR) << "查询数据库失败" << std::endl;
        return false;
    }

    LOG(INFO) << "数据库查询成功!" << std::endl;

    // 提取结果
    MYSQL_RES* res = mysql_store_result(mysql);

    if (res == nullptr)
    {
        if (mysql_field_count(mysql) > 0)
        {
            LOG(ERR) << "内部发生错误 " << mysql_error(mysql) << std::endl;
        }
        else
        {
            // 这个项目 应该不会触发这个
            LOG(INFO) << "执行成功" << std::endl;
        }
    }

    int rows = mysql_num_rows(res);
    int cols = mysql_num_fields(res);

    for (int i = 0; i < rows; i++)
    {
        MYSQL_ROW row = mysql_fetch_row(res); // 其实就是char** 二级指针
        struct Question q;
        //   create table if not exists `oj_questions`(
        // 	`id` int primary key auto_increment comment "题目的编号",
        // 	`title` varchar(128) NOT NULL comment "题目的标题",
        //  `star` varchar(8) NOT NULL comment "题目的难度",
        // 	`desc` text NOT NULL comment "题目的描述",
        //  `header` text not NULL comment "题目的预设代码",
        //  `tail` text not NULL comment "题目的测试用例",
        //  `test_number` int default 2 comment "测试用例的个数",
        //  `mem_limit` int default 1 comment "超时时间",
        //  `cpu_limit` int default 30000 comment "内存开辟"
        // )engine=InnoDB default charset=utf8;
        q.id = row[0];
        q.title = row[1];
        q.star = row[2];
        q.desc = row[3];
        q.header = row[4];
        q.tail = row[5];
        q.test_number = atoi(row[6]);
        q.mem_limit = atoi(row[7]);
        q.cpu_limit = atoi(row[8]);
        out->push_back(q);
    }

    mysql_free_result(res);
    return true;
}

#endif

#ifdef FILE_MODEL
bool OjModel::LoadQuestionList(const std::string& path)
{
    std::ifstream in(path);

    if (!in.is_open())
    {
        LOG(ERR) << "加载题库失败" << std::endl;
        return false;
    }

    // 按行读取
    std::string line;
    while (std::getline(in, line))
    {
        std::vector<std::string> tokens;
        LosStr::SplitString(line, &tokens, " ");

        if (tokens.size() != 6)
        {
            // 这个配置文件就有问题
            LOG(WARNING) << "加载部分题目失败: " << tokens.size() << std::endl;
            continue;
        }

        Question q;
        q.id = tokens[0];
        q.title = tokens[1];
        q.star = tokens[2];
        q.cpu_limit = atoi(tokens[3].c_str());
        q.mem_limit = atoi(tokens[4].c_str());
        q.test_number = atoi(tokens[5].c_str());

        std::string questionFolder = QUESTIONS_FOLDERS_PATH;
        questionFolder += q.id; // 路径有了
        questionFolder += '/';

        std::string descPath = questionFolder + "desc.txt";
        std::string headerPath = questionFolder + "header.cc";
        std::string tailPath = questionFolder + "tail.cc";

        LosCom::LosFile::ReadFile(descPath, &(q.desc), true);
        LosCom::LosFile::ReadFile(headerPath, &(q.header), true);
        LosCom::LosFile::ReadFile(tailPath, &(q.tail), true);

        questionsLists.insert({q.id, q});
    }

    LOG(INFO) << "加载题库成功" << std::endl;
    in.close();

    return true;
}
#endif

bool OjModel::GetAllQuestions(std::vector<Question>* out)
{
#ifdef FILE_MODEL
    if (questionsLists.size() == 0)
    {
        LOG(ERR) << "获取所有题目失败" << std::endl;
        return false;
    }

    for (const auto& [a, b] : questionsLists)
    {
        out->push_back(b); // 问题放进去
    }
    return true;
#endif

#ifdef MYSQL_MODEL
    std::string sql = "select * from ";
    sql += tableName;
    return QueryMysql(sql, out);
#endif
}

bool OjModel::GetOneQuestion(const std::string& questions_id, Question* q)
{
#ifdef FILE_MODEL

    const auto& iter = questionsLists.find(questions_id);

    if (iter == questionsLists.end())
    {
        LOG(ERR) << "获取单个题目失败:" << questions_id << std::endl;
        return false;
    }

    LOG(INFO) << "获取单个题目成功" << std::endl;
    (*q) = iter->second;
    q->Print();

    return true;
#endif

#ifdef MYSQL_MODEL
    bool res = false;
    std::string sql = "select * from ";
    sql += tableName;
    sql += " where id = ";
    sql += questions_id;

    std::vector<Question> rlt;
    if (QueryMysql(sql, &rlt))
    {
        if (rlt.size() == 1)
        {
            res = true;
            *q = rlt[0];
        }
    }

    return res;

#endif
}

} // namespace LosOj

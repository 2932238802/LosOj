

#include "ojController.h"

#include "losFile.h"
#include "losStr.h"
#include "ojModel.h"
#include "ojView.h"
#include <algorithm>
#include <cassert>
#include <common/common.h>
#include <cstdint>
#include <fstream>
#include <httplib.h>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

/////////////////////////////////////////////
namespace LosOj
{
Machine::Machine() : ip(""), port(0), load(0), mtx(nullptr) {}
Machine::~Machine() {}

void Machine::updateLoad(bool mode)
{
    if (mtx)
        mtx->lock();
    mode == true ? load++ : load--;
    if (mtx)
        mtx->unlock();
}

uint64_t Machine::getLoad() const
{
    uint64_t loadCopy = 0;
    if (mtx)
        mtx->lock();
    loadCopy = load;
    if (mtx)
        mtx->unlock();

    return loadCopy;
}

void Machine::ResetLoad()
{
    std::lock_guard<std::mutex> guard(*mtx);
    load = 0;
}

} // namespace LosOj
/////////////////////////////////////////////

/////////////////////////////////////////////
namespace LosOj
{
LoadBlance::LoadBlance()
{
    assert(LoadConf(MACHINES_PATH));
    LOG(INFO) << "加载主机信息成功" << std::endl;
}
LoadBlance::~LoadBlance() {}

bool LoadBlance::LoadConf(const std::string& path)
{
    std::fstream in(path);
    if (!in.is_open())
    {
        LOG(FATAL) << "文件打开失败" << std::endl;
        return false;
    }

    std::string line;

    while (std::getline(in, line))
    {
        std::vector<std::string> tokens;
        LosStr::SplitString(line, &tokens, ":");
        if (tokens.size() != 2)
        {
            LOG(WARNING) << "切分 主机ip和端口号失败 size: " << tokens.size() << std::endl;
            for (auto& a : tokens)
            {
                std::cout << a << " ";
            }
            continue;
        }

        Machine m;
        m.ip = tokens[0];
        m.port = atoi(tokens[1].c_str());
        m.load = 0;
        m.mtx = new std::mutex();
        onlineMachines.push_back(machines.size());
        machines.push_back(m);
    }
    in.close();
    return true;
}

bool LoadBlance::smartChoose(int* id, Machine** m)
{
    std::lock_guard<std::mutex> guard(mtx);
    // 随机数和轮询
    int onlineNumber = onlineMachines.size();
    if (onlineNumber == 0)
    {
        // 都离线
        LOG(FATAL) << "后端的主线全部离线 服务器崩溃 选择失败" << std::endl;
        return false;
    }

    // 找到负载的最小的机器
    uint64_t minLoad = machines[onlineMachines[0]].getLoad();
    *id = onlineMachines[0];
    *m = &machines[onlineMachines[0]];
    for (int i = 0; i < onlineNumber; i++)
    {
        int nowId = onlineMachines[i];
        uint64_t newLoad = machines[nowId].getLoad();
        if (minLoad > newLoad)
        {
            minLoad = newLoad;
            *id = nowId;
            *m = &machines[nowId];
        }
    }

    return true;
}

void LoadBlance::off(int id)
{

    std::lock_guard<std::mutex> guard(mtx);

    for (auto iter = onlineMachines.begin(); iter != onlineMachines.end(); iter++)
    {
        if (*iter == id)
        {
            machines[id].ResetLoad();
            onlineMachines.erase(iter);
            outlineMachines.push_back(id);
            break; // 不影响迭代器失效
        }
    }
}

void LoadBlance::on()
{
    LOG(INF) << "LoadBlance::on()" << std::endl;

    std::lock_guard<std::mutex> guard(mtx);
    onlineMachines.insert(onlineMachines.end(), outlineMachines.begin(), outlineMachines.end());
    outlineMachines.erase(outlineMachines.begin(), outlineMachines.end());
}

void LoadBlance::show()
{
    std::lock_guard<std::mutex> guard(mtx);
    std::cout << "------------------------------" << std::endl;
    std::cout << "当前在线主机列表: " << std::endl;
    for (auto& iter : onlineMachines)
    {
        std::cout << iter << " ";
    }
    std::cout << std::endl;
    std::cout << "------------------------------" << std::endl;
    std::cout << "当前离线主机列表: " << std::endl;
    for (auto& iter : outlineMachines)
    {
        std::cout << iter << " ";
    }
    std::cout << std::endl;
    std::cout << "------------------------------" << std::endl;
}

} // namespace LosOj
/////////////////////////////////////////////

/////////////////////////////////////////////
namespace LosOj
{

OjController::OjController()
{
    model = std::make_shared<OjModel>();
    view = std::make_shared<OjView>();
    loadBlance = std::make_shared<LoadBlance>();
}

OjController::~OjController()
{
    // TODO
}

void OjController::RecoveryMachine() { loadBlance->on(); }

bool OjController::allQuestions(std::string* html)
{
    bool ret = true;
    std::vector<Question> all;
    if (model->GetAllQuestions(&all))
    {
        // 题目获取成功
        sort(all.begin(), all.end(), [](const Question& q, const Question& p)
             { return atoi(q.id.c_str()) < atoi(p.id.c_str()); });
        view->getViewOfAllQuestions(all, html);
    }
    else
    {
        LOG(ERR) << "获取网页失败(所有题目)";
        *html = "获取网页失败(所有题目)";
        ret = false;
    }

    return ret;
}

/**
 * @brief
 *
 * @param id 问题的id
 * @param html 输出的网页
 * @return true
 * @return false
 */
bool OjController::oneQuestion(const std::string& id, std::string* html)
{
    Question q;
    bool ret = true;
    if (model->GetOneQuestion(id, &q))
    {
        // 单个题目
        if (view->getViewOfOneQuestions(&q, html))
        {
            return true;
        }
        else
        {
            LOG(ERR) << "获取单个题目的 view 失败" << std::endl;
            return false;
        }
    }
    else
    {
        LOG(ERR) << "获取网页失败(单个题目)";
        *html = "获取网页失败(单个题目)";
        ret = false;
    }

    return ret;
}

void OjController::judgeQuestion(const std::string& number, const std::string& in_json,
                                 std::string* out_json)
{
    Question q;
    model->GetOneQuestion(number, &q);

    Json::Reader reader;
    Json::Value inValue;
    reader.parse(in_json, inValue);
    std::string inCode = inValue["code"].asString();

    LOG(INF) << inValue << std::endl;

    // 反序列化之后
    // 形成结构体 在 inValue里面
    Json::Value compileValue;
    compileValue["input"] = inValue["input"].asString();
    compileValue["code"] = inCode + q.tail; // 用户代码 + 测试代码 才是完整的

    compileValue["cpu_limit"] = q.cpu_limit;
    compileValue["mem_limit"] = q.mem_limit;
    Json::FastWriter writer;
    std::string requestContent = writer.write(compileValue);

    while (true)
    {
        int id = 0;
        Machine* machine = nullptr;
        if (!loadBlance->smartChoose(&id, &machine))
        {
            break;
        }
        // 选择了一台主机
        LOG(INFO) << " 选择主机成功: " << id << " 详情ip: " << machine->ip << ":" << machine->port
                  << std::endl;

        // 发起请求
        machine->updateLoad(true);
        httplib::Client client(machine->ip, machine->port);
        if (auto res = client.Post("/compile", requestContent, "application/json;charset=utf-8"))
        {
            machine->updateLoad(false);

            if (res->status == 200)
            {
                Json::Reader reader;
                Json::StyledWriter writer;
                Json::Value value;

                reader.parse(res->body, value);

                value["all_number"] = q.test_number;
                *out_json = writer.write(value);
                LOG(INFO) << "请求编译和运行服务成功" << std::endl;
                break;
            }
        }
        else
        {
            // 失败
            LOG(ERR) << "当前请求的主机" << machine->ip << ":" << machine->ip
                     << " 无响应 可能已离线" << std::endl;
            // 离线
            loadBlance->off(id);
            loadBlance->show();
        }
    }
}

} // namespace LosOj
/////////////////////////////////////////////

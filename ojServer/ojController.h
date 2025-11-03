#pragma once
#include <cstdint>
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>

#include "common/common.h"
#include "ojModel.h"
#include "ojView.h"

namespace LosOj
{
inline const std::string MACHINES_PATH = "./conf/serviceHost.conf";

using namespace LosCom;

class Machine
{
  public:
    std::string ip;
    int port;
    uint64_t load;
    std::mutex* mtx;

  public:
    Machine();
    ~Machine();
    void ResetLoad();
    void updateLoad(bool mode);
    uint64_t getLoad() const;
};

class LoadBlance
{
  private:
    std::vector<Machine> machines; // 可以提供编译服务的所有主机
    std::vector<int> onlineMachines;
    std::vector<int> outlineMachines;
    std::mutex mtx;

  public:
    LoadBlance();
    ~LoadBlance();
    bool smartChoose(int* id, Machine** m);
    void off(int id);
    void on();
    void show();

  public:
    bool LoadConf(const std::string& path);
};

class OjController
{
  public:
    OjController();
    ~OjController();

  public:
    void RecoveryMachine();
    bool allQuestions(std::string* html);
    bool oneQuestion(const std::string& id, std::string* html);
    void judgeQuestion(const std::string& number, const std::string& in_json,
                       std::string* out_json);

  private:
    std::shared_ptr<OjModel> model;
    std::shared_ptr<OjView> view;
    std::shared_ptr<LoadBlance> loadBlance;
};

}; // namespace LosOj
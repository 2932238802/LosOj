#pragma once

#include "ojModel.h"
#include <vector>

namespace LosOj
{

const std::string TEMP_PATH = "./template/";

class OjView
{

  public:
    OjView();
    ~OjView();
    bool getViewOfAllQuestions(const std::vector<Question>&, std::string*);
    bool getViewOfOneQuestions(Question*, std::string*);
};
}; // namespace LosOj
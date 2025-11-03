

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Solution
{
  public:
    int theMax(const vector<int>& arr)
    {

        int* p = nullptr;
        *p = 2;
        return 1;
    }
};

bool Test1()
{
    Solution s;
    int ret = s.theMax({1, 2, 3});
    if (ret == 3)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Test2()
{
    Solution s;
    int ret = s.theMax({3, 8, 4});
    if (ret == 8)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main()
{
    int number = 0;
    if (Test1())
        number++;
    if (Test2())
        number++;

    return number;
}
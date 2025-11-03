#ifndef COMPILE_ONLINE
#include "header.cc"
#endif

#include <vector>

bool Test1()
{
    Solution s;
    std::vector<int> result = s.nextGreaterElement({2, 1, 5});
    std::vector<int> expected = {5, 5, -1};
    if (result == expected)
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
    std::vector<int> result = s.nextGreaterElement({1, 3, 2, 4});
    std::vector<int> expected = {3, 4, 4, -1};
    if (result == expected)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Test3()
{
    Solution s;
    std::vector<int> result = s.nextGreaterElement({5, 4, 3, 2, 1});
    std::vector<int> expected = {-1, -1, -1, -1, -1};
    if (result == expected)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Test4()
{
    Solution s;
    std::vector<int> result = s.nextGreaterElement({1, 2, 3, 4, 5});
    std::vector<int> expected = {2, 3, 4, 5, -1};
    if (result == expected)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Test5()
{
    Solution s;
    std::vector<int> result = s.nextGreaterElement({4, 5, 2, 10, 8});
    std::vector<int> expected = {5, 10, 10, -1, -1};
    if (result == expected)
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
    if (Test3())
        number++;
    if (Test4())
        number++;
    if (Test5())
        number++;

    return number;
}
#ifndef COMPILE_ONLINE
#include "header.cc"
#endif

#include <algorithm>
#include <vector>

bool compareVectors(std::vector<int> a, std::vector<int> b)
{
    if (a.size() != b.size())
        return false;
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    return a == b;
}

bool Test1()
{
    Solution s;
    std::vector<int> result = s.twoSum({2, 7, 11, 15}, 9);
    std::vector<int> expected = {0, 1};
    return compareVectors(result, expected);
}

bool Test2()
{
    Solution s;
    std::vector<int> result = s.twoSum({3, 2, 4}, 6);
    std::vector<int> expected = {1, 2};
    return compareVectors(result, expected);
}

bool Test3()
{
    Solution s;
    std::vector<int> result = s.twoSum({3, 3}, 6);
    std::vector<int> expected = {0, 1};
    return compareVectors(result, expected);
}

bool Test4()
{
    Solution s;
    std::vector<int> result = s.twoSum({-1, -2, -3, -4, -5}, -8);
    std::vector<int> expected = {2, 4};
    return compareVectors(result, expected);
}

bool Test5()
{
    Solution s;
    std::vector<int> result = s.twoSum({0, 4, 3, 0}, 0);
    std::vector<int> expected = {0, 3};
    return compareVectors(result, expected);
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
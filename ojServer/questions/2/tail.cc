

#ifndef COMPILE_ONLINE
#include "header.cc"
#endif

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
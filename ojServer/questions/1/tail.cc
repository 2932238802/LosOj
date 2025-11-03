

#ifndef COMPILE_ONLINE
#include "header.cc"
#endif

bool Test1()
{
    Solution s;
    bool ret = s.isHuiwen(121);
    if (ret)
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
    bool ret = s.isHuiwen(132);
    if (!ret)
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
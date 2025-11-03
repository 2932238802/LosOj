#ifndef COMPILE_ONLINE
#include "header.cc"
#endif

#include <string>

bool Test1()
{
    Solution s;
    return s.isValid("()") == true;
}

bool Test2()
{
    Solution s;
    return s.isValid("()[]{}") == true;
}

bool Test3()
{
    Solution s;
    return s.isValid("(]") == false;
}

bool Test4()
{
    Solution s;
    return s.isValid("([)]") == false;
}

bool Test5()
{
    Solution s;
    return s.isValid("{[]}") == true;
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
#include <sys/resource.h>
#include <sys/time.h>

#include <iostream>

int main()
{
    struct rlimit r;
    r.rlim_cur = 1;
    r.rlim_max = RLIM_INFINITY;  // 这个表示无约束

    setrlimit(RLIMIT_CPU, &r);
    while (1);

    return 0;
}
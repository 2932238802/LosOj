#include <cstdlib>
#include <iostream>
#include <sched.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;
void func() { cout << " hello world! " << endl; }

int main()
{

    pid_t id = fork();

    if (id == 0)
    {
        func();
        exit(3);
    }
    else if (id > 0)
    {
        int state = 0;
        waitpid(id, &state, 0);

        if (WIFEXITED(state))
        {
            int excode = WEXITSTATUS(state);
            cout << excode << endl;
        }

        cout << "我是父进程 我在等待 子进程结束" << endl;
    }

    return 0;
}
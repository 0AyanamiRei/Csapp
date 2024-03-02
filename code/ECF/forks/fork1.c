#include "fork.c"

int main()
{
    pid_t pid;
    int x = 1;

    pid = Fork(); /* 创建子进程 */
    if(pid == 0) {
        printf("child :  x=%d, this pid : %d, it's parent's PID : %d\n", ++x, getpid(), getppid());
        exit(0);
    }

    printf("parent : x=%d, this pid : %d, it's child's PID : %d\n", --x, getpid(), pid);
    exit(0);
}
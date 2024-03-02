/****************************************************
这是一个具有非确定性的示范, 程序并不会按照顺序回收子进程 
****************************************************/ 

#include "/home/refrain/csappLab/code/ECF/mycsapp.c"
#define N 5

int main()
{
    int status, i;
    pid_t pid;

    /* 创建N个子进程 */
    for (i = 0; i < N; i++)
        if ((pid = Fork()) == 0) 
            exit(100 + i); /* 子进程返回状态100+i给父进程  */
    while ((pid = waitpid(-1, &status, 0)) > 0) /* 见mycsapp.c 函数解析1 */
    {
        if (WIFEXITED(status))
            printf("child %d terminated normally, exit status=%d\n", pid, WEXITSTATUS(status));
        else
            printf("child %d terminated abnormally\n", pid);
    }

    if (errno != ECHILD) /* 见mycsapp.c 函数解析1 */
        unix_error("waitpid error");

    exit(0);
}
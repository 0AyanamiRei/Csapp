/* Read me!
 *
 * 修改图8-18的程序,使每个子进程去写一个只读文本段, 然后父进程处理相应的异常,按照以下格式输出:
 * child 12255 terminated by signal 11: Segmentation fault
 * child 12254 terminated by signal 11: Segmentation fault
 */
/*** Just try and believe yourself !! ***/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#define N 2

typedef void (*sighandler_t)(int);
extern char etext; /* &etext会获取只读文本段的结束地址 */
char *ptr = &etext + 0x8;

void unix_error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

pid_t Fork(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
        unix_error("Fork error");
    return pid;
}

sighandler_t Signal(int signum, sighandler_t handler)
{
    if (signal(signum, handler) == SIG_ERR)
        unix_error("signal error");
}

void SIGSEGV_handler(int sig)
{
    exit(SIGSEGV);
}
/* 新知识:
    signal信号宏定义的常数只有8位(表示256个信号类型)
    在通过exit(SIGSEGV)返回的时候,会左移8位以便在一个16位的整数中包含信号编号, SIGSEGV<<8 = 2816
    需要注意的是,waitpid回收子进程记录的status等于exit(SIGSEGV)=2816,而不是11.
    但是WIFSIGNALED(status)根据原始的信号宏定义的值来判断是否是因为一个未被捕获的信号而中断
    所以我们要将status先右移8位再进行下一步
*/

int main()
{
    int status, i;
    pid_t pid;

    Signal(SIGSEGV, SIGSEGV_handler);
    /* 创建N个子进程 */
    for (i = 0; i < N; i++)
    {
        if ((pid = Fork()) == 0)
            *ptr = 'a'; /* 尝试写只读文本段的内容 */
    }
    while ((pid = waitpid(-1, &status, 0)) > 0)
    {
        status = status >> 8;
        if (WIFSIGNALED(status)) {
            int sig_num = WTERMSIG(status);
            printf("child %d terminated by signal %d: ", pid, sig_num);
            if(sig_num = SIGSEGV) {
                printf("Segmentation fault\n");
            }
            else {
                printf("Unknown fault\n");
            }
        }
        if(WIFEXITED(status)){
            printf("child %d terminated by status %d\n", pid, status);
        }
    }

    if (errno != ECHILD)
        unix_error("waitpid error");

    exit(0);
}
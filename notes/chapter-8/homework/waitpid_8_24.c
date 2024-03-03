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
#define LEN 64

// extern char etext; /* &etext会获取只读文本段的结束地址 */
// char *ptr = &etext + 0x8;

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

/* 新知识:
    signal信号宏定义的常数只有8位(表示256个信号类型)
    在通过exit(SIGSEGV)返回的时候,会左移8位以便在一个16位的整数中包含信号编号, SIGSEGV<<8 = 2816
    需要注意的是,waitpid回收子进程记录的status等于exit(SIGSEGV)=2816,而不是11.
    但是WIFSIGNALED(status)根据原始的信号宏定义的值来判断是否是因为一个未被捕获的信号而中断
    所以我们要将status先右移8位再进行下一步
*/

/* 2.0 核对答案检查之后发现可以简化一下,以下是修改内容

    1:并不需要去修改SIGSEGV信号默认的行为,反而会引发信号宏定义值的转变
    2:直接接收status的内容,不从信号处理函数中返回,这样就不需要修改status=status>>8
    3:使用void psignal(int sig, const char *s);函数可以直接打印导致进程退出的信号描述
    4:直接赋值地址ptr=系统文件存放的地址然后修改即可,不需要使用etext
*/
int main()
{
    int status, i;
    pid_t pid;

    //Signal(SIGSEGV, SIGSEGV_handler);
    /* 创建N个子进程 */
    for (i = 0; i < N; i++)
    {
        if ((pid = Fork()) == 0){
            char* ptr = NULL;
            *ptr = 'a';  /* 尝试写只读文本段的内容 */
        }
    }
    while ((pid = waitpid(-1, &status, 0)) > 0)
    {
        if(WIFEXITED(status)){
            printf("child %d terminated by status %d\n", pid, status);
        }
        else if (WIFSIGNALED(status)) {
            char buf[LEN];
            sprintf(buf, "child %d terminated by signal %d", pid, WTERMSIG(status));
            psignal(WTERMSIG(status), buf);
        }
        else{
            printf("child %d terminated abnormally\n", pid);
        }
    }
    if (errno != ECHILD)
        unix_error("waitpid error");
    exit(0);
}